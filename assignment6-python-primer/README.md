# Assignment 6: Python Primer

Hands-on exercises from the lecture "Python for C++ Programmers"
([slides](https://docs.google.com/presentation/d/18mWrXh0xDLk5mfe-i0O9WQJCFeM5x79GAApEBOiims4/edit?usp=sharing)):

1. Imperial College's [Python for C++ Programmers](https://python.pages.doc.ic.ac.uk/cpp/lessons/cpp/01-intro/)
   course ("Everything is an object"), chapters 1-10.
2. NumPy/pandas primer following the
   [Python Data Science Handbook](https://github.com/jakevdp/PythonDataScienceHandbook).

## Structure

| File | Covers |
|---|---|
| `chapter01/guessing_game.py` | Example program: script-style Python, no `main`, indentation blocks |
| `chapter02/basic_types.py` | Numbers, booleans, strings, multiline strings |
| `chapter03/everything_is_an_object.py` | Everything is an object; constructors; aliasing |
| `chapter04/sequences.py` | Lists, slicing, tuples, string formatting/escaping |
| `chapter05/sets_and_dicts.py` | Set operations, dict methods, grouping data with dict |
| `chapter06/control_flow.py` | if/elif/else, loops, range, enumerate, zip, comprehensions |
| `chapter07/functions.py` | Default/keyword args, functions as objects, `*args`/`**kwargs` |
| `chapter08/oop.py` | Classes, dunder methods, inheritance, `@property` encapsulation |
| `chapter09/geometry.py`, `use_modules.py` | Custom modules, `__name__ == "__main__"` |
| `chapter10/files_and_pickle.py` | Text/JSON/CSV files; **exercise 10.6**: pickle a list of `Vector`s |
| `data_science/numpy_pandas_primer.py` | ndarray internals, views, vectorisation, DataFrame as SoA |

Every file is self-checking via `assert` statements.

## Running

```bash
./run_all.sh                 # uses python3
./run_all.sh /path/to/python # chapters 1-10 need only the stdlib;
                             # data_science additionally needs numpy + pandas
```
