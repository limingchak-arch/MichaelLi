/* ========================================================================= *
 * Test harness for Assignment 6: Declarative Pipelines.                      *
 *                                                                           *
 * This file provides main(). It runs a small demo (./main --demo) and a     *
 * test mode (./main --test [name]) that checks each function/view you       *
 * implement in main.cpp.                                                     *
 *                                                                           *
 * You don't need to read past this point unless you're curious!             *
 * ========================================================================= */

#include <exception>
#include <forward_list>
#include <list>
#include <map>
#include <sstream>
#include <string>
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

inline std::string formatted(const auto& rng) {
  std::stringstream s;
  format_range(s, rng);
  return s.str();
}

inline std::string categorized(const auto& rng) {
  std::stringstream s;
  format_categorized(s, rng);
  return s.str();
}

inline void test_format_range() {
  std::vector<int> empty;
  check(formatted(empty) == "{}", "empty vector -> {}");

  std::list<int> empty_list;
  check(formatted(empty_list) == "{}", "empty list -> {}");

  std::vector<int> one{1};
  check(formatted(one) == "{1}", "single element -> {1}");

  std::vector<int> many{1, 2, 3};
  check(formatted(many) == "{1,2,3}", "vector{1,2,3} -> {1,2,3}");

  std::list<int> as_list{1, 2, 3};
  check(formatted(as_list) == "{1,2,3}", "list{1,2,3} -> {1,2,3} (comma for any range)");
}

inline void test_format_categorized() {
  std::vector<int> ra{1, 2, 3};
  check(categorized(ra) == "{1,2,3}", "random-access (vector) uses ','");

  std::list<int> bidi{1, 2, 3};
  check(categorized(bidi) == "{1<=>2<=>3}", "bidirectional (list) uses '<=>'");

  std::forward_list<int> fwd{1, 2, 3};
  check(categorized(fwd) == "{1->2->3}", "forward (forward_list) uses '->'");

  std::vector<int> empty;
  check(categorized(empty) == "{}", "empty random-access -> {}");
}

inline std::string trimmed(const std::string& in) {
  std::string out;
  for (char c : in | trim_whitespace) out.push_back(c);
  return out;
}

inline void test_trim_whitespace() {
  check(trimmed("") == "", "empty string trims to empty");
  check(trimmed("abc") == "abc", "no whitespace is unchanged");
  check(trimmed("  abc  ") == "abc", "leading + trailing spaces removed");
  check(trimmed("\t\ta b c") == "a b c", "leading tabs removed, inner spaces kept");
  check(trimmed("a b c\t") == "a b c", "trailing tab removed, inner spaces kept");
  check(trimmed("   ") == "", "all-whitespace trims to empty");
}

inline std::vector<int> digits_of(const std::string& s) {
  std::vector<int> v;
  for (char c : s) v.push_back(c - '0');
  return v;
}

inline void test_valid_card() {
  const std::vector<std::string> valid{
      "4111111111111111", // Visa
      "6011514433546201", // Discover
      "370267463651064",  // American Express
      "5425233430109903"  // Mastercard
  };
  bool all_valid = true;
  for (const auto& n : valid)
    if (!valid_card(digits_of(n))) all_valid = false;
  check(all_valid, "accepts known-valid card numbers");

  const std::vector<std::string> invalid{
      "1234567812345678", "0000000000000001", "1111111111111111",
      "9999999999999996"};
  bool all_invalid = true;
  for (const auto& n : invalid)
    if (valid_card(digits_of(n))) all_invalid = false;
  check(all_invalid, "rejects known-invalid card numbers");
}

using TestFn = void (*)();
inline const std::map<std::string, TestFn> test_functions = {
    {"format_range", test_format_range},
    {"format_categorized", test_format_categorized},
    {"trim_whitespace", test_trim_whitespace},
    {"valid_card", test_valid_card},
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

int main(int argc, char* argv[]) {
  std::string arg = argc >= 2 ? argv[1] : "--test";

  if (arg == "--demo") {
    std::vector<int> v{1, 2, 3};
    std::list<int> l{1, 2, 3};
    std::forward_list<int> f{1, 2, 3};
    std::cout << "format_range(vector{1,2,3})       = ";
    format_range(std::cout, v) << "\n";
    std::cout << "format_categorized(vector{1,2,3}) = ";
    format_categorized(std::cout, v) << "\n";
    std::cout << "format_categorized(list{1,2,3})   = ";
    format_categorized(std::cout, l) << "\n";
    std::cout << "format_categorized(flist{1,2,3})  = ";
    format_categorized(std::cout, f) << "\n";

    std::string messy = "   trim me \t";
    std::cout << "trim_whitespace(\"   trim me \\t\") = \"";
    for (char c : messy | trim_whitespace) std::cout << c;
    std::cout << "\"\n";

    std::vector<int> card{4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    std::cout << "valid_card(4111111111111111)      = " << std::boolalpha
              << valid_card(card) << "\n";
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

  std::cout << "Usage:\n"
            << "  " << argv[0] << " --test            # run all tests\n"
            << "  " << argv[0] << " --test <name>     # run one named test\n"
            << "  " << argv[0] << " --demo            # run a small demo\n";
  return 1;
}
