/*
 * Assignment 6: Declarative Pipelines
 * Ported by Tinkercademy from the "Modern-Only C++ Course" homework
 * (Lesson 6) by Simon Toth (HappyCerberus), used under the MIT License.
 * See ATTRIBUTION.md. Re-themed for the Tinkercademy Bootcamp.
 *
 * Complete each STUDENT TODO below. Read the README carefully: the point
 * of this lab is to express WHAT you want declaratively (ranges, views,
 * concepts, folds) rather than HOW to loop over it.
 *
 * This lab targets C++23. Compile with: g++ -std=c++23 main.cpp -o main
 */

#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>
#include <ranges>
#include <stdexcept>
#include <string>
#include <utility>

/* ------------------------------------------------------------------ *
 * Part 1: format_range                                               *
 * ------------------------------------------------------------------ */

/**
 * Shared helper: formats any range into `s` as "{a<delim>b<delim>c}".
 * The trailing-delimiter problem is solved with std::exchange: the
 * delimiter starts empty and is swapped in only after the first element.
 */
template <std::ranges::range Rng>
std::ostream& format_range_with_delimiter(std::ostream& s, const Rng& rng,
                                          const char* delim) {
  s << '{';
  const char* d = "";
  for (const auto& elem : rng) {
    s << std::exchange(d, delim) << elem;
  }
  s << '}';
  return s;
}

/**
 * Formats any range into `s` as "{a,b,c}". An empty range prints "{}".
 * Requirements:
 *   - Constrain the template to `std::ranges::range`.
 *   - Elements are separated by a single ',' (no trailing comma).
 */
template <std::ranges::range Rng>
std::ostream& format_range(std::ostream& s, const Rng& rng) {
  return format_range_with_delimiter(s, rng, ",");
}

/* ------------------------------------------------------------------ *
 * Part 2: format_categorized                                         *
 * ------------------------------------------------------------------ */

/**
 * Like format_range, but the delimiter advertises how powerful the
 * range's iterators are:
 *   - random-access range -> ","
 *   - bidirectional range -> "<=>"
 *   - forward range       -> "->"
 * Requirements:
 *   - Provide three overloads selected by range concept (most-derived
 *     concept wins). Do not inspect a tag by hand; let overload
 *     resolution pick.
 */
template <std::ranges::forward_range Rng>
std::ostream& format_categorized(std::ostream& s, const Rng& rng) {
  return format_range_with_delimiter(s, rng, "->");
}

template <std::ranges::bidirectional_range Rng>
std::ostream& format_categorized(std::ostream& s, const Rng& rng) {
  return format_range_with_delimiter(s, rng, "<=>");
}

template <std::ranges::random_access_range Rng>
std::ostream& format_categorized(std::ostream& s, const Rng& rng) {
  return format_range_with_delimiter(s, rng, ",");
}

/* ------------------------------------------------------------------ *
 * Part 3: trim_whitespace (a compound view)                          *
 * ------------------------------------------------------------------ */

/**
 * A range adaptor object that removes leading and trailing whitespace
 * from a bidirectional character range. Usage: `str | trim_whitespace`.
 * Requirements:
 *   - Build it by composing existing views (drop_while, reverse). No
 *     hand-written loop, no copying into a new container.
 */
// STUDENT TODO: Replace the identity placeholder below with a compound view
// that drops leading AND trailing whitespace. Hint: std::views::drop_while
// removes a leading run; std::views::reverse lets you reach the other end.
// (The placeholder `std::views::all` compiles but does no trimming, so the
// trim_whitespace tests will fail until you implement this.)
inline constexpr auto is_space = [](unsigned char c) {
  return std::isspace(c) != 0;
};

inline constexpr auto trim_whitespace = std::views::drop_while(is_space) |
                                        std::views::reverse |
                                        std::views::drop_while(is_space) |
                                        std::views::reverse;

/* ------------------------------------------------------------------ *
 * Part 4: valid_card (Luhn check)                                    *
 * ------------------------------------------------------------------ */

/**
 * Returns whether `digits` (a range of single digits, most-significant
 * first) passes the Luhn checksum. Each element is already an int in 0-9
 * (e.g. std::vector<int>{4, 1, 1, 1}), not a char, so no `c - '0'` needed.
 * Requirements:
 *   - Express the check declaratively. You MUST use C++23 features here:
 *     std::views::stride to pick every second digit and
 *     std::ranges::fold_left to sum. No manual index bookkeeping in a raw
 *     loop, and no std::accumulate.
 */
template <std::ranges::bidirectional_range Rng>
bool valid_card(const Rng& digits) {
  // Walk from the rightmost digit: reverse once, then stride(2) picks the
  // digits left alone, and drop(1) | stride(2) picks the digits to double.
  auto reversed = digits | std::views::reverse;

  auto undoubled = reversed | std::views::stride(2);

  auto doubled = reversed | std::views::drop(1) | std::views::stride(2) |
                 std::views::transform([](int d) {
                   d *= 2;
                   return d > 9 ? d - 9 : d;
                 });

  const int total =
      std::ranges::fold_left(undoubled, 0, std::plus<>{}) +
      std::ranges::fold_left(doubled, 0, std::plus<>{});
  return total % 10 == 0;
}

/* #### Please don't remove this line! #### */
#include "tests/utils.hpp"
