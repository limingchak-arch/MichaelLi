/* ========================================================================= *
 * Test harness for Assignment 7: Parallel Letter Frequency.                  *
 *                                                                           *
 * This file provides main(). It runs a small demo (./main --demo) and a     *
 * test mode (./main --test [name]) that checks the functions you implement  *
 * in main.cpp. Results are compared against an independent sequential        *
 * reference, so the parallel answer must match exactly.                      *
 *                                                                           *
 * You don't need to read past this point unless you're curious!             *
 * ========================================================================= */

#include <cctype>
#include <exception>
#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace tests {

inline int failures = 0;

inline void check(bool cond, const std::string& msg) {
  if (cond) {
    std::cout << "  PASS: " << msg << "\n";
  } else {
    std::cout << "  FAIL: " << msg << "\n";
    ++failures;
  }
}

// Independent reference: a dead-simple sequential count used only to
// validate your answers. (Ordered map so failures print readably.)
inline std::map<char, std::size_t>
reference_counts(const std::vector<std::string_view>& texts) {
  std::map<char, std::size_t> ref;
  for (std::string_view t : texts)
    for (unsigned char c : t)
      if (std::isalpha(c)) ref[static_cast<char>(std::tolower(c))] += 1;
  return ref;
}

template <typename Map>
inline bool matches_reference(const Map& got,
                              const std::vector<std::string_view>& texts) {
  auto ref = reference_counts(texts);
  if (got.size() != ref.size()) return false;
  for (const auto& [c, n] : ref) {
    auto it = got.find(c);
    if (it == got.end() || it->second != n) return false;
  }
  return true;
}

inline const std::vector<std::string> corpus_storage = {
    "The quick brown Fox",
    "jumps over THE lazy dog!",
    "",
    "12345 -- symbols, and numbers are ignored.",
    "Aa Bb Cc aA bB cC",
};

inline std::vector<std::string_view> corpus() {
  std::vector<std::string_view> v;
  for (const auto& s : corpus_storage) v.emplace_back(s);
  return v;
}

inline void test_count_letters() {
  auto f = count_letters("Hello, World!");
  check(f['l'] == 3, "'Hello, World!' has three 'l'");
  check(f['o'] == 2, "'Hello, World!' has two 'o'");
  check(f['h'] == 1, "case-insensitive: 'H' counted as 'h'");
  check(f.find(',') == f.end(), "punctuation is ignored");
  check(f.find(' ') == f.end(), "whitespace is ignored");
  check(count_letters("").empty(), "empty text -> empty map");
  check(count_letters("123 !!!").empty(), "no letters -> empty map");
}

inline void test_frequency_seq() {
  check(frequency_seq({}).empty(), "no texts -> empty map");

  auto single = frequency_seq({std::string_view{"aabbbc"}});
  check(single['a'] == 2 && single['b'] == 3 && single['c'] == 1,
        "single text counted correctly");

  auto c = corpus();
  check(matches_reference(frequency_seq(c), c),
        "mixed corpus matches sequential reference");
}

inline void test_frequency() {
  check(frequency({}).empty(), "no texts -> empty map");

  auto single = frequency({std::string_view{"aabbbc"}});
  check(single['a'] == 2 && single['b'] == 3 && single['c'] == 1,
        "single text counted correctly");

  auto c = corpus();
  check(matches_reference(frequency(c), c),
        "mixed corpus matches sequential reference");
}

inline void test_frequency_async() {
  auto c = corpus();
  check(matches_reference(frequency_async(c), c),
        "async result matches sequential reference");

  auto single = frequency_async({std::string_view{"zzzz"}});
  check(single['z'] == 4, "async single text counted correctly");
}

inline void test_stress_no_lost_updates() {
  // Many identical texts: a data race or lost partial would show up as a
  // count that isn't an exact multiple. Repeat to flush nondeterminism.
  const std::string base = "abcabcabc"; // 3 each of a,b,c
  const std::size_t copies = 500;
  std::vector<std::string_view> many(copies, std::string_view{base});

  bool ok = true;
  for (int iter = 0; iter < 20 && ok; ++iter) {
    auto f = frequency(many);
    auto fa = frequency_async(many);
    if (!(f['a'] == 3 * copies && f['b'] == 3 * copies &&
          f['c'] == 3 * copies))
      ok = false;
    if (!matches_reference(fa, many)) ok = false;
  }
  check(ok, "no lost updates across 500 texts x 20 iterations (threads + async)");
}

using TestFn = void (*)();
inline const std::map<std::string, TestFn> test_functions = {
    {"count_letters", test_count_letters},
    {"frequency_seq", test_frequency_seq},
    {"frequency", test_frequency},
    {"frequency_async", test_frequency_async},
    {"stress", test_stress_no_lost_updates},
};

inline int run_one(const std::string& name) {
  auto it = test_functions.find(name);
  if (it == test_functions.end()) {
    std::cerr << "Test '" << name << "' not found.\nAvailable tests:\n";
    for (const auto& [n, _] : test_functions) std::cerr << "  " << n << "\n";
    return 1;
  }
  std::cout << "===== " << name << " =====\n";
  try {
    it->second();
  } catch (const std::exception& e) {
    std::cout << "  ERROR: " << e.what() << "\n";
    ++failures;
  }
  return 0;
}

inline int run_all() {
  failures = 0;
  for (const auto& [name, _] : test_functions) run_one(name);
  std::cout << "\n"
            << (failures == 0 ? "All checks passed.\n"
                              : std::to_string(failures) + " check(s) failed.\n");
  return failures == 0 ? 0 : 1;
}

} // namespace tests

#include "utils_timer.h"

int main(int argc, char* argv[]) {
  std::string arg = argc >= 2 ? argv[1] : "--test";

  if (arg == "--demo") {
    std::vector<std::string> texts = {"The quick brown fox",
                                      "jumps over the lazy dog"};
    std::vector<std::string_view> views(texts.begin(), texts.end());
    std::cout << "Texts:\n";
    for (const auto& t : texts) std::cout << "  \"" << t << "\"\n";
    std::cout << "Combined letter frequency (parallel):\n";
    auto f = frequency(views);
    std::map<char, std::size_t> sorted(f.begin(), f.end());
    for (const auto& [c, n] : sorted) std::cout << "  " << c << ": " << n << "\n";
    return 0;
  }

  if (arg == "--test") {
    if (argc >= 3) {
      tests::failures = 0;
      tests::run_one(argv[2]);
      return tests::failures == 0 ? 0 : 1;
    }
    return tests::run_all();
  }

  if (arg == "--time") {
    return tests::run_timing_demo();
  }

  std::cout << "Usage:\n"
            << "  " << argv[0] << " --test            # run all tests\n"
            << "  " << argv[0] << " --test <name>     # run one named test\n"
            << "  " << argv[0] << " --demo            # run a small demo\n"
            << "  " << argv[0] << " --time            # benchmark all versions\n";
  return 1;
}
