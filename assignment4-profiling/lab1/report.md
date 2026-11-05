# Intro Profiling Lab Report

## Environment Note

This lab was completed on macOS (Apple Silicon, Apple Clang 21). The
Linux-only tools named in the walkthrough (`perf`, `gprof`, `valgrind`,
`callgrind`, `taskset`) are not available on macOS, so the following
equivalents were used, measuring the same quantities:

| README tool | macOS equivalent used |
|---|---|
| `time taskset -c 0 ./grid_bfs` | `time ./grid_bfs` |
| `perf record` + `perf report` | `sample <pid> 2 -file out.txt` (time sampling with call stacks) |
| FlameGraph | `sample` textual call tree (same sampled stacks, text form) |
| Valgrind Memcheck | `leaks <pid>` (macOS heap-leak detector) |
| Callgrind / KCachegrind | per-line attribution from `sample` (function + source-line view) |

`sample` still showed which source lines inside each function were hot, and
`leaks` tracked heap blocks allocated but never freed.

## 1. Optimizations Made

- **Fixed the BFS memory leak and removed per-request allocation churn.**
  `shortest_path_bfs` allocated `distance` (`new int[total]`) and `visited`
  (`new unsigned char[total]`) plus a `frontier` vector on every one of the
  1200 requests and never freed the two raw arrays. The buffers are now static
  scratch arrays reused across requests. This fixed the `definitely lost`
  leaks and removed the repeated large allocations (and their `sys` time).
- **Replaced full-array clearing with a generation stamp.** A cell counts as
  visited only when `visit_stamp[i] == current_stamp`, so BFS no longer pays
  for a 67600-element `fill`/`__bzero` per request.
- **Made the congestion pass cache-friendly.** `compute_congestion_pressure`
  walked a row-major array in column-major order (one cache line per cell).
  The inner loops now run row over column. Because every update reads only
  `current` and writes only `next`, the reordering does not change results.

## 2. Methodology Walkthrough

### Step 1: baseline run and `time`

Before:

```text
$ time ./grid_bfs
... time_sec = 0.745652
./grid_bfs  0.72s user 0.03s system 65% cpu 1.142 total
```

Most time is `user`, but `sys` (0.03s of 0.75s) was suspicious for a program
that does no I/O in its hot loops — an early hint of allocator/page-fault
activity.

### Step 2: sampling profile (`perf record/report` equivalent)

Before (612 samples on the main thread):

```text
370 main -> compute_congestion_pressure(...)   grid_bfs.cpp:368  (~60%)
242 main -> run_all_requests -> shortest_path_bfs(...)           (~40%)
            ! 20 __bzero  (in libsystem_platform.dylib)   <-- per-call clearing
```

Two clear hotspots: the congestion-pressure pass and BFS, plus visible
`__bzero` time inside `shortest_path_bfs` from zeroing `visited` every call.

### Step 3: leak check (Valgrind Memcheck equivalent)

Before:

```text
$ ./grid_bfs & leaks $!
Process 92240: 358 leaks for 64520192 total leaked bytes.
      1 (80.0K) ROOT LEAK: 0x75a718000 [81920]   ... (hundreds of blocks)
```

Hundreds of leaked blocks, two per BFS request — matching the two raw `new[]`
arrays in `shortest_path_bfs` that were never `delete[]`d.

### Step 4: fix, then measure again

After all changes:

```text
$ make test
./grid_bfs --test
sanity check passed

$ time ./grid_bfs          (3 runs)
0.43s user 0.00s system 98% cpu 0.436 total
0.43s user 0.00s system 98% cpu 0.436 total
0.43s user 0.00s system 98% cpu 0.435 total
```

```text
$ leaks $!
Process 93554: 0 leaks for 0 total leaked bytes.
```

After sampling profile (303 samples on the main thread, top-of-stack view):

```text
shortest_path_bfs(...)            306   (~79%, now the dominant cost)
compute_congestion_pressure(...)   79   (~20%, was ~60%)
(no __bzero / memset frames remain under BFS)
```

Summary: wall clock ~1.14s -> 0.44s, `user` 0.72s -> 0.43s, `sys` 0.03s ->
0.00s. The congestion pass dropped from ~60% to ~20% of samples after the
row-major reorder, and the BFS allocation/`__bzero` overhead disappeared from
the profile entirely. BFS graph search itself is now the remaining hotspot,
which is real work rather than overhead.

## 3. Correctness Evidence

`make test` output after optimization:

```text
./grid_bfs --test
sanity check passed
```

Final normal run (after):

```text
grid = 260 x 260
open_cells = 51400
requests = 1200
reachable = 1182
unreachable = 18
average_distance = 180.002
route_label_checksum = 16916932106541079682
heatmap_total_visits = 32813338
heatmap_active_cells = 51169
heatmap_max_visits = 962
heatmap_threshold_checksum = 16893359133801811161
congestion_passes = 4096
congestion_total_pressure = 3699909
congestion_max_pressure = 173
congestion_pressure_checksum = 2463005136280950131
```

Checksum comparison before vs. after optimization — all identical:

| field | before | after |
|---|---|---|
| route_label_checksum | 16916932106541079682 | 16916932106541079682 |
| heatmap_total_visits | 32813338 | 32813338 |
| heatmap_threshold_checksum | 16893359133801811161 | 16893359133801811161 |
| congestion_total_pressure | 3699909 | 3699909 |
| congestion_pressure_checksum | 2463005136280950131 | 2463005136280950131 |

## 4. Conceptual Questions

**Q1.1:** `real` is wall-clock time from start to finish, while `user + sys`
is CPU time actually spent executing our code (`user`) or kernel code on our
behalf (`sys`). They differ whenever the process is not running: scheduler
delays, waiting on page faults or I/O, and competing processes all add to
`real` without adding to `user + sys`. Conversely, a multithreaded program
can show `user + sys` greater than `real` because CPU time accumulates across
cores. In our runs `real` slightly exceeded `user + sys` due to process
startup (dyld) and scheduling gaps.

**Q2.1:** `perf stat` reads hardware performance-monitoring counters (PMUs)
that the CPU increments as events occur; the raw numbers (cycles,
instructions, branches, ...) are those counter values. Derived metrics are
simple ratios of two counters: `insn per cycle` = instructions / cycles,
`% of all branches` = branch-misses / branches, `% of all cache refs` =
cache-misses / cache-references. They characterize *how* the CPU spent its
time, not *where*.

**Q2.2:** The parenthesized percentages are counter multiplexing ratios: the
fraction of the total run during which that event's counter was actually
programmed into a physical PMU register. The CPU has fewer physical counters
than the eight events requested, so `perf` time-shares them; a counter active
only 24.94% of the time has its observed count scaled up to estimate the
full-run value. 100% would mean the counter was scheduled for the whole run.

**Q2.3:** No. When counters are multiplexed (see Q2.2), the printed number is
an extrapolation from partial measurement windows, not a literal count. Even
without multiplexing, PMU events count microarchitectural occurrences
(including speculative work that may be discarded) and suffer from sampling
skid and implementation-specific event definitions, so the figure is a close
estimate. It is reliable for relative comparisons between runs, not as an
exact tally.

**Q3.1:** A frame pointer is a dedicated register (rbp on x86-64, x29 on
arm64) that points at the base of the current stack frame. When every
function saves its caller's frame pointer and return address at a fixed
offset, the frames form a linked list through the stack. `perf record -g`
walks that chain at each sample: frame pointer -> caller's frame pointer ->
..., reading the return address at each step, which reconstructs the full
call stack. If the binary was built with frame pointers omitted, the chain is
broken and stack reconstruction fails or needs slower alternatives (DWARF
unwinding).

**Q3.2:** Self cost counts only the work done by a function's own
instructions, excluding everything its callees do. Inclusive cost
(`Children` in `perf report`, `total` in `gprof`, `Incl.` in Callgrind)
counts self cost plus the entire cost of all functions it called. `main`
typically has ~100% inclusive but ~0% self, while a hot leaf like the
congestion inner loop has high self cost. Optimization targets usually come
from high self cost; high inclusive cost tells you which call paths matter.

**Q4.1:** `gprof` gets call counts from compile-time instrumentation: `-pg`
makes the compiler insert an `mcount` call at every function entry. At
runtime `mcount` reads the caller's return address off the stack, so each
call increments a counter for that specific (caller -> callee) arc; at exit
these exact arc counts are written to `gmon.out`. Time attribution, by
contrast, is still statistical: a periodic timer interrupt records the
program counter into a histogram, so `gprof`'s seconds per function are
sampled estimates while its call counts are exact.

**Q4.2:** Because `perf`/FlameGraphs only sample, they cannot give exact call
counts, and at `-O2` inlining can hide helper functions entirely. `gprof`
complements them with exact per-arc call counts — e.g. showing that a helper
like `next_pressure_value` is invoked hundreds of millions of times — which
quantifies *how often* code runs, not just *how long*. That structural
information helps explain the sampling profile, at the cost of measuring a
different, instrumented (`-O0 -pg`) binary.

**Q5.1:** Valgrind Memcheck instruments an existing binary dynamically: no
recompilation needed, and it detects invalid reads/writes, leaks, and use of
uninitialized values, but it is 10-50x slower and weak on stack/global buffer
overflows. AddressSanitizer instruments at compile time
(`-fsanitize=address`): only ~2x slowdown, excellent at heap/stack/global
overflow and use-after-free, but it requires a rebuild, uses more memory, and
does not detect uninitialized reads (that needs MSan). Use ASan during
day-to-day development and CI for cheap, broad checks; use Memcheck when you
cannot recompile (third-party binaries) or when you specifically need leak
and uninitialized-value detection on an unmodified executable.

**Q6.1:** Yes, but the disagreements are methodological, not real
contradictions. First, `gprof`'s total attributed time (~10 s in the README
example) differs from `perf stat`'s wall time (~1.8 s) because `gprof`
measures a different binary: `-O0 -pg` removes inlining and adds an `mcount`
call to every function, which is especially expensive for helpers called
hundreds of millions of times. Second, `perf` at `-O2` attributes nearly all
congestion cost to `compute_congestion_pressure` itself because
`next_pressure_value` was inlined, while `gprof` lists the helper as a
separate high-cost function — same real work, attributed differently due to
inlining plus instrumentation vs. sampling. Both tools agree on the substance:
the congestion pass and BFS are the two dominant costs, which is exactly what
our `sample`-based profiles showed on macOS before and after optimizing.
