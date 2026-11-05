#!/usr/bin/env bash
# Run every hands-on exercise for assignment6 (Python primer).
# Usage: ./run_all.sh [python_executable]
# Chapters 1-10 need only the standard library; data_science needs numpy/pandas.
set -euo pipefail

PY="${1:-python3}"
cd "$(dirname "$0")"

# chapter01 is the interactive guessing game: feed it the secret number
echo 42 | "$PY" chapter01/guessing_game.py

for ch in chapter02 chapter03 chapter04 chapter05 chapter06 chapter07 chapter08 chapter10; do
    for f in "$ch"/*.py; do
        "$PY" "$f"
    done
done

# chapter09: run the importing script (geometry.py is the module it imports)
"$PY" chapter09/use_modules.py

# data science part requires numpy and pandas
if "$PY" -c "import numpy, pandas" 2>/dev/null; then
    "$PY" data_science/numpy_pandas_primer.py
else
    echo "SKIP data_science (numpy/pandas not installed for $PY)"
fi

echo "ALL OK"
