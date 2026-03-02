#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <string>

// std:: because we don't want to pollute the global namespace in the header
// file
std::string searchExecutable(const std::string& name);

#endif  // EXECUTOR_H