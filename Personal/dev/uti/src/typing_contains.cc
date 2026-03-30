#include <fstream>
#include <iostream>
#include <print>
#include <unordered_set>
#include <vector>

using namespace std;
using istring = istringstream;

void write_words(auto const& words)
{
    for (const auto& w : words) {
        print(stdout, "{} ", w);
    }
    println("");
}

bool word_is_acceptable(string_view word, string_view allow)
{
    if (word.length() < 4) {
        return false;
    }
    bool ok = false;
    for (auto const& c : word) {
        // print("{} ", c);
        for (size_t j = 0; j < allow.size(); j++) {
            if (allow[j] == c) {
                ok = true;
                break;
            }
        }
        if (!ok) {
            return false;
        }
        ok = false;
    }
    return true;
}

int main(int argc, char const** argv)
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc != 2) {
        println("Usage: ./typing <allowed_characters> ");
        println("Note: the argument must be a contiguous string");
        return 1;
    }

    ifstream file("/Users/manuelfarzini/Personal/dev/doc/typing/typing.input");
    if (!file) {
        println("Can't open file");
        return 1;
    }

    string_view allow = {argv[1], strlen(argv[1])};
    std::vector<string> wordset;
    wordset.reserve(200);
    string word;

    while (file >> word) {
        transform(word.begin(), word.end(), word.begin(), [](unsigned char&& c) {
            return tolower(c);
        });

        if (word_is_acceptable(word, argv[1])) {
            wordset.push_back(std::move(word));
        }
    }

    write_words(wordset);
}
