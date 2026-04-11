// clang-format off

#include <fstream>
#include <iostream>
#include <print>
#include <sstream>
#include <string_view>
#include <unordered_map>

using std::println;
using std::string;
using std::unordered_map;
using vstring = std::string_view;
using istring = std::istringstream;

void print_table(const auto & table) {
  for (const auto & w : table)
    print("{} ", w.first, w.second);
  println("");
}

bool word_is_acceptable(vstring const & word, char const ** argv) {
  if (word.length() < 4)
    return false;
  for (auto const & c : word)
    for (size_t t = 0; t < std::strlen(argv[2]); t++)
      if (c == argv[2][t])
        return false;
  size_t count = 0;
  for (auto const & c : word) {
    if (c == argv[1][count])
      count++;
    if (count == std::strlen(argv[1]))
      return true;
  }
  return false;
}

int main(int argc, char const ** argv) {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  if (argc != 3) {
    println("Usage: ./typing <characters_must_have> <characters_must_filter_out>");
    println("Note: the arguments must be two contiguous strings");
    return 1;
  }

  std::ifstream file("/Users/manuelfarzini/Personal/docs/typing.input");
  if (!file) {
    println("Can't open file");
    return 1;
  }

  unordered_map<string, int> table;
  table.reserve(1024);
  string word;

  while (file >> word) {
    std::transform(word.begin(), word.end(), word.begin(),
                   [](unsigned char && c) { return std::tolower(c); });
    if (!word_is_acceptable(word, argv)) continue;
    table.try_emplace(std::move(word), 1).first->second++;
  }

  print_table(table);
}
