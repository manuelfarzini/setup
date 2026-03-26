#include <cstdio>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

enum : uint8_t {
    Code,
    Comment,
    Directive,
    Blank,
    Brace,
};

using array_t = size_t[5];

inline constexpr char tab[] = "  ";

inline std::string_view trim(std::string_view sv) {
    while (!sv.empty() && std::isblank(sv.back()))
        sv.remove_suffix(1);
    while (!sv.empty() && std::isblank(sv.front()))
        sv.remove_prefix(1);
    return sv;
}

inline void count_file(std::ifstream & file, array_t & count) {
    bool in_comment = false;
    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty()) {
            count[Blank]++;
        } else if (line == "{" || line == "}") {
            count[Brace]++;
        } else if (line.starts_with("/*")) {
            in_comment = true;
            count[Comment]++;
            if (line.find("*/") != std::string::npos) {
                in_comment = false;
            }
        } else if (line == "*/") {
            count[Comment]++;
            in_comment = false;
        } else if (in_comment || line.starts_with("//")) {
            count[Comment]++;
        } else if (
            line[0] == '#' || line[0] == '@' || line.starts_with("import")
            || line.starts_with("using") || line.starts_with("namespace")
            || line.starts_with("package")) {
            count[Directive]++;
        } else {
            count[Code]++;
        }
    }
}

int main(int argc, char ** argv) {
    if (argc != 2) {
        printf("Usage: %s <file>", argv[0]);
        return 1;
    }
    if (!std::filesystem::exists(argv[1])) {
        std::printf("File %s does not exist", argv[1]);
        return 1;
    }

    if (std::filesystem::is_regular_file(argv[1])) {
        std::ifstream file(argv[1]);
        size_t count[] = {0, 0, 0, 0, 0};
        count_file(file, count);
        file.close();
        std::ostringstream obuff;
        obuff
            << "Files: " << argv[1] << "\n"
            << "Lines of code: " << count[Code] << "\n"
            << "Lines of comment: " << count[Comment] << "\n"
            << "Lines of directive: " << count[Directive] << "\n"
            << "Lines of blank: " << count[Blank] << "\n"
            << "Lines of brace: " << count[Brace];
        std::printf("%s", obuff.str().data());
        return 0;
    }

    std::ostringstream obuff;
    obuff << tab << "Files in directory:\n";
    int file_count = 0;
    size_t count[] = {0, 0, 0, 0, 0};
    for (const auto & entry : std::filesystem::recursive_directory_iterator(argv[1])) {
        if (entry.is_regular_file()) {
            obuff << tab << tab << "- " << entry.path().filename().string() << "\n";
            std::ifstream file(entry.path());
            count_file(file, count);
            file_count++;
            file.close();
        }
    }
    obuff
        << tab << "Total files: " << file_count << "\n"
        << tab << "Lines of code: " << count[Code] << "\n"
        << tab << "Lines of comment: " << count[Comment] << "\n"
        << tab << "Lines of directive: " << count[Directive] << "\n"
        << tab << "Lines of blank: " << count[Blank] << "\n"
        << tab << "Lines of brace: " << count[Brace];
    std::printf("%s\n", obuff.str().data());
}
