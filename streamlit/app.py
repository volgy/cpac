#!/usr/bin/env python3
"""Experiment with the boosting ML methods using Streamlit

Author: Peter Volgyesi <peter.volgyesi@gmail.com>

"""
import re
from pathlib import Path
from urllib.parse import urlparse

import boto3
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import streamlit as st

from sklearn import linear_model, ensemble
from sklearn import metrics, preprocessing, pipeline, model_selection

KNOWN_DATASETS = {
    "PXV_10_15_19": (
        "s3://cpac/ORIG/"
        "SpinePredictions_500taks_PXV_1S_10_15_19/"
        "lumbar_results_PXV_10_15_19.csv"
    ),
    "PXV_10_15_19 V2": (
        "s3://cpac/ORIG/"
        "SpinePredictions_500taks_PXV_1S_10_15_19/"
        "lumbar_results2_PXV_10_15_19.csv"
    ),
    "PXV_10_15_19 V3": (
        "s3://cpac/ORIG/"
        "SpinePredictions_500taks_PXV_1S_10_15_19/"
        "lumbar_results3_PXV_10_15_19.csv"
    )
}

META_PATTERNS = [
    "Trial_Name.*",
    "Trial_Index.*",
    "Trial_Type.*",
    "Mass.*"
]

TARGET_PATTERNS = [
    "Pelvis_Moment_X.*",
    "Pelvis_Moment_Y.*",
    "Pelvis_Moment_Z.*"
]

@st.cache(persist=True, show_spinner=False)
def load_dataset(s3_uri):
    """Download CSV dataset from S3"""
    parts = urlparse(s3_uri)
    path = parts.path.lstrip(" /")
    filename = Path(path).name
    bucket = parts.netloc
    with st.spinner(f"Loading {filename}"):
        s3 = boto3.resource('s3')
        s3_obj = s3.Object(bucket, path).get()
        return pd.read_csv(s3_obj["Body"])

def select_solumns(df):
    """Select target and feature columns"""
    metas = []
    features = []
    targets = []
    for column in df.columns:
        kind = features
        for pattern in META_PATTERNS:
            if re.match(pattern, column):
                kind = metas
        for pattern in TARGET_PATTERNS:
            if re.match(pattern, column):
                kind = targets
        kind.append(column)

    selected_target = st.sidebar.selectbox("Target", targets)
    selected_features = st.sidebar.multiselect(
        "Features", features, default=features)

    return selected_target, selected_features, metas

def filter_dataset(df, target, features, metas):
    """Keep only relevant data"""
    na_strategy = st.sidebar.radio("Drop N/A", ("Samples", "Features"))
    drop_axis = 0 if na_strategy == "Samples" else 1
    keep = [target] + features + metas
    ignored = set(df.columns) - set(keep)
    return df[keep].dropna(axis=drop_axis)

def analyze_feature(df, feature):
    """Analyze and plot a single feature"""
    with st.spinner(f"Analyzing {feature}"):
        sns.distplot(df[feature])
        st.pyplot()

def train_test_split(df, metas):
    """Split dataset to train/test subsets"""
    # TODO: hacks
    meta = metas[0]
    meta_options = df[meta].unique().tolist()
    default_option = [option for option in meta_options
                     if re.match("S0[0-9]_5kg", option)]
    test_meta = st.sidebar.multiselect(
        "Select Test Set", meta_options, default=default_option)
    is_test = df[meta].isin(test_meta)
    return df[~is_test], df[is_test]

@st.cache
def _lasso_trainer(df_train, df_test, target, features):
    reg = pipeline.Pipeline([
            ('scaler', preprocessing.StandardScaler()),
            ('lasso', linear_model.Lasso())
        ])
    reg.fit(df_train[features], df_train[target])
    mse = metrics.mean_squared_error(
            reg.predict(df_test[features]), df_test[target])
    return reg, mse

def lasso_regression(df_train, df_test, target, features):
    st.header("LASSO Regression")

    with st.spinner("Training in progress..."):
        reg, mse = _lasso_trainer(df_train, df_test, target, features)

    st.write(f"**MSE**: {mse:.2f}")

    ind = np.arange(len(features))
    plt.bar(ind, np.abs(reg.named_steps["lasso"].coef_))
    plt.xticks(ind, features, rotation=90)
    plt.title("Feature importance with Lasso");
    plt.xlabel("Features")
    plt.ylabel("Importance (coefficient)")
    st.pyplot()

    plt.figure()
    plt.plot(df_test[target], reg.predict(df_test[features]), 'k.', alpha=.2)
    plt.grid()
    plt.title(f"Correlation of predicted values with Lasso (MSE={mse:.2f})")
    st.pyplot()

def gradient_boosting(df_train, df_test, target, features):
    st.header("Gradient Boosting")
    with st.spinner("Training in progress..."):
        scaler = preprocessing.StandardScaler()
        X_train = scaler.fit_transform(df_train[features])
        y_train = np.ravel(df_train[target])
        X_test = scaler.transform(df_test[features])
        y_test = np.ravel(df_test[target])

        gboost = ensemble.GradientBoostingRegressor(
            n_estimators=50
        )
        gboost.fit(X_train, y_train)
        mse = metrics.mean_squared_error(gboost.predict(X_test), y_test)

    st.write(f"**MSE**: {mse:.2f}")

    ind = np.arange(len(features))
    plt.bar(ind, np.abs(gboost.feature_importances_))
    plt.xticks(ind, features, rotation=90)
    plt.title("Relative feature importance with Gradient Boosting");
    plt.xlabel("Features")
    plt.ylabel("Relative Importance")
    st.pyplot()

    plt.figure()
    plt.plot(y_test, gboost.predict(X_test), 'k.', alpha=.2)
    plt.grid()
    plt.title(f"Correlation of predicted values with Gradient Boosting (MSE={mse:.2f})");
    st.pyplot()

def main():
    """Life starts here"""
    st.title("CPAC Boosting Experiments")

    dataset_name = st.sidebar.selectbox("Dataset",
                                        tuple(KNOWN_DATASETS.keys()))
    df = load_dataset(KNOWN_DATASETS[dataset_name])

    ml_methods = {
        "LASSO": lasso_regression,
        "Gradient Boosting": gradient_boosting
    }
    ml_method = st.sidebar.selectbox("Machine Learning Approach",
                                     tuple(ml_methods.keys()))

    target, features, metas = select_solumns(df)
    df = filter_dataset(df, target, features, metas)

    st.header("Dataset Information")
    st.write(f"**Number of samples**: {len(df):,}")
    st.write(f"**Meta fields**: {', '.join(metas)}")
    st.write(df.describe())

    st.header("Features")
    st.write(f"**Number of selected features**: {len(features)}")
    show_feature = st.selectbox("", features)
    analyze_feature(df, show_feature    )

    st.header("Train / Test Split")
    df_train, df_test = train_test_split(df, metas)
    n_train, n_test = len(df_train), len(df_test)
    st.write(f"**Training set size:**: {n_train:,} ({n_train / (n_train + n_test):.0%})")
    st.write(f"**Test set**: {n_test:,} ({n_test / (n_train + n_test):.0%})")

    ml_methods[ml_method](df_train, df_test, target, features)

if __name__ == "__main__":
    main()
