"""Data Science hands-on: NumPy and pandas primer.

Follows the second "Deferred Hands-on" of the lecture, using ideas from the
Python Data Science Handbook (https://github.com/jakevdp/PythonDataScienceHandbook):
ndarray internals, views vs copies, vectorisation, and DataFrames as
Struct-of-Arrays.

Run with:  python3 numpy_pandas_primer.py   (requires numpy and pandas)
"""

import numpy as np
import pandas as pd


# --- ndarray: fixed dtype, contiguous memory, shape + strides ---
a = np.arange(12, dtype=np.int64).reshape(3, 4)
assert a.dtype == np.int64
assert a.shape == (3, 4)
# strides = bytes to skip to reach the next element along each axis
assert a.strides == (4 * a.itemsize, a.itemsize)

# Slicing creates a VIEW (like std::span), not a copy
sub = a[1:, 1:3]
assert np.shares_memory(a, sub)
sub[0, 0] = -1
assert a[1, 1] == -1               # mutation through the view hits the original

# --- Vectorisation beats Python loops ---
rng = np.random.default_rng(42)
big = rng.random(1_000_000)
loop_sum = sum(big.tolist())       # Python-level loop
vec_sum = big.sum()                # vectorised: runs in C, releases the GIL
assert abs(loop_sum - vec_sum) < 1e-6

x = np.arange(5)
assert np.array_equal(x + 10, np.array([10, 11, 12, 13, 14]))
assert np.array_equal(x * x, np.array([0, 1, 4, 9, 16]))
assert np.array_equal(np.sqrt(np.array([1.0, 4.0, 9.0])), np.array([1.0, 2.0, 3.0]))

# Broadcasting: (3,1) + (1,4) -> (3,4)
b = np.arange(3).reshape(3, 1) + np.arange(4)
assert b.shape == (3, 4)
assert np.array_equal(b[2], np.array([2, 3, 4, 5]))

# Aggregations and boolean masking
m = np.arange(10)
assert m[m % 2 == 0].tolist() == [0, 2, 4, 6, 8]
assert m.mean() == 4.5 and m.max() == 9


# --- pandas: Series (1D) and DataFrame (2D, Struct-of-Arrays) ---
balances = pd.Series([120.5, 80.0, 200.25], index=["ada", "brian", "cleo"])
assert balances["brian"] == 80.0
assert balances.mean() > 133.0

df = pd.DataFrame({
    "name": ["ada", "brian", "cleo", "dana"],
    "degree": ["MSc", "MSc", "PhD", "PhD"],
    "balance": [120.5, 80.0, 200.25, 150.0],
})
# Each column is backed by one contiguous array (SoA)
assert df["balance"].dtype == np.float64
assert df["balance"].mean() == (120.5 + 80.0 + 200.25 + 150.0) / 4

# Vectorised column ops, filtering, groupby
df["balance_gbp"] = df["balance"] * 1.0
rich = df[df["balance"] > 100]
assert rich["name"].tolist() == ["ada", "cleo", "dana"]
by_degree = df.groupby("degree")["balance"].mean()
assert by_degree["MSc"] == (120.5 + 80.0) / 2

# Missing data handling
df.loc[4] = ["erin", "MSc", np.nan, np.nan]
assert df["balance"].isna().sum() == 1
assert df["balance"].mean() > 0                       # NaN skipped by default
assert df["balance"].fillna(0.0).isna().sum() == 0

print("data_science OK")
