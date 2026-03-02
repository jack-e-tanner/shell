#include <cstring>
#include <string>
#include <vector>
#include <unistd.h>
#include <unordered_map>
#include <filesystem>
namespace fs = std::filesystem;

#include <readline/readline.h>
#include "completion.h"
#include "builtins.h"
#include "utils.h"

static std::vector<std::string> sorted_cache;

static std::unordered_map<std::string, std::string> getexecnames() {
  std::unordered_map<std::string, std::string> mapcache;
  const char* path = getenv("PATH");

  if (path != nullptr) {
    std::vector<std::string> paths = split(path, ':');

    for (const auto& p : paths) {
      if (!fs::exists(p) || !fs::is_directory(p)) {
        continue;
      }

      for (const auto& entry : fs::directory_iterator(p)) {
        if (entry.is_regular_file() && access(entry.path().c_str(), X_OK) == 0) {
          if (mapcache.contains(entry.path().filename())) {
            continue;
          }
          mapcache[entry.path().filename()] = entry.path();
        }
      }
    }
  }

  return mapcache;
}

static char* command_generator(const char* text, int state) {
  static size_t index;
  static size_t len;

  // starting a new completion
  if (!state) {
    len = strlen(text);

    // binary search baby
    auto it = std::lower_bound(sorted_cache.begin(), sorted_cache.end(), std::string(text));
    index = it - sorted_cache.begin();
  }

  if (index < sorted_cache.size() && sorted_cache[index].compare(0, len, text) == 0) {
    return strdup(sorted_cache[index++].c_str());
  }

  return nullptr;
}

static char** command_completion(const char* text, int start, int /*end*/) {
  rl_attempted_completion_over = 1;

  if (start == 0) {
    return rl_completion_matches(text, command_generator);
  }

  return nullptr;
}

void init_completion() {
  auto execs = getexecnames();
  auto builtins = get_builtin_names();

  // pre-allocate the cache for better performance
  sorted_cache.reserve(execs.size() + builtins.size());

  for (const auto& [name, path] : execs) {
    sorted_cache.push_back(name);
  }

  for (const auto& name : builtins) {
    sorted_cache.push_back(name);
  }

  // sort the cache for binary search
  std::sort(sorted_cache.begin(), sorted_cache.end());
  /* worth it?
    sorted_cache.erase(std::unique(sorted_cache.begin(), 
    sorted_cache.end()), sorted_cache.end());
  */

  // global func pointer for readline to call when it needs to do completion
  rl_attempted_completion_function = command_completion;
}
