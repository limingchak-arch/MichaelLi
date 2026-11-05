/*
 * Assignment 1: Marriage Pact
 * Adapted by Tinkercademy from Stanford CS106L
 * (originally by Haven Whitney, with modifications by Fabio Ibanez
 * & Jacob Roberts-Baca).
 *
 * Complete each STUDENT TODO below. Read the README carefully — the
 * requirements there (ranges, projections, sample, reserve, no raw
 * for-loops in find_matches, iterator-safe erase in run_mixer) are
 * part of the assignment, not optional polish.
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <random>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

/**
 * Reads `filename` line by line and returns the applicants.
 *
 * Requirements:
 *   - Take `filename` as `const std::string&`.
 *   - Call `reserve()` before populating, with a sensible capacity.
 *     Justify your choice in short_answer.txt.
 */
std::vector<std::string> get_applicants(const std::string& filename) {
  std::ifstream file(filename);
  std::vector<std::string> applicants;
  applicants.reserve(1000);
  std::string line;
  while (std::getline(file, line)) {
    if (!line.empty()) applicants.push_back(line);
  }
  return applicants;
}

/**
 * Returns the initials of `name`, uppercased.
 *   e.g. initials("Marceline McMillan") == "MM"
 *
 * Requirements:
 *   - Parameter must be `std::string_view` (no allocation).
 */
std::string initials(std::string_view name) {
  auto space = name.find(' ');
  char first  = static_cast<char>(std::toupper(static_cast<unsigned char>(name[0])));
  char second = static_cast<char>(std::toupper(static_cast<unsigned char>(name[space + 1])));
  return std::string{first, second};
}

/**
 * Returns every applicant in `students` who shares initials with `name`.
 *
 * Requirements:
 *   - No raw `for` loops. Use std::ranges::copy_if (or views::filter
 *     piped into a vector). Use a projection where it makes the call
 *     clearer.
 *   - Take `students` as `const std::vector<std::string>&`.
 */
std::vector<std::string> find_matches(std::string_view name,
                                      const std::vector<std::string>& students) {
  std::vector<std::string> matches;
  std::string target = initials(name);
  std::ranges::copy_if(students, std::back_inserter(matches),
      [&target](const std::string& s) { return initials(s) == target; });
  return matches;
}

/**
 * Returns one randomly-chosen match, or "NO MATCHES FOUND." if empty.
 *
 * Requirements:
 *   - Use std::sample with a seeded std::mt19937.
 *   - Do NOT use pop_back() or rand() % size.
 */
std::string get_match(const std::vector<std::string>& matches) {
  if (matches.empty()) return "NO MATCHES FOUND.";
  std::random_device rd;
  std::mt19937 gen(rd());
  std::string result;
  std::sample(matches.begin(), matches.end(), &result, 1, gen);
  return result;
}

/**
 * Runs a multi-round mixer. In each round, scan the remaining
 * applicants left-to-right; for each applicant, look for another
 * applicant with the same initials still in the pool. If found,
 * pair them, remove both from `applicants`, and record the pair.
 * Continue rounds until a full pass yields no new pairs.
 *
 * `applicants` is mutated: paired names are removed. Whatever is
 * left over at the end is unpaired.
 *
 * Requirements:
 *   - The naive "iterate and erase as you go" approach WILL invalidate
 *     your iterator. You must handle this — see the README for the
 *     three acceptable strategies — and document your choice in
 *     short_answer.txt.
 */
std::vector<std::pair<std::string, std::string>>
run_mixer(std::vector<std::string>& applicants) {
  std::vector<std::pair<std::string, std::string>> pairs;
  bool found_pair = true;
  while (found_pair) {
    found_pair = false;
    for (auto it = applicants.begin(); it != applicants.end(); ) {
      auto match = std::find_if(it + 1, applicants.end(),
          [&](const std::string& s) { return initials(s) == initials(*it); });
      if (match != applicants.end()) {
        pairs.emplace_back(*it, *match);
        match = applicants.erase(match);
        it = applicants.erase(it);
        found_pair = true;
      } else {
        ++it;
      }
    }
  }
  return pairs;
}

/* #### Please don't remove this line! #### */
#include "tests/utils.hpp"
