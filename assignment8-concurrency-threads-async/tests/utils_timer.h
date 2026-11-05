/* ========================================================================= *
 * Timing harness for Assignment 7: Parallel Letter Frequency.                *
 *                                                                           *
 * Provides `./main --time`, which benchmarks frequency_seq vs frequency vs   *
 * frequency_async on a larger corpus so you can see whether going parallel   *
 * actually pays off. Each implementation is checked against the sequential   *
 * reference before its time is reported.                                     *
 *                                                                           *
 * You don't need to read this file to complete the assignment.              *
 * ========================================================================= */

#pragma once

#include <chrono>
#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace tests {

// Runs `fn(texts)` once, verifies the result against `expected`, and prints
// the wall-clock time. Returns the elapsed milliseconds, or -1.0 on mismatch.
template <typename Fn>
inline double benchmark_frequency_version(
    const char* label,
    Fn&& fn,
    const std::vector<std::string_view>& texts,
    const std::map<char, std::size_t>& expected) {
  const auto start = std::chrono::steady_clock::now();
  const auto result = fn(texts);
  const auto end = std::chrono::steady_clock::now();

  const std::map<char, std::size_t> sorted(result.begin(), result.end());
  if (sorted != expected) {
    std::cout << label << ": result mismatch\n";
    return -1.0;
  }

  const std::chrono::duration<double, std::milli> elapsed = end - start;
  std::cout << label << ": " << elapsed.count() << " ms\n";
  return elapsed.count();
}

inline int run_timing_demo() {
  // Build a benchmark corpus of a MODEST number of LARGE texts. The text count
  // is deliberately bounded: frequency_async launches one thread per text, so
  // tens of thousands of short texts would mean tens of thousands of threads
  // (extremely slow, and on many systems a std::system_error from thread
  // exhaustion). A few hundred long texts give plenty of total work while
  // keeping the thread count sane and the seq/threads/async comparison fair.
  constexpr std::size_t num_texts = 256;
  constexpr std::size_t blocks_per_text = 400; // corpus copies packed per text

  std::vector<std::string> benchmark_storage;
  benchmark_storage.reserve(num_texts);
  for (std::size_t i = 0; i < num_texts; ++i) {
    std::string big;
    for (std::size_t b = 0; b < blocks_per_text; ++b)
      for (const auto& text : corpus_storage) {
        big += text;
        big += ' ';
      }
    benchmark_storage.push_back(std::move(big));
  }

  std::vector<std::string_view> benchmark_views;
  benchmark_views.reserve(benchmark_storage.size());
  for (const auto& text : benchmark_storage) benchmark_views.emplace_back(text);

  const auto expected = reference_counts(benchmark_views);
  std::size_t total_chars = 0;
  for (std::string_view v : benchmark_views) total_chars += v.size();

  std::cout << "Benchmarking letter-frequency implementations\n";
  std::cout << "Texts: " << benchmark_views.size() << "  (~" << total_chars / 1000
            << "k chars total)\n";

  bool ok = true;
  ok &= benchmark_frequency_version("frequency_seq", frequency_seq,
                                    benchmark_views, expected) >= 0.0;
  ok &= benchmark_frequency_version("frequency", frequency,
                                    benchmark_views, expected) >= 0.0;
  ok &= benchmark_frequency_version("frequency_async", frequency_async,
                                    benchmark_views, expected) >= 0.0;

  return ok ? 0 : 1;
}

} // namespace tests
