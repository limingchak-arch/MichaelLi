# Assignment 7: Parallel Letter Frequency

## Overview

Counting how often each letter appears in a pile of texts is the kind of
problem that is *embarrassingly parallel*: the texts don't depend on each
other, so you can count them at the same time and merge the results. This
assignment uses that problem to introduce **concurrency** in modern C++ —
`std::thread`, safe result-combining, and `std::async`/`std::future`.

All of your code goes into `main.cpp`. There are four tasks that build on
each other.

- `main.cpp` — all your code goes here.
- `short_answer.txt` — short answer responses go here.

## Running your code

Make sure you are in the `assignment7-concurrency-threads-async/` directory.
Concurrency needs the threading library, so **compile with `-pthread`**:

```sh
g++ -std=c++20 -pthread main.cpp -o main
```

Then:

```sh
./main --test              # run every test
./main --test frequency    # run a single named test
./main --demo              # see the parallel counter in action
./main --time              # benchmark seq vs threads vs async
```

The `--time` mode runs each implementation over a larger corpus and prints the
wall-clock time, so you can see whether going parallel actually pays off.

The tests live in `tests/utils.hpp`. They compare your parallel answers
against an independent sequential reference, so the result must match exactly.

> [!IMPORTANT]
> Concurrency bugs are often *nondeterministic* — a race can pass nine times
> and fail the tenth. The `stress` test deliberately runs many texts over many
> iterations to flush these out. If it fails even occasionally, you have a real
> bug, not bad luck.

> [!NOTE]
> ### Catching races with a sanitizer
> Once your code compiles, run it under **ThreadSanitizer** to catch data
> races the tests might miss:
> ```sh
> g++ -std=c++20 -pthread -fsanitize=thread -g main.cpp -o main && ./main --test
> ```
> A correct solution reports no warnings.

---

## Part 1: `count_letters`

> [!IMPORTANT]
> ### `count_letters`
>
> Count how often each letter appears in a single `std::string_view`.
>
> **Requirements**
> - Case-insensitive: `'A'` and `'a'` both count toward `'a'`.
> - Ignore anything that isn't an alphabetic character (digits, spaces,
>   punctuation).
>
> This is the sequential building block the next three parts reuse.

## Part 2: `frequency_seq` (sequential baseline)

> [!IMPORTANT]
> ### `frequency_seq`
>
> Count the **combined** letter frequency across a list of texts, on a single
> thread.
>
> **Requirements**
> - Reuse `count_letters` on each text and merge the per-text counts into one
>   map.
> - No threads here — this is the straightforward, single-threaded version.
>
> This is the baseline the parallel versions are measured against. Run
> `./main --time` to compare it with `frequency` and `frequency_async` and see
> how much (if anything) the threads buy you.

## Part 3: `frequency` (with `std::thread`)

> [!IMPORTANT]
> ### `frequency`
>
> Count the **combined** letter frequency across a list of texts, using
> multiple threads.
>
> **Requirements**
> - Use `std::thread` to split the texts across several workers (a good
>   default is `std::thread::hardware_concurrency()` workers).
> - Give each worker its **own local map**, then merge the partial maps after
>   you `join()` them.
> - The result must equal the sequential answer for any partitioning.

> [!IMPORTANT]
> **Why no mutex?** If every thread wrote into one shared `std::unordered_map`,
> you'd have a data race — concurrent writes to the same container are
> undefined behaviour. By giving each thread a private map and only merging
> *after* joining, the parallel phase touches no shared mutable state, so no
> lock is needed on the hot path. You'll explain this trade-off in
> `short_answer.txt`.

> [!NOTE]
> Remember to `join()` every thread before the function returns. A `std::thread`
> that is destroyed while still joinable calls `std::terminate()`.

## Part 4: `frequency_async` (with `std::async`)

> [!IMPORTANT]
> ### `frequency_async`
>
> Produce the **same result** as Part 2, but with `std::async` and
> `std::future` instead of managing thread objects yourself.
>
> **Requirements**
> - Launch the per-text work with `std::async(std::launch::async, ...)`.
> - Collect each `future`'s result with `.get()` and combine them.
>
> Notice how much bookkeeping the futures remove — no manual `join()`, and the
> result comes back as a return value instead of a captured variable.

> [!NOTE]
> Passing `std::launch::async` *forces* a new thread. Without it, the
> implementation is allowed to defer the work and run it lazily on `.get()` —
> which would be sequential, not parallel. You'll be asked why this matters.

---

## Short answer questions

Answer briefly (2–4 sentences each) in `short_answer.txt`:

1. **Data races.** What exactly is a data race, and why would having all
   worker threads write into one shared `std::unordered_map` be undefined
   behaviour — even if you "got the right answer" once?
2. **Why the merge is sequential.** Your workers run in parallel but the final
   merge of partial maps runs on one thread. Why is that the right call here,
   and when (if ever) would it become a bottleneck?
3. **`std::launch::async` vs deferred.** What is the difference between
   `std::async(std::launch::async, f)` and `std::async(f)`? Why does it matter
   for *this* assignment?
4. **Thread cost.** Spawning a thread per text would create thousands of
   threads for a large corpus. Why is that usually a bad idea, and what is the
   common fix? (One or two sentences — no need to implement it.)

---

## Attribution

This assignment is **ported from the Exercism C++ track exercise
["Parallel Letter Frequency"](https://exercism.org/tracks/cpp/exercises/parallel-letter-frequency)**,
used under the MIT License. The core task (`frequency` over a list of texts) is
from Exercism; the `count_letters` baseline and the `frequency_async` part were
added for this lab, and the handout was re-themed for the Tinkercademy
Bootcamp. See [`ATTRIBUTION.md`](./ATTRIBUTION.md) for the full license text.
