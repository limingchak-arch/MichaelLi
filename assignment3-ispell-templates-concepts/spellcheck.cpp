#include "spellcheck.h"

#include <algorithm>
#include <concepts>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ranges>
#include <set>
#include <vector>

template <std::input_iterator Iterator,
          std::indirect_unary_predicate<Iterator> UnaryPred>
std::vector<Iterator> find_all(Iterator begin, Iterator end, UnaryPred pred);

namespace rv = std::ranges::views;

Corpus tokenize(std::string& source) {
  auto its = find_all(source.begin(), source.end(), ::isspace);
  Corpus tokens;
  std::transform(its.begin(), its.end() - 1, its.begin() + 1,
                 std::inserter(tokens, tokens.end()),
                 [&source](auto it1, auto it2) { return Token{source, it1, it2}; });
  std::erase_if(tokens, [](const Token& t) { return t.content.empty(); });
  return tokens;
}

std::set<Misspelling> spellcheck(const Corpus& source, const Dictionary& dictionary) {
  auto view = source
      | rv::filter([&dictionary](const Token& t) { return !dictionary.contains(t.content); })
      | rv::transform([&dictionary](const Token& t) {
          auto sugg_view = dictionary | rv::filter([&t](const std::string& word) {
                             return levenshtein(t.content, word) == 1;
                           });
          std::set<std::string> suggestions(sugg_view.begin(), sugg_view.end());
          return Misspelling{t, suggestions};
        })
      | rv::filter([](const Misspelling& m) { return !m.suggestions.empty(); });
  return std::set<Misspelling>(view.begin(), view.end());
};

/* Helper methods */

#include "utils.cpp"