#include "lexer.hpp"
#include <format>
#include <stdexcept>

struct TokenTypeResult
{
  bool matchesATokenType;
  TokenType tokenType;
};

TokenTypeResult determineTokenType(char c)
{
  switch (c)
  {
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    return TokenTypeResult{true, TokenType::Integer};
  case 'd':
  case 'D':
    return TokenTypeResult{true, TokenType::D};
  case 'l':
  case 'L':
    return TokenTypeResult{true, TokenType::L};
  case 'h':
  case 'H':
    return TokenTypeResult{true, TokenType::H};
  case '+':
    return TokenTypeResult{true, TokenType::Add};
  case '-':
    return TokenTypeResult{true, TokenType::Subtract};
  case '/':
    return TokenTypeResult{true, TokenType::Divide};
  case '*':
    return TokenTypeResult{true, TokenType::Multiply};
  case '(':
    return TokenTypeResult{true, TokenType::OpenParenthesis};
  case ')':
    return TokenTypeResult{true, TokenType::CloseParenthesis};
  case ' ':
  case '\t':
  case '\n':
    return TokenTypeResult{false};
  default:;
    std::string msg = std::format("Unexpected character in input: '{}'", c);
    throw std::invalid_argument(msg);
  }
}

std::vector<Token> tokenize(std::string input)
{
  std::string currentInt = "";
  std::vector<Token> results;

  for (char c : input)
  {
    auto tokenTypeResult = determineTokenType(c);
    if (!tokenTypeResult.matchesATokenType)
    {
      continue;
    }

    if (tokenTypeResult.tokenType == TokenType::Integer)
    {
      currentInt += c;
      continue;
    }

    unsigned long integer = std::stoul(currentInt); // TODO handle exceptions
    currentInt = "";

    results.push_back(
        Token{
            .tokenType = TokenType::Integer,
            .integerValue = integer,
        }
    );
    results.push_back(
        Token{
            .tokenType = tokenTypeResult.tokenType,
        }
    );
  }

  if (currentInt.length() > 0)
  {
    results.push_back(
        Token{
            .tokenType = TokenType::Integer,
            .integerValue = std::stoul(currentInt), // TODO handle exceptions,
        }
    );
  }

  return results;
}
