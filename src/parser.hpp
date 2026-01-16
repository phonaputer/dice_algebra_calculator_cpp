#include "lexer.hpp"
#include <memory>
#include <optional>

struct TreeExecutionResult
{
  long result;
  std::string description;
};

class Tree
{
public:
  virtual ~Tree() {}
  virtual TreeExecutionResult execute() = 0;
};

std::unique_ptr<Tree> parse(std::vector<Token> tokens);
