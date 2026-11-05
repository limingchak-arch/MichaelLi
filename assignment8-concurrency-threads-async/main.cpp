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
  // STUDENT TODO: Implement this function.
  throw std::runtime_error("Not implemented: count_letters");
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
  // STUDENT TODO: Implement this function.
  throw std::runtime_error("Not implemented: frequency_seq");
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
  // STUDENT TODO: Implement this function.
  throw std::runtime_error("Not implemented: frequency");
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
  // STUDENT TODO: Implement this function.
  throw std::runtime_error("Not implemented: frequency_async");
}

/* #### Please don't remove this line! #### */
#include "tests/utils.hpp"
