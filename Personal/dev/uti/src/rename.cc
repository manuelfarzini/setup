#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>

namespace fs = std::filesystem;
namespace rng = std::ranges;
using namespace std;
using File = fs::directory_entry;
using Files = vector<File>;

#ifndef fm_inline
#if defined(_MSC_VER)
#define fm_inline __force_inline
#elif defined(__clang__)
#define fm_inline [[clang::always_inline]] __attribute__((always_inline, hot)) inline
#elif defined(__GNUC__)
#define fm_inline __attribute__((always_inline, hot)) inline
#else
#define fm_inline inline
#endif
#endif  // fm_inline

fm_inline bool natural_less(const string& a, const string& b) {
  size_t i = 0, j = 0;
  while (i < a.size() && j < b.size()) {
    if (!isdigit(a[i]) || !isdigit(b[j])) {
      if (a[i] != b[j])
        return a[i] < b[j];
      i++;
      j++;
    } else {
      size_t ni = i, nj = j;
      while (ni < a.size() && isdigit(a[ni])) {
        ni++;
      }
      while (nj < b.size() && isdigit(b[nj])) {
        nj++;
      }
      int num_a = stoi(a.substr(i, ni - i));
      int num_b = stoi(b.substr(j, nj - j));
      if (num_a != num_b) {
        return num_a < num_b;
      }
      i = ni;
      j = nj;
    }
  }
  return a.size() < b.size();
}

fm_inline string enumer(int j, int max_zeros) {
  ostringstream n;
  for (int i = 0; i < max_zeros - floor(log10(j)); i++)
    n << "0";
  n << j++;
  return n.str();
}

int main(int argc, char** argv) {
  if (argc != 3) {
    printf("Usage: rename <dirpath> <prefix>");
    return 1;
  }
  if (!fs::exists(argv[1])) {
    printf("Unable to find directory");
    return 1;
  }

  Files files;
  string dirpath(argv[1]);
  string prefix(argv[2]);

  for (auto& entry : fs::directory_iterator(argv[1])) {
    if (fs::is_regular_file(entry)) {
      files.push_back(entry);
    }
  }
  if (files.empty()) {
    return 1;
  }
  rng::sort(files, [](File& a, File& b) {
    return natural_less(a.path().filename().string(), b.path().filename().string());
  });

  int j = 1;
  int max_zeros = floor(log10(files.size()));
  for (auto& file : files) {
    fs::rename(file, string(argv[1]) + "/" + argv[2] + "_" + enumer(j++, max_zeros) +
                            file.path().extension().string());
  }
}
