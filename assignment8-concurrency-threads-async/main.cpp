/*
 * Assignment 7: Parallel Letter Frequency
 * Ported by Tinkercademy from the Exercism C++ track exercise
 * "Parallel Letter Frequency", used under the MIT License.
 * See ATTRIBUTION.md. Re-themed and extended with a std::async part.
 *
 * Complete each STUDENT TODO below. Read the README carefully: the point
 * of this lab is to do work concurrently and combine the results safely.
 */

#include <algorithm>
#include <cctype>
#include <future>
#include <stdexcept>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <vector>

/* ------------------------------------------------------------------ *
 * Part 1: count_letters (sequential baseline)                        *
 * ------------------------------------------------------------------ */

/**
 * Counts how often each letter appears in `text`.
 * Requirements:
 *   - Case-insensitive: 'A' and 'a' both count as 'a'.
 *   - Ignore everything that is not an alphabetic character.
 */
std::unordered_map<char, std::size_t> count_letters(std::string_view text) {
  std::unordered_map<char, std::size_t> counts;
  for (unsigned char c : text) {
    if (std::isalpha(c)) {
      counts[static_cast<char>(std::tolower(c))] += 1;
    }
  }
  return counts;
}

/* ------------------------------------------------------------------ *
 * Part 2: frequency_seq (sequential baseline over many texts)        *
 * ------------------------------------------------------------------ */

/**
 * Counts the combined letter frequency across many texts, sequentially.
 * This is the baseline the parallel versions below are compared against
 * (see `./main --time`).
 * Requirements:
 *   - Reuse `count_letters` and merge the per-text counts into one map.
 *   - No threads here: this is the single-threaded reference.
 */
std::unordered_map<char, std::size_t>
frequency_seq(const std::vector<std::string_view>& texts) {
  std::unordered_map<char, std::size_t> combined;
  for (std::string_view text : texts) {
    for (const auto& [c, n] : count_letters(text)) {
      combined[c] += n;
    }
  }
  return combined;
}

/* ------------------------------------------------------------------ *
 * Part 3: frequency (parallel with std::thread)                      *
 * ------------------------------------------------------------------ */

/**
 * Counts the combined letter frequency across many texts, in parallel.
 * Requirements:
 *   - Use std::thread to split the work across multiple workers.
 *   - Each worker must accumulate into its OWN local map; combine the
 *     partial maps after joining. (That way there is no shared mutable
 *     state, so no mutex is required on the hot path.)
 *   - The result must equal the sequential answer regardless of how the
 *     work is partitioned.
 */
std::unordered_map<char, std::size_t>
frequency(const std::vector<std::string_view>& texts) {
  const std::size_t n = texts.size();
  if (n == 0) return {};

  // Pick a worker count: hardware concurrency, but never more workers
  // than there are texts (empty workers would just be wasted threads).
  unsigned int hw = std::thread::hardware_concurrency();
  const std::size_t workers =
      std::max<std::size_t>(1, std::min<std::size_t>(hw == 0 ? 1 : hw, n));

  // Pre-sized so threads only write into their OWN slot: no resizing,
  // no shared mutable state, no mutex needed on the hot path.
  std::vector<std::unordered_map<char, std::size_t>> partials(workers);
  std::vector<std::thread> threads;
  threads.reserve(workers);

  // Partition the texts into `workers` contiguous chunks.
  const std::size_t chunk = n / workers;
  const std::size_t rem = n % workers; // first `rem` workers take one extra
  std::size_t begin = 0;
  for (std::size_t w = 0; w < workers; ++w) {
    const std::size_t len = chunk + (w < rem ? 1 : 0);
    const std::size_t end = begin + len;
    threads.emplace_back([&texts, &partials, w, begin, end] {
      std::unordered_map<char, std::size_t>& local = partials[w];
      for (std::size_t i = begin; i < end; ++i) {
        for (const auto& [c, n] : count_letters(texts[i])) {
          local[c] += n;
        }
      }
    });
    begin = end;
  }

  for (auto& t : threads) t.join(); // join every thread before returning

  // Sequential merge of the partial maps.
  std::unordered_map<char, std::size_t> combined;
  for (const auto& local : partials) {
    for (const auto& [c, n] : local) {
      combined[c] += n;
    }
  }
  return combined;
}

/* ------------------------------------------------------------------ *
 * Part 4: frequency_async (std::async / std::future)                 *
 * ------------------------------------------------------------------ */

/**
 * Same result as `frequency`, but expressed with std::async + futures
 * instead of managing std::thread objects by hand.
 * Requirements:
 *   - Launch the per-text work with std::async(std::launch::async, ...).
 *   - Collect each future's result and combine them.
 */
std::unordered_map<char, std::size_t>
frequency_async(const std::vector<std::string_view>& texts) {
  // std::launch::async forces each task onto its own thread (no deferred,
  // lazy-on-get() execution). Futures give us the result as a return
  // value, so no manual join() and no captured output variable.
  std::vector<std::future<std::unordered_map<char, std::size_t>>> futures;
  futures.reserve(texts.size());
  for (std::string_view text : texts) {
    futures.push_back(std::async(std::launch::async, count_letters, text));
  }

  std::unordered_map<char, std::size_t> combined;
  for (auto& fut : futures) {
    for (const auto& [c, n] : fut.get()) {
      combined[c] += n;
    }
  }
  return combined;
}

/* #### Please don't remove this line! #### */
#include "tests/utils.hpp"
