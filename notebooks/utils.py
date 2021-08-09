"""Utility functions for working with CPAC datasets

Author: Peter Volgyesi <peter.volgyesi@gmail.com>
"""

from pathlib import Path
from urllib.parse import urlparse

import numpy as np
import pandas as pd
import boto3
import sagemaker

def download_dataset(s3_uri):
    """Download S3 dataset (if not available localy), return local path"""
    bucket, path = sagemaker.s3.parse_s3_url(s3_uri)
    local_path = Path(path)
    if not local_path.exists():
        sagemaker.s3.S3Downloader.download(s3_uri, local_path.parent)
    return local_path

def load_dataset(s3_uri_or_path):
    """Load a local or S3 dataset into a pandas dataframe"""
    if urlparse(s3_uri_or_path).scheme == "s3":
        local_path = download_dataset(s3_uri_or_path)
    else:
        local_path = s3_uri_or_path
    if str(local_path.suffix).lower() == ".csv":
        df = pd.read_csv(local_path)
    elif str(local_path.suffix).lower() in (".xlsx", ".xls"):
        df = pd.read_excel(local_path)
    return df

### Simple tests
if __name__ == "__main__":
    s3_uri = "s3://cpac/ORIG/S1_lumbar_results.csv"
    print(f"Loading: {s3_uri}")
    df = load_dataset(s3_uri)
    print(df.describe())