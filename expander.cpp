#include "expander.h"

#include <fstream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "absl/log/check.h"
#include "absl/log/globals.h"
#include "absl/log/log.h"
#include "absl/strings/match.h"
#include "absl/strings/str_join.h"
#include "absl/strings/strip.h"

namespace mallard {
static constexpr std::string_view kIncludeString = "#include";

static const absl::flat_hash_set<std::string_view> kExtendedHeaders{
    "bits/stdc++.h",
    "ext/pb_ds/assoc_container.hpp",
    "ext/pb_ds/tree_policy.hpp",
    "tr2/dynamic_bitset",
};

static const absl::flat_hash_set<std::string_view> kSystemHeaders{
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

std::optional<fs::path> Expander::ResolvePath(const std::string_view file_name) {
  if (fs::exists(file_name)) { return file_name; }

  for (const fs::path &search_path : search_paths_) {
    if (fs::path file_path = search_path / file_name; fs::exists(file_path)) {
      return file_path;
    }
  }

  return std::nullopt;
}

[[gnu::always_inline, nodiscard]] bool
Expander::ShouldIgnore(const std::string_view file_name) {
  return kSystemHeaders.contains(file_name);
}

[[gnu::always_inline, nodiscard]] bool
Expander::ShouldPreserve(const std::string_view file_name) {
  return kExtendedHeaders.contains(file_name);
}

bool Expander::HandleInclude(const std::string_view line) {
  std::string_view include = line.substr(kIncludeString.length());

  include = absl::StripLeadingAsciiWhitespace(include);
  QCHECK(include.length() > 2) << "Malformed include statement!";

  const bool is_wrapped_in_quotes = include.starts_with('"') && include.ends_with('"');
  const bool is_wrapped_in_angles = include.starts_with('<') && include.ends_with('>');
  QCHECK(is_wrapped_in_quotes || is_wrapped_in_angles)
      << "Malformed include statement!";

  const std::string_view file_name = include.substr(1, include.length() - 2);
  if (ShouldIgnore(file_name)) { return false; }

  if (!ShouldPreserve(file_name)) {
    if (std::optional<fs::path> path = ResolvePath(file_name)) {
      DoExpand(std::move(*path));
      return true;
    }

    LOG(ERROR) << "Could not resolve: " << file_name;
  }

  lines_.emplace_back(line);
  return false;
}

[[gnu::always_inline, nodiscard]] std::string
Expander::LineDirective(const std::string_view file_path, const std::size_t line_no) {
  return absl::StrCat("#line ", line_no, " \"", file_path, "\"");
}

void Expander::DoExpand(fs::path file_path) {
  if (!expanded_paths_.emplace(file_path).second) {
    LOG(INFO) << "Skipping: " << file_path;
    return;
  }

  LOG(INFO) << "Expanding: " << file_path;
  lines_.push_back(LineDirective(file_path.native(), 1));

  std::ifstream file(file_path);
  std::string raw_line;
  for (std::size_t line_no = 1; std::getline(file, raw_line); ++line_no) {
    std::string_view line(raw_line);

    line = absl::StripAsciiWhitespace(line);
    if (!line.starts_with(kIncludeString)) {
      lines_.emplace_back(std::move(raw_line));
      continue;
    }

    if (HandleInclude(line)) {
      lines_.push_back(LineDirective(file_path.native(), line_no + 1));
    }
  }
}

std::string Expander::Expand(fs::path file_path, std::vector<fs::path> search_paths) {
  search_paths_ = std::move(search_paths);

  lines_.clear();
  expanded_paths_.clear();

  DoExpand(std::move(file_path));
  return absl::StrJoin(lines_, "\n");
}

} // namespace mallard