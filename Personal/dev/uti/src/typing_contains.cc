#include <fstream>
#include <iostream>
#include <print>
#include <set>

using namespace std;
using vstring = string_view;
using istring = istringstream;

void write_words(const auto& words) {
  for (const auto& w : words)
    print("{} ", w);
  println("");
}

bool word_is_acceptable(vstring const& word, char const* s) {
  if (word.length() < 4)
    return false;
  bool allowed = false;
  for (auto const& c : word) {
    for (size_t j = 0; j < strlen(s); j++) {
      if (s[j] == c) {
        allowed = true;
        break;
      }
    }
    if (!allowed) {
      return false;
    }
    allowed = false;
  }
  return true;
}

int main(int argc, char const** argv) {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  if (argc != 2) {
    println("Usage: ./typing <allowed_characters> ");
    println("Note: the argument must be a contiguous string");
    return 1;
  }

  ifstream file("/Users/manuelfarzini/Personal/docs/typing.input");
  if (!file) {
    println("Can't open file");
    return 1;
  }

  set<string> wordset;
  string word;
  while (file >> word) {
    transform(word.begin(), word.end(), word.begin(),
              [](unsigned char&& c) { return tolower(c); });
    if (word_is_acceptable(word, argv[1])) {
      wordset.insert(std::move(word));
    }
  }

  write_words(wordset);
}

// string const s = "abcdefghijklmnopqrstuvwxyz";
