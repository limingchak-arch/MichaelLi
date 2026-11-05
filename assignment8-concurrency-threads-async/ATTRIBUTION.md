# Attribution

This assignment is adapted from the **Exercism C++ track exercise "Parallel
Letter Frequency"**:

- Source: https://exercism.org/tracks/cpp/exercises/parallel-letter-frequency
- Repository: https://github.com/exercism/cpp
- Author: Exercism and contributors
- License: MIT (see full text below)

## Changes made

- Kept the core task — `frequency(std::vector<std::string_view>)` counting
  combined letter frequency across many texts in parallel.
- Added a sequential `count_letters` baseline (Part 1), a `frequency_seq`
  sequential combiner (Part 2), and a `frequency_async` variant (Part 4) built
  on `std::async` / `std::future`.
- Added a `./main --time` benchmark (`tests/utils_timer.h`) that compares the
  sequential, threaded, and async implementations on a larger corpus.
- Specified `std::thread`-based parallelism with per-thread local maps instead
  of relying on C++17 parallel execution policies (which require TBB on
  libstdc++), so the lab builds with a plain `g++ -std=c++20 -pthread`.
- Replaced the CMake + Catch2 build/test setup with the lightweight
  header-only test harness used by the other assignments in this repository
  (`tests/utils.hpp`, `./main --test`), and re-themed the handout for the
  Tinkercademy Bootcamp.

## Original License

```
MIT License

Copyright (c) 2021 Exercism

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
