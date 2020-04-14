#!/usr/bin/env python3
"""Experiment with the boosting ML methods using Streamlit

Author: Peter Volgyesi <peter.volgyesi@gmail.com>

"""
import time
import re
from pathlib import Path
from urllib.parse import urlparse

import boto3
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import streamlit as st

from sklearn.experimental import enable_hist_gradient_boosting  # noqa
from sklearn import (
    linear_model,
    ensemble,
    metrics,
    preprocessing,
    pipeline,
    model_selection,
    inspection
)

KNOWN_DATASETS = {
    "PXV_10_15_19 (lab-based)": (
        "s3://cpac/ORIG/"
        "SpinePredictions_500taks_PXV_1S_10_15_19/"
        "lumbar_results_PXV_10_15_19.csv"
    ),
    "PXV_10_15_19 V2 (wearables)": (
        "s3://cpac/ORIG/"
        "SpinePredictions_500taks_PXV_1S_10_15_19/"
        "lumbar_results2_PXV_10_15_19.csv"
    ),
    "PXV_10_15_19 V3 (lab-based subset)": (
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
    st.sidebar.header("Select columns")
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

@st.cache(persist=True)
def _filter_dataset(
    df_orig,
    target,
    trial_name,
    filter_idp,
    filter_walk,
    filter_nan,
    target_range):

    df = df_orig.copy()
    if filter_idp:
        df = df[~df[trial_name].str.startswith("IDP")]
    if filter_walk:
        df = df[~df[trial_name].str.contains("walk")]
    if filter_nan:
        df = df.dropna()
    df = df[df[target].between(*target_range)]
    return df

def filter_dataset(df, target, features, metas):
    """Keep only relevant data"""
    st.sidebar.header("Select filters")
    filter_idp = st.sidebar.checkbox(
        "Remove 'IDP' trials", value=True)
    filter_walk = st.sidebar.checkbox(
        "Remove 'walk' trials", value=True)
    filter_nan = st.sidebar.checkbox(
        "Remove trials with NaN", value=True)

    # TODO: plot target distribution
    target_vals = df[target]
    def_low, def_high = target_vals.quantile(0.01), target_vals.quantile(0.99)
    target_range = st.sidebar.slider(
        "Valid target range",
        df[target].min(), df[target].max(),
        (def_low, def_high))

    # TODO: hack
    trial_name = metas[0]

    return _filter_dataset(
        df, target, trial_name, filter_idp, filter_walk,
        filter_nan, target_range)

def train_test_split(df, metas):
    """Split dataset to train/test subsets"""
    st.sidebar.header("Train - Test Split")
    # TODO: hack
    trial_name = metas[0]

    meta_options = df[trial_name].unique().tolist()

    # TODO: hack
    default_option = [option for option in meta_options
                     if re.match("S0[0-9]_5kg", option)]
    test_meta = st.sidebar.multiselect(
        "Select Test Set", meta_options, default=default_option)
    is_test = df[trial_name].isin(test_meta)
    return df[~is_test], df[is_test]

def analyze_feature(df, feature):
    """Analyze and plot a single feature"""
    with st.spinner(f"Analyzing {feature}"):
        sns.distplot(df[feature])
        st.pyplot()


def gradient_boosting(df_train, df_test, target, features):
    #st.header("Gradient Boosting (Histogram-based)")
    st.header("Results")
    t_start = time.time()
    with st.spinner("Training in progress..."):
        X_train = df_train[features]
        y_train = df_train[target]

        model = pipeline.Pipeline([
            ('scaler', preprocessing.StandardScaler()),
            ('gboost', ensemble.HistGradientBoostingRegressor(max_iter=200))
        ]).fit(X_train, y_train)
        t_train = time.time() - t_start

    t_start = time.time()
    with st.spinner("Testing in progress...."):
        X_test = df_test[features]
        y_test = df_test[target]
        y_test_pred = model.predict(X_test)
        mse = metrics.mean_squared_error(y_test_pred, y_test)
        r2 = metrics.r2_score(y_test_pred, y_test)
        t_test = time.time() - t_start

    st.write(f"**MSE**: {mse:.2f}")
    st.write(f"**R^2 Score**: {r2:.3f}")
    st.write(f"_Time train: {t_train:.3f} s, test: {t_test:.3f} s_")

    ax = sns.jointplot(y_test, y_test_pred, alpha=0.1, s=1.0, color="black")
    #sns.jointplot(y_test, y_test_pred, kind="hex")
    #sns.jointplot(y_test, y_test_pred, kind="reg")
    ax.set_axis_labels("Actual Pelvis Moment", 'Predicted Pelvis Moment')
    #ax.ax_joint.legend_.remove()
    ax.ax_joint.grid()
    #ax.ax_marg_x.set_title(f"Correlation of actual vs. predicted moments")
    st.pyplot()


    st.sidebar.header("Feature importances")
    do_imps = st.sidebar.checkbox(
        "Compute Feature Importance", value=False)

    if do_imps:
        n_repeats = st.sidebar.number_input(
            label="Repeats",
            value=10,
            min_value=1
        )

        st.header("Feature importances")
        st.write("The permutation feature importance is defined to be the "
                "decrease in a model score when a single feature value is "
                "randomly shuffled.")

        with st.spinner("Computing permutation importance...."):
            imps = inspection.permutation_importance(
                model,
                X_test,
                y_test,
                n_repeats=n_repeats,
                random_state=42,
                n_jobs=-1
            )
            sorted_imps_idx = imps.importances_mean.argsort()

        fig, ax = plt.subplots()
        ax.boxplot(imps.importances[sorted_imps_idx].T,
            vert=False, showfliers=False,
            labels=X_test.columns[sorted_imps_idx])
        fig.tight_layout()
        st.pyplot()

def main():
    """Life starts here"""
    st.title("CPAC - Gradient Boosting Regression")

    st.sidebar.header("Select dataset")
    dataset_name = st.sidebar.selectbox("Dataset",
                                        tuple(KNOWN_DATASETS.keys()))
    df = load_dataset(KNOWN_DATASETS[dataset_name])

    target, features, metas = select_solumns(df)
    df = filter_dataset(df, target, features, metas)

    st.header("Dataset Information")
    #st.write(f"**Number of samples**: {len(df):,}")
    #st.write(f"**Meta fields**: {', '.join(metas)}")
    st.write(df.describe())

    # st.header("Features")
    # st.write(f"**Number of selected features**: {len(features)}")
    # show_feature = st.selectbox("", features)
    # analyze_feature(df, show_feature    )

    st.header("Train - Test Sets")
    df_train, df_test = train_test_split(df, metas)
    n_train, n_test = len(df_train), len(df_test)
    st.write(f"{n_train:,} (**{n_train / (n_train + n_test):.0%}**) - "
             f"{n_test:,} (**{n_test / (n_train + n_test):.0%}**)")

    gradient_boosting(df_train, df_test, target, features)

def sandbox():
    import time
    my_bar = st.progress(0)
    for percent_complete in range(100):
        time.sleep(0.01)
        my_bar.progress(percent_complete + 1)

    with st.spinner('Wait for it...'):
        time.sleep(2)
    st.success("Done")
    with st.spinner('Take two...'):
        time.sleep(2)

if __name__ == "__main__":
    main()
    #sandbox()
