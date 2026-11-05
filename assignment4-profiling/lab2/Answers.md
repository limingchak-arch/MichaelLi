# Performance Assignment — Answers

Environment: macOS (Apple Silicon, Apple Clang 21). `perf`/`kcachegrind` are
Linux-only, so profiling was done with the macOS `sample` time profiler
(call-stack sampling, text output attached). Because `-O2` inlines everything
into `main`, profiling builds were made with `-O2 -g -fno-inline` so hot
functions stay visible — same optimization level, readable attribution.

## How the hotspots were found

### Baseline, `history_cols = 128`

`./main` prints `6040578838`. Wall time is only ~23 ms, most of it process
startup, so aggregated sampling over 5 runs of a `-fno-inline` build
(`sample_128_before_raw.txt`) shows the compute concentrated in:

```text
14  <deduplicated_symbol>                  (inlined-adjacent hot loops)
 5  chase_dependency(int, int, ...)
 5  branchy_score(Packet const&, ...)
```

### Baseline, `history_cols = 2048`

`./main` prints `6745589558`; min wall time 180.4 ms over 15 runs. Sampling
the `-fno-inline` build (`sample_prof_2048.txt`, top-of-stack counts):

```text
95  <deduplicated_symbol>             (inner loops, see below)
72  cold_column_probe(...)            main.cpp:172  <- column-major walk
30  refresh_history(...)              main.cpp:98/104-106
 6  branchy_score(...)
```

The call graph in the same file attributes the `deduplicated_symbol` samples
to the inner loops of `cold_column_probe` (line 172) and `refresh_history`.
So the 2048 hotspots are:

1. `cold_column_probe` — walks a row-major buffer **column by column**
   (rotated by `seed`), one cache line per access, over a 32 MB buffer.
2. `refresh_history` — data-dependent scatter writes into the same 32 MB
   buffer, plus the per-row prefix pass.
3. `branchy_score` — six data-dependent if/else branches per packet.

## Optimizations applied

### 1. `cold_column_probe`: visit-order independence -> row-major sweep

The rotation `(start_col + offset) % cols` covers **every** column exactly
once, so every cell is summed exactly once and the total does not depend on
visit order. Replaced the column-major rotated walk with a single sequential
range-for sweep over `history`. Identical result, prefetch-friendly access.
This was the single biggest win at 2048.

### 2. `branchy_score`: branchless via ternaries

The six conditions on `x` are pseudo-random per packet, so the if/else chain
mispredicts constantly. Rewritten as ternaries with exactly the same
operations (`+=` vs `^=` on the same operands), which Clang compiles to
conditional selects. Identical result, no branches.

### 3. `refresh_history` scatter: tried, measured, rejected

Since `history[idx] = (history[idx] + reading + quality) & 2047` accumulates
contributions associatively mod 2048, packets can be applied in any order.
I implemented a variant that sorts packet indices by history index once and
then writes sequentially. A/B measurement at 2048 (min of 8 runs):

```text
direct scatter: 0.05s user
sorted scatter: 0.06s user
```

The one-time sort plus indirect packet reads cost more than the improved
write locality saves on this machine (its large system cache absorbs much of
the 32 MB scatter), so the change was reverted and the direct scatter kept.
A comment in the code records the experiment.

### Not worth fixing

`chase_dependency` is a serial pointer-chasing chain (`idx = next[idx]`, 7
steps); each load depends on the previous one, so it cannot be reordered or
parallelized without changing semantics. With the constant `STEPS = 7` the
compiler already unrolls it after inlining. Left as is.

## Results

| configuration | baseline | optimized | speedup |
|---|---|---|---|
| `history_cols = 128` | 22.8 ms min / 23.9 ms median (40 runs) | 20.0 ms min / 21.6 ms median | ~1.12x |
| `history_cols = 2048` | 180.4 ms min / 189.5 ms median (15 runs) | 53.3 ms min / 56.4 ms median | ~3.4x |

## Correctness

- `history_cols = 128`: `make test` -> `baseline output matched`
  (`6040578838`, unchanged).
- `history_cols = 2048`: prints `6745589558`, unchanged from the unoptimized
  starter.
- No `build_` function was modified; no function signatures or call sites
  changed. (`cold_column_probe` keeps its signature; unused parameters are
  marked `(void)`.)

## Post-optimization profile at 2048

`sample_prof_2048_after1.txt` (same `-fno-inline` method): the remaining
samples are dominated by process startup (`dyld`), the one-time 32 MB
`history` zero-fill, and the now-sequential `cold_column_probe` /
`refresh_history` memory passes — i.e. the remaining cost is memory
bandwidth and startup, not fixable compute. `branchy_score` dropped from a
visible hotspot to 6 samples.
