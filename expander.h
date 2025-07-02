#ifndef EXPANDER_H
#define EXPANDER_H

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "absl/container/flat_hash_set.h"

namespace fs = std::filesystem;

namespace mallard {
class Expander {
  std::vector<fs::path> search_paths_;

  std::vector<std::string> lines_;
  absl::flat_hash_set<fs::path> expanded_paths_;

  std::optional<fs::path> ResolvePath(std::string_view file_name);

  static bool ShouldIgnore(std::string_view file_name);
  static bool ShouldPreserve(std::string_view file_name);
  static std::string LineDirective(std::string_view file_path, std::size_t line_no);

  bool HandleInclude(std::string_view line);

  void DoExpand(fs::path file_path);

public:
  std::string Expand(fs::path file_path, std::vector<fs::path> search_paths);
};
} // namespace mallard

#endif // EXPANDER_H
