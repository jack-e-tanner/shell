#include "executor.h"

#include <unistd.h>

#include <filesystem>

#include "utils.h"
namespace fs = std::filesystem;

std::string searchExecutable(const std::string& name) {
  const char* path = getenv("PATH");

  if (path != nullptr) {
    std::vector<std::string> paths = split(path, ':');

    for (const auto& p : paths) {
      std::string executable = p + "/" + name;

      if (fs::exists(executable) && access(executable.c_str(), X_OK) == 0) {
        return executable;
      }
    }
  }

  return "";
}