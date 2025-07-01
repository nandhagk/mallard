#include <filesystem>
#include <fstream>
#include <string_view>

#include "absl/container/flat_hash_set.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/log/check.h"
#include "absl/log/globals.h"
#include "absl/log/initialize.h"
#include "absl/log/log.h"
#include "absl/strings/match.h"
#include "absl/strings/str_join.h"
#include "absl/strings/strip.h"

ABSL_FLAG(std::string, filename, "", "");
ABSL_FLAG(std::vector<std::string>, paths, {},
          "comma-separated list of directories to search through for includes");

namespace mallard {

static constexpr std::string_view kEndIfString = "#endif";
static constexpr std::string_view kIfNDefString = "#ifndef";
static constexpr std::string_view kDefineString = "#define";

static constexpr std::string_view kIncludeString = "#include";

static const absl::flat_hash_set<std::string_view> kSystemHeaders{
    "bits/stdc++.h", "ext/pb_ds/assoc_container.hpp", "ext/pb_ds/tree_policy.hpp",
    "tr2/dynamic_bitset",
    // C++ headers
    "algorithm", "any", "array", "atomic", "barrier", "bit", "bitset", "cassert",
    "cctype", "cerrno", "cfenv", "cfloat", "charconv", "chrono", "cinttypes", "climits",
    "clocale", "cmath", "codecvt", "compare", "complex", "concepts",
    "condition_variable", "coroutine", "csetjmp", "csignal", "cstdarg", "cstddef",
    "cstdint", "cstdio", "cstdlib", "cstring", "ctime", "cuchar", "cwchar", "cwctype",
    "deque", "exception", "execution", "expected", "filesystem", "flat_map", "flat_set",
    "format", "forward_list", "fstream", "functional", "future", "generator",
    "initializer_list", "iomanip", "ios", "iosfwd", "iostream", "istream", "iterator",
    "latch", "limits", "list", "locale", "map", "mdspan", "memory", "memory_resource",
    "mutex", "new", "numbers", "numeric", "optional", "ostream", "print", "queue",
    "random", "ranges", "ratio", "regex", "scoped_allocator", "semaphore", "set",
    "shared_mutex", "source_location", "span", "spanstream", "sstream", "stack",
    "stacktrace", "stdexcept", "stdfloat", "stop_token", "streambuf", "string",
    "string_view", "strstream", "syncstream", "system_error", "thread", "tuple",
    "type_traits", "typeindex", "typeinfo", "unordered_map", "unordered_set", "utility",
    "valarray", "variant", "vector", "version",
    // C headers
    "assert.h", "complex.h", "ctype.h", "errno.h", "fenv.h", "float.h", "inttypes.h",
    "iso646.h", "limits.h", "locale.h", "math.h", "setjmp.h", "signal.h", "stdalign.h",
    "stdarg.h", "stdatomic.h", "stdbit.h", "stdbool.h", "stdckdint.h", "stddef.h",
    "stdint.h", "stdio.h", "stdlib.h", "stdnoreturn.h", "string.h", "tgmath.h",
    "threads.h", "time.h", "uchar.h", "wchar.h", "wctype.h"};

class Expander {
  std::vector<std::filesystem::path> search_paths_;

  std::vector<std::string> lines_;
  absl::flat_hash_set<std::string> expanded_files_;

  std::filesystem::path FindFilePath(const std::string_view filename) {
    if (std::filesystem::exists(filename)) { return filename; }

    for (const std::filesystem::path &search_path : search_paths_) {
      if (std::filesystem::path file_path = search_path / filename; exists(file_path)) {
        return file_path;
      }
    }

    LOG(FATAL) << "Filename: " << filename << " not found!";
  }

  void HandleInclude(const std::string_view line) {
    std::string_view include = line.substr(kIncludeString.length());

    include = absl::StripLeadingAsciiWhitespace(include);
    QCHECK(include.length() > 2) << "Malformed include statement!";

    const bool wrapped_in_quotes =
        absl::StartsWith(include, "\"") && absl::EndsWith(include, "\"");
    const bool wrapped_in_angles =
        absl::StartsWith(include, "<") && absl::EndsWith(include, ">");

    QCHECK(wrapped_in_quotes || wrapped_in_angles) << "Malformed include statement!";

    if (const std::string_view filename = include.substr(1, include.length() - 2);
        !kSystemHeaders.contains(filename)) {
      DoExpand(filename);
    }
  }

  void DoExpand(std::string_view filename) {
    if (!expanded_files_.emplace(filename).second) {
      LOG(INFO) << "Skipping: " << filename;
      return;
    }

    LOG(INFO) << "Expanding: " << filename;

    std::ifstream file(FindFilePath(filename));
    for (std::string raw_line; std::getline(file, raw_line);) {
      std::string_view line(raw_line);
      line = absl::StripAsciiWhitespace(line);

      if (absl::StartsWith(line, kEndIfString) ||
          absl::StartsWith(line, kIfNDefString) ||
          absl::StartsWith(line, kDefineString)) {
        continue;
      }

      if (absl::StartsWith(line, kIncludeString)) {
        HandleInclude(line);
      } else {
        lines_.emplace_back(std::move(raw_line));
      }
    }
  }

public:
  std::string Expand(const std::string_view filename,
                     std::vector<std::filesystem::path> search_paths) {
    search_paths_ = std::move(search_paths);

    lines_.clear();
    expanded_files_.clear();

    DoExpand(filename);
    return absl::StrJoin(lines_, "\n");
  }
};

} // namespace mallard

int main(const int argc, char *argv[]) {
  absl::SetStderrThreshold(absl::LogSeverityAtLeast::kError);
  absl::InitializeLog();

  absl::ParseCommandLine(argc, argv);

  const std::string filename = absl::GetFlag(FLAGS_filename);
  QCHECK(!filename.empty()) << "Filename must not be empty!";

  std::vector<std::string> raw_paths = absl::GetFlag(FLAGS_paths);
  std::vector<std::filesystem::path> paths(raw_paths.begin(), raw_paths.end());

  mallard::Expander expander;
  std::cout << expander.Expand(filename, std::move(paths));

  return 0;
}