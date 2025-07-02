#include <filesystem>
#include <fstream>
#include <string_view>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "absl/log/check.h"
#include "absl/log/globals.h"
#include "absl/log/initialize.h"

#include "expander.h"

ABSL_FLAG(std::string, file, "", "");
ABSL_FLAG(std::vector<std::string>, search_paths, {},
          "comma-separated list of directories, to search through for includes");
ABSL_FLAG(bool, debug, false, "include line directives");

int main(const int argc, char *argv[]) {
  absl::SetStderrThreshold(absl::LogSeverityAtLeast::kError);
  absl::InitializeLog();

  absl::SetProgramUsageMessage(
      "--file=<file> --search_paths=<path, ...> --debug=false");
  absl::ParseCommandLine(argc, argv);

  fs::path file_path = absl::GetFlag(FLAGS_file);
  QCHECK(fs::exists(file_path)) << "File must not be empty!";

  std::vector<std::string> raw_search_paths = absl::GetFlag(FLAGS_search_paths);
  std::vector<fs::path> search_paths(raw_search_paths.begin(), raw_search_paths.end());

  mallard::Expander expander;
  std::cout << expander.Expand(std::move(file_path), std::move(search_paths));

  return 0;
}