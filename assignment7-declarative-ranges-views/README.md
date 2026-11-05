# Assignment 6: Declarative Pipelines

## Overview

So far you've been telling the computer *how* to do things — set up an
index, loop, mutate, repeat. This assignment is about the opposite mindset:
**declarative** programming, where you describe *what* you want and let the
library figure out the *how*. You'll lean on modern **ranges**, **views**,
**concepts**, and **folds** to write code that reads like a description of the
result.

> [!IMPORTANT]
> This lab targets **C++23** and you are expected to use C++23 declarative
> features — in particular `std::ranges::fold_left` and `std::views::stride`
> (and other C++23 range adaptors where they help). Compile everything with
> `-std=c++23`.
>
> **Use GCC 13 or newer** (GCC 15 is known good). `std::views::stride` is
> required, and libstdc++ has shipped it since GCC 13. Heads-up: as of this
> writing **libc++ (the standard library Clang uses by default, including
> Apple Clang on macOS) does not implement `std::views::stride`** — even
> Clang 21 fails to compile it — so the lab will **not** build with a stock
> Clang toolchain. Older `-std=c++20` compilers will **not** build this lab
> either; that part is intentional.

> [!NOTE]
> Before starting, watch:
> - [Back to Basics: (Range) Algorithms in C++ — Klaus Iglberger, CppCon 2023](https://youtu.be/eJCA2fynzME)
> - [C++20 Ranges in Practice — Tristan Brindle](https://youtu.be/d_E-VLyUnzc)

All of your code goes into `main.cpp`. There are four small tasks, each
building toward the same idea: composition over iteration.

- `main.cpp` — all your code goes here.
- `short_answer.txt` — short answer responses go here.

## Running your code

Make sure you are in the `assignment6-declarative-ranges-views/` directory and
run:

```sh
g++ -std=c++23 main.cpp -o main
```

> [!NOTE]
>
> ### Note for macOS
>
> On macOS, the `g++` (and `clang++`) on your `PATH` is **Apple Clang**, which
> uses libc++ and **cannot build this lab** (no `std::views::stride`). Install
> a real GCC and call it directly:
>
> ```sh
> brew install gcc          # provides g++-15
> g++-15 -std=c++23 main.cpp -o main
> ```

Then:

```sh
./main --test                 # run every test
./main --test trim_whitespace # run a single named test
./main --demo                 # see each function/view in action
```

The tests live in `tests/utils.hpp`. Use `--test` as your feedback loop.

> [!NOTE]
>
> ### Note for Windows
>
> On Windows you may need `-static-libstdc++`, and the executable may be
> `main.exe`:
>
> ```sh
> g++ -static-libstdc++ -std=c++23 main.cpp -o main
> ./main.exe --test
> ```

> [!IMPORTANT]
> This lab is about *declarative* style. With the exception of the small loop
> you may need to consume a view, **prefer ranges/views/algorithms over raw
> `for`/`while` loops** in your solutions. If you find yourself writing manual
> index bookkeeping, step back and look for a view that already does it.

---

## Part 1: `format_range`

> [!IMPORTANT]
> ### `format_range`
>
> Print any range to a `std::ostream` as `{a,b,c}`. An empty range prints
> `{}`; a single element prints `{x}` — note there is **no trailing
> delimiter**.
>
> **Requirements**
> - Constrain the template parameter to `std::ranges::range`.
> - Separate elements with a single `,`.
>
> **Hint:** the trailing-delimiter problem has a tidy idiom using
> `std::exchange`. Start the delimiter empty, then swap in `,` after the first
> element.

## Part 2: `format_categorized`

The delimiter you print can advertise *how powerful* a range's iterators are.
This is a chance to see **overload resolution on concepts** in action.

> [!IMPORTANT]
> ### `format_categorized`
>
> Same braces-and-delimiters idea as Part 1, but choose the delimiter by the
> strongest range concept the type satisfies:
>
> | Range concept            | Example container      | Delimiter |
> | ------------------------ | ---------------------- | --------- |
> | `random_access_range`    | `std::vector`          | `,`       |
> | `bidirectional_range`    | `std::list`            | `<=>`     |
> | `forward_range`          | `std::forward_list`    | `->`      |
>
> **Requirements**
> - Write **three overloads**, each constrained by one of the concepts above.
> - Do **not** branch on a type tag yourself — let overload resolution pick
>   the most specific match.

> [!TIP]
> The only thing that changes between these overloads (and Part 1) is the
> delimiter. Rather than copy-paste the braces-and-`std::exchange` logic four
> times, factor it out once into a helper that takes the delimiter as a
> parameter — e.g. `format_range_with_delimiter(s, rng, "->")` — and have each
> overload (and `format_range`) delegate to it. The public API stays the same.

> [!NOTE]
> Why does the `random_access_range` overload win for a `std::vector`, even
> though a vector also satisfies `forward_range` and `bidirectional_range`?
> Think about concept subsumption — you'll explain this in `short_answer.txt`.

## Part 3: `trim_whitespace` (a compound view)

> [!IMPORTANT]
> ### `trim_whitespace`
>
> Build a **range adaptor object** so that `str | trim_whitespace` yields the
> string with leading and trailing whitespace removed, but **inner whitespace
> preserved**. For example `"  a b c \t"` becomes `"a b c"`.
>
> **Requirements**
> - Compose it from existing views — `std::views::drop_while` and
>   `std::views::reverse` are all you need. No hand-written loop, and **no
>   copying into a new container**.
>
> **Hint:** dropping a *leading* run of whitespace is one `drop_while`.
> Trimming the *trailing* run is the same operation — once you `reverse` the
> range so the end becomes the beginning.

## Part 4: `valid_card` (Luhn check)

The [Luhn algorithm](https://en.wikipedia.org/wiki/Luhn_algorithm) is the
checksum that validates credit-card numbers. Walking from the rightmost digit:
double every second digit (and if the result exceeds 9, subtract 9), sum
everything, and the number is valid iff the total is divisible by 10.

> [!IMPORTANT]
> ### `valid_card`
>
> Given a range of single digits (most-significant digit first), return
> whether it passes the Luhn checksum. Each element is already an `int` in
> `0–9` (e.g. `std::vector<int>{4, 1, 1, 1}`), **not** a character — the test
> helper has already converted characters to integers, so you do **not** need
> any `c - '0'` conversion inside `valid_card`.
>
> **Requirements**
> - Express it declaratively. You **must** use the C++23 features
>   `std::views::stride` (to pick every second digit) and
>   `std::ranges::fold_left` (to sum). **Do not** use `std::accumulate` or a
>   raw loop with manual index bookkeeping.
>
> **Hint:** `range | std::views::reverse | std::views::stride(2)` gives you
> every second digit starting from the right. Pair it with
> `std::views::drop(1)` to get the *other* digits, `std::views::transform` to
> double them, and `std::ranges::fold_left(rng, 0, std::plus<>{})` to total
> each group.

---

## Short answer questions

Answer briefly (2–4 sentences each) in `short_answer.txt`:

1. **Laziness.** A `views` pipeline does no work until it is iterated. Explain
   one concrete benefit of this for `trim_whitespace` compared with a function
   that returns a trimmed `std::string`.
2. **Concept subsumption.** In Part 2, why does the `random_access_range`
   overload win for a `std::vector`, even though the vector also satisfies the
   other two concepts?
3. **Lifetime of views.** `trim_whitespace` produces a view that
   refers back to the original string. What goes wrong if the underlying
   string is destroyed (or modified) while the view is still in use?
4. **Declarative vs. imperative.** Pick one of your four solutions and
   describe what the equivalent hand-written loop would look like. Which is
   easier to read, and why?

---

## Attribution

This assignment is **ported from the "Modern-Only C++ Course" homework
(Lesson 6) by RNDr. Simon Toth (HappyCerberus)**, used under the MIT License.
The original tasks (`format_range`, the concept-overloaded formatter,
`trim_whitespace`, and Luhn validation) were written against C++23; this
version keeps the C++23 feature set and is re-themed for the Tinkercademy
Bootcamp. See [`ATTRIBUTION.md`](./ATTRIBUTION.md) for the full license text.
