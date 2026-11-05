# Attribution

This assignment is adapted from the **"Modern-Only C++ Course" homework
(Lesson 6)** by RNDr. Simon Toth (HappyCerberus):

- Source: https://github.com/HappyCerberus/daily-bite-course-06
- Author: RNDr. Simon Toth (HappyCerberus)
- License: MIT (see full text below)

## Changes made

- Kept the original **C++23** feature set: the Luhn task uses
  `std::ranges::fold_left` and `std::views::stride`, so the lab requires a
  C++23 compiler (GCC 13+, Clang 16+, MSVC 19.34+) built with `-std=c++23`.
- Replaced the Bazel + Catch2 build/test setup with a single-translation-unit
  layout and the lightweight header-only test harness used by the other
  assignments in this repository (`tests/utils.hpp`, `./main --test`).
- Renamed the advanced formatter to `format_categorized` so the basic and
  concept-overloaded versions can coexist in one file, and re-themed the
  handout text for the Tinkercademy Bootcamp.

## Original License

```
MIT License

Copyright (c) 2023 RNDr. Simon Toth

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
