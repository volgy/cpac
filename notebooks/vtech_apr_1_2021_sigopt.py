#!/usr/bin/env python
"""Hyper-parameter optimization for the VTech dataset"""
import sys
import numpy as np
import pandas as pd
import tensorflow as tf
from tensorflow import keras
from scipy.io import wavfile
from sigopt import Connection


def print_df_summary(df):
    print(f"Number of workers: {len(df['worker'].unique())}")
    for worker, worker_df in df.groupby("worker"):
        print(f"\tWorker {worker}: {len(worker_df['day'].unique())} days"
            f", {len(worker_df['trial'].unique())} trials"
            f", {worker_df['mode'].diff().abs().sum():3.0f} transitions"
            f", {len(worker_df):10,} samples")
    print(f"Total number of transitions: {df['mode'].diff().abs().sum()}")


def build_dataset(df, window_size, features, target, **kwargs):

    columns = features + [target]
    def make_windowed_dataset(ds, window_size, shift=1):
        windows = ds.window(window_size, shift=shift)

        def sub_to_batch(sub):
            return sub.batch(window_size, drop_remainder=True)

        def add_labels(batch):
            return batch[:, :-1], batch[-1, -1]
            #return batch[:, :-1], batch[0, -1]

        windows = windows.flat_map(sub_to_batch)
        return windows.map(add_labels)


    ds = None
    for trial_id, trial_data in df.groupby(["worker", "trial"]):
        ds_trial = make_windowed_dataset(
            tf.data.Dataset.from_tensor_slices(trial_data.loc[:,columns].values),
            window_size=window_size
        )
        if ds is None:
            ds = ds_trial
        else:
            ds = ds.concatenate(ds_trial)

    return ds

def build_model(ds, window_size, features, layer_1, layer_2, **kwargs):
    normalization = tf.keras.layers.experimental.preprocessing.Normalization()
    normalization.adapt(ds.take(100000).map(lambda x, y: x))


    model = keras.Sequential(
        [
            keras.layers.Input(shape=(window_size, len(features))),
            normalization,
            keras.layers.Flatten(),
            keras.layers.Dense(layer_1, activation="relu", name="layer1"),
            keras.layers.Dense(layer_2, activation="relu", name="layer2"),
            keras.layers.Dense(1, activation= "sigmoid" ),
        ]
    )

    model.compile(optimizer='adam',
              loss=tf.keras.losses.binary_crossentropy,
              metrics=['accuracy'])

    return model

def build_experiment(conn):
    experiment = conn.experiments().create(
        name='VTech Apr 1, 2021',
        # Define which parameters you would like to tune
        parameters=[
            dict(name='window_size', type='int', bounds=dict(min=2, max=200)),
            dict(name='layer_1', type='int', bounds=dict(min=4, max=256)),
            dict(name='layer_2', type='int', bounds=dict(min=2, max=32))
        ],
        metrics=[dict(name='accuracy', objective='maximize')],
        parallel_bandwidth=1,
        # Define an Observation Budget for your experiment
        observation_budget=30,
        project="cpac",
    )
    return experiment


def train_and_evaluate(model, ds, epochs, **kwargs):
    model.fit(ds.shuffle(buffer_size=2**18).batch(128), epochs=epochs)
    loss, accuracy = model.evaluate(ds.batch(128), batch_size=128)
    return accuracy

def main():

    hyper_params = {
        "window_size": 10,
        "layer_1": 16,
        "layer_2": 4,
        "features": [
            "orientation_T8_q0",
            "orientation_T8_q1",
            "orientation_T8_q2",
            "orientation_T8_q3",
        ],
        "target": "mode",
        "epochs": 5
    }

    print("Loading dataset....")
    df = pd.read_csv(f"../data/VTech/_VTech_merged.csv")
    print_df_summary(df)

    conn = Connection()
    experiment = build_experiment(conn)
    print(f"Created experiment: https://app.sigopt.com/experiment/{experiment.id}")

    for _ in range(experiment.observation_budget):
        suggestion = conn.experiments(experiment.id).suggestions().create()
        assignments = suggestion.assignments

        hyper_params["window_size"] = assignments["window_size"]
        hyper_params["layer_1"] = assignments["layer_1"]
        hyper_params["layer_2"] = assignments["layer_2"]

        print(f"Evaluating with: {hyper_params}")
        ds = build_dataset(df, **hyper_params)
        model = build_model(ds, **hyper_params)
        accuracy = train_and_evaluate(model, ds, **hyper_params)
        print(f"Accuracy: {accuracy}")


        conn.experiments(experiment.id).observations().create(
            suggestion=suggestion.id,
            value=accuracy
        )

    assignments = conn.experiments(experiment.id).best_assignments().fetch().data[0].assignments

    print(assignments)


if __name__ == "__main__":
    main()
