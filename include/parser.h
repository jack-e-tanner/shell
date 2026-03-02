#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <string>
#include <vector>

#include "ast.h"

// want encapsulation
class Parser {
 public:
  // explicit preevents silent casting
  explicit Parser(const std::vector<std::string>& tokens);
  std::unique_ptr<ASTNode> parse();

 private:
  const std::vector<std::string>& tokens_;  // _ means belongs to global scope
  size_t current_;

  // grammar rules
  std::unique_ptr<ASTNode> parse_list();
  std::unique_ptr<ASTNode> parse_pipeline();
  std::unique_ptr<ASTNode> parse_command();

  // utility functions
  const std::string& peek() const;
  const std::string& advance();
  const std::string& previous() const;
  bool match(const std::string& expected);
  bool is_at_end() const; 
};

#endif  // PARSER