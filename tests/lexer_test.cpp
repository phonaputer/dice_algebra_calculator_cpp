#include "dice_exception.hpp"
#include "lexer.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

MATCHER(TokenEq, "Compares two Token structs for equality")
{
  const Token &lhs = std::get<0>(arg);
  const Token &rhs = std::get<1>(arg);

  return lhs.tokenType == rhs.tokenType && lhs.integerValue == rhs.integerValue;
}

TEST(Lexer, tokenize_InputIsEmptyString_ReturnsEmptyTokenVector)
{
  auto result = tokenize("");

  EXPECT_TRUE(result.empty());
}

TEST(Lexer, tokenize_InputIsOnlyWhitespace_ReturnsEmptyTokenVector)
{
  auto result = tokenize(" \n\t");

  EXPECT_TRUE(result.empty());
}

TEST(Lexer, tokenize_InputContainsOnlyValidTokens_ReturnsVectorOfTheseTokens)
{
  auto result = tokenize("1dD100+-*/()hHlL");

  std::vector<Token> expected = {
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
      Token{.tokenType = TokenType::D, .integerValue = 0},
      Token{.tokenType = TokenType::D, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 100},
      Token{.tokenType = TokenType::Add, .integerValue = 0},
      Token{.tokenType = TokenType::Subtract, .integerValue = 0},
      Token{.tokenType = TokenType::Multiply, .integerValue = 0},
      Token{.tokenType = TokenType::Divide, .integerValue = 0},
      Token{.tokenType = TokenType::OpenParenthesis, .integerValue = 0},
      Token{.tokenType = TokenType::CloseParenthesis, .integerValue = 0},
      Token{.tokenType = TokenType::H, .integerValue = 0},
      Token{.tokenType = TokenType::H, .integerValue = 0},
      Token{.tokenType = TokenType::L, .integerValue = 0},
      Token{.tokenType = TokenType::L, .integerValue = 0},
  };
  EXPECT_THAT(result, testing::Pointwise(TokenEq(), expected));
}

TEST(
    Lexer,
    tokenize_InputContainsValidTokensSeparatedByWhitespace_ReturnsVectorOfTheseTokensExcludingWhitespace
)
{
  auto result = tokenize(
      "1\t\n d\t\n D\t\n 100\t\n +\t\n -\t\n *\t\n /\t\n (\t\n )\t\n h\t\n "
      "H\t\n l\t\n L\t\n "
  );

  std::vector<Token> expected = {
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
      Token{.tokenType = TokenType::D, .integerValue = 0},
      Token{.tokenType = TokenType::D, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 100},
      Token{.tokenType = TokenType::Add, .integerValue = 0},
      Token{.tokenType = TokenType::Subtract, .integerValue = 0},
      Token{.tokenType = TokenType::Multiply, .integerValue = 0},
      Token{.tokenType = TokenType::Divide, .integerValue = 0},
      Token{.tokenType = TokenType::OpenParenthesis, .integerValue = 0},
      Token{.tokenType = TokenType::CloseParenthesis, .integerValue = 0},
      Token{.tokenType = TokenType::H, .integerValue = 0},
      Token{.tokenType = TokenType::H, .integerValue = 0},
      Token{.tokenType = TokenType::L, .integerValue = 0},
      Token{.tokenType = TokenType::L, .integerValue = 0},
  };
  EXPECT_THAT(result, testing::Pointwise(TokenEq(), expected));
}

TEST(Lexer, tokenize_InputContainsInvalidToken_ThrowsDiceException)
{
  // Would be nice to have a table driven test with many characters, but this
  // is fine for now.
  try
  {
    auto result = tokenize("s");
  }
  catch (DiceException e)
  {
    EXPECT_STREQ("Unexpected character in input: 's'", e.what());
    return;
  }

  FAIL();
}
