#include <filesystem>
#include <fstream>
#include <string_view>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "absl/log/check.h"
#include "absl/log/globals.h"
#include "absl/log/initialize.h"
#include "absl/log/log.h"

#include "expander.h"

ABSL_FLAG(std::vector<std::string>, search_paths, {},
          "comma-separated list of directories, to search through for includes");
ABSL_FLAG(bool, debug, false, "include line directives");

int main(const int argc, char *argv[]) {
  absl::SetStderrThreshold(absl::LogSeverityAtLeast::kError);

  absl::InitializeLog();
  absl::SetProgramUsageMessage("<file> --search_paths=<path, ...> --debug=false");

  const std::vector<char *> args = absl::ParseCommandLine(argc, argv);
  QCHECK(args.size() == 2) << "File must not be empty!";

  fs::path file_path = args[1];
  QCHECK(fs::exists(file_path)) << "File must exist!";

  std::vector<std::string> raw_search_paths = absl::GetFlag(FLAGS_search_paths);
  std::vector<fs::path> search_paths(raw_search_paths.begin(), raw_search_paths.end());

  mallard::Expander expander;
  std::cout << expander.Expand(std::move(file_path), std::move(search_paths));

  return 0;
}