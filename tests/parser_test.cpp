#include "dice_exception.hpp"
#include "parser.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

void parse_and_expect_dice_exception(
    std::vector<Token> input, const char *expectedErrMsg
)
{
  try
  {
    parse(input);
  }
  catch (DiceException e)
  {
    EXPECT_STREQ(expectedErrMsg, e.what());
    return;
  }

  FAIL() << "Expected DiceException.";
}

void execute_and_expect_dice_exception(
    std::unique_ptr<Tree> tree, const char *expectedErrMsg
)
{
  try
  {
    tree->execute();
  }
  catch (DiceException e)
  {
    EXPECT_STREQ(expectedErrMsg, e.what());
    return;
  }

  FAIL() << "Expected DiceException.";
}

TEST(Parser, parse_EmptyInput_ThrowsDiceException)
{
  std::vector<Token> emptyVector;

  parse_and_expect_dice_exception(emptyVector, "Empty input.");
}

TEST(Parser, parse_InvalidShortRollMissingInteger_ThrowsDiceException)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::D, .integerValue = 0},
  };

  parse_and_expect_dice_exception(input, "Input expression is not valid.");
}

TEST(Parser, parse_InvalidLongRollMissingInteger_ThrowsDiceException)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
      Token{.tokenType = TokenType::D, .integerValue = 0},
  };

  parse_and_expect_dice_exception(input, "Input expression is not valid.");
}

TEST(Parser, parse_InvalidLongRollWithHighMissingInteger_ThrowsDiceException)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
      Token{.tokenType = TokenType::D, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
      Token{.tokenType = TokenType::H, .integerValue = 1},
  };

  parse_and_expect_dice_exception(input, "Input expression is not valid.");
}

TEST(Parser, parse_InvalidLongRollWithLowMissingInteger_ThrowsDiceException)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
      Token{.tokenType = TokenType::D, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
      Token{.tokenType = TokenType::L, .integerValue = 1},
  };

  parse_and_expect_dice_exception(input, "Input expression is not valid.");
}

TEST(Parser, parse_AddWithoutOperands_ThrowsDiceException)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Add, .integerValue = 0},
  };

  parse_and_expect_dice_exception(input, "Input expression is not valid.");
}

TEST(Parser, parse_AddWithNoLeftOperand_ThrowsDiceException)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Add, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
  };

  parse_and_expect_dice_exception(input, "Input expression is not valid.");
}

TEST(Parser, parse_AddWithNoRightOperand_ThrowsDiceException)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
      Token{.tokenType = TokenType::Add, .integerValue = 0},
  };

  parse_and_expect_dice_exception(input, "Input expression is not valid.");
}

TEST(Parser, parse_SubtractWithoutOperands_ThrowsDiceException)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Subtract, .integerValue = 0},
  };

  parse_and_expect_dice_exception(input, "Input expression is not valid.");
}

TEST(Parser, parse_SubtractWithNoLeftOperand_ThrowsDiceException)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Subtract, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
  };

  parse_and_expect_dice_exception(input, "Input expression is not valid.");
}

TEST(Parser, parse_SubtractWithNoRightOperand_ThrowsDiceException)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
      Token{.tokenType = TokenType::Subtract, .integerValue = 0},
  };

  parse_and_expect_dice_exception(input, "Input expression is not valid.");
}

TEST(Parser, parse_MultiplyWithoutOperands_ThrowsDiceException)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Multiply, .integerValue = 0},
  };

  parse_and_expect_dice_exception(input, "Input expression is not valid.");
}

TEST(Parser, parse_MultiplyWithNoLeftOperand_ThrowsDiceException)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Multiply, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
  };

  parse_and_expect_dice_exception(input, "Input expression is not valid.");
}

TEST(Parser, parse_MultiplyWithNoRightOperand_ThrowsDiceException)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
      Token{.tokenType = TokenType::Multiply, .integerValue = 0},
  };

  parse_and_expect_dice_exception(input, "Input expression is not valid.");
}

TEST(Parser, parse_DivideWithoutOperands_ThrowsDiceException)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Divide, .integerValue = 0},
  };

  parse_and_expect_dice_exception(input, "Input expression is not valid.");
}

TEST(Parser, parse_DivideWithNoLeftOperand_ThrowsDiceException)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Divide, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
  };

  parse_and_expect_dice_exception(input, "Input expression is not valid.");
}

TEST(Parser, parse_DivideWithNoRightOperand_ThrowsDiceException)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
      Token{.tokenType = TokenType::Divide, .integerValue = 0},
  };

  parse_and_expect_dice_exception(input, "Input expression is not valid.");
}

TEST(Parser, parse_MultipleDTokensWithoutInteger_ThrowsDiceException)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::D, .integerValue = 0},
      Token{.tokenType = TokenType::D, .integerValue = 0},
  };

  parse_and_expect_dice_exception(input, "Input expression is not valid.");
}

TEST(Parser, parse_UnclosedOpenParen_ThrowsDiceException)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::OpenParenthesis, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
      Token{.tokenType = TokenType::CloseParenthesis, .integerValue = 0},
      Token{.tokenType = TokenType::OpenParenthesis, .integerValue = 0},
  };

  parse_and_expect_dice_exception(
      input, "Expression contains an unclosed parenthetical."
  );
}

TEST(Parser, parse_UnopenedCloseParen_ThrowsDiceException)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::OpenParenthesis, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
      Token{.tokenType = TokenType::CloseParenthesis, .integerValue = 0},
      Token{.tokenType = TokenType::CloseParenthesis, .integerValue = 0},
  };

  parse_and_expect_dice_exception(
      input, "Expression contains an unclosed parenthetical."
  );
}

TEST(Parser, parse_CloseBeforeOpenParen_ThrowsDiceException)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::CloseParenthesis, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
      Token{.tokenType = TokenType::OpenParenthesis, .integerValue = 0},
  };

  parse_and_expect_dice_exception(input, "Input expression is not valid.");
}

TEST(Parser, parse_DivisionByZero_ThrowsDiceException)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Integer, .integerValue = 10},
      Token{.tokenType = TokenType::Divide, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 0},
  };
  auto parseResult = parse(input);

  execute_and_expect_dice_exception(
      std::move(parseResult), "Division by zero is not allowed."
  );
}

// TODO stub the random generator to avoid all the "nD1" stuff in these execute
// tests

TEST(Parser, parse_ValidExpressionWithAllTheFixings_ReturnsCorrectResult)
{
  // 4d1l2 * (7 - 2) / (d1 + 2d1h1 + 1d1 + 2)
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Integer, .integerValue = 4},
      Token{.tokenType = TokenType::D, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
      Token{.tokenType = TokenType::L, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 2},
      Token{.tokenType = TokenType::Multiply, .integerValue = 0},
      Token{.tokenType = TokenType::OpenParenthesis, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 7},
      Token{.tokenType = TokenType::Subtract, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 2},
      Token{.tokenType = TokenType::CloseParenthesis, .integerValue = 0},
      Token{.tokenType = TokenType::Divide, .integerValue = 0},
      Token{.tokenType = TokenType::OpenParenthesis, .integerValue = 0},
      Token{.tokenType = TokenType::D, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
      Token{.tokenType = TokenType::Add, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 2},
      Token{.tokenType = TokenType::D, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
      Token{.tokenType = TokenType::H, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
      Token{.tokenType = TokenType::Add, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
      Token{.tokenType = TokenType::D, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
      Token{.tokenType = TokenType::Add, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 2},
      Token{.tokenType = TokenType::CloseParenthesis, .integerValue = 0},
  };

  auto parseResult = parse(input);
  auto executeResult = parseResult->execute();

  EXPECT_EQ(2, executeResult.result);
  std::string expectedDescription = "\nRolling 4d1..."
                                    "\nYou rolled: 1"
                                    "\nYou rolled: 1"
                                    "\nYou rolled: 1"
                                    "\nYou rolled: 1"
                                    "\n"
                                    "\nRolling d1..."
                                    "\nYou rolled: 1"
                                    "\n"
                                    "\nRolling 2d1..."
                                    "\nYou rolled: 1"
                                    "\nYou rolled: 1"
                                    "\n"
                                    "\nRolling 1d1..."
                                    "\nYou rolled: 1"
                                    "\n";
  EXPECT_EQ(expectedDescription, executeResult.description);
}

TEST(Parser, parse_SingleInteger_ReturnsCorrectResult)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Integer, .integerValue = 4},
  };

  auto parseResult = parse(input);
  auto executeResult = parseResult->execute();

  EXPECT_EQ(4, executeResult.result);
  EXPECT_EQ("", executeResult.description);
}

TEST(Parser, parse_ShortRoll_ReturnsCorrectResult)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::D, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
  };

  auto parseResult = parse(input);
  auto executeResult = parseResult->execute();

  EXPECT_EQ(1, executeResult.result);
  std::string expectedDescription = "\nRolling d1..."
                                    "\nYou rolled: 1"
                                    "\n";
  EXPECT_EQ(expectedDescription, executeResult.description);
}

TEST(Parser, parse_ShortRoll0Faces_Returns0)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::D, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 0},
  };

  auto parseResult = parse(input);
  auto executeResult = parseResult->execute();

  EXPECT_EQ(0, executeResult.result);
  std::string expectedDescription = "\nRolling d0...\nYou rolled: 0\n";
  EXPECT_EQ(expectedDescription, executeResult.description);
}

TEST(Parser, parse_LongRoll_ReturnsCorrectResult)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Integer, .integerValue = 4},
      Token{.tokenType = TokenType::D, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
  };

  auto parseResult = parse(input);
  auto executeResult = parseResult->execute();

  EXPECT_EQ(4, executeResult.result);
  std::string expectedDescription = "\nRolling 4d1..."
                                    "\nYou rolled: 1"
                                    "\nYou rolled: 1"
                                    "\nYou rolled: 1"
                                    "\nYou rolled: 1"
                                    "\n";
  EXPECT_EQ(expectedDescription, executeResult.description);
}

TEST(Parser, parse_LongRoll0Die_Returns0)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Integer, .integerValue = 0},
      Token{.tokenType = TokenType::D, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
  };

  auto parseResult = parse(input);
  auto executeResult = parseResult->execute();

  EXPECT_EQ(0, executeResult.result);
  std::string expectedDescription = "\nRolling 0d1...\nYou rolled: 0\n";
  EXPECT_EQ(expectedDescription, executeResult.description);
}

TEST(Parser, parse_LongRoll0Faces_Returns0)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Integer, .integerValue = 4},
      Token{.tokenType = TokenType::D, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 0},
  };

  auto parseResult = parse(input);
  auto executeResult = parseResult->execute();

  EXPECT_EQ(0, executeResult.result);
  std::string expectedDescription = "\nRolling 4d0...\nYou rolled: 0\n";
  EXPECT_EQ(expectedDescription, executeResult.description);
}

TEST(Parser, parse_LongRollWithHigh_ReturnsCorrectResult)
{
  // 4d1h2
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Integer, .integerValue = 4},
      Token{.tokenType = TokenType::D, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
      Token{.tokenType = TokenType::H, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 2},
  };

  auto parseResult = parse(input);
  auto executeResult = parseResult->execute();

  EXPECT_EQ(2, executeResult.result);
  std::string expectedDescription = "\nRolling 4d1..."
                                    "\nYou rolled: 1"
                                    "\nYou rolled: 1"
                                    "\nYou rolled: 1"
                                    "\nYou rolled: 1"
                                    "\n";
  EXPECT_EQ(expectedDescription, executeResult.description);
}

TEST(Parser, parse_ValidExpressionWithHigh0_Returns0)
{
  // 4d1h0
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Integer, .integerValue = 4},
      Token{.tokenType = TokenType::D, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
      Token{.tokenType = TokenType::H, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 0},
  };

  auto parseResult = parse(input);
  auto executeResult = parseResult->execute();

  EXPECT_EQ(0, executeResult.result);
  std::string expectedDescription = "\nRolling 4d1..."
                                    "\nYou rolled: 1"
                                    "\nYou rolled: 1"
                                    "\nYou rolled: 1"
                                    "\nYou rolled: 1"
                                    "\n";
  EXPECT_EQ(expectedDescription, executeResult.description);
}

TEST(Parser, parse_LongRollWithLow_ReturnsCorrectResult)
{
  // 4d1l2
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Integer, .integerValue = 4},
      Token{.tokenType = TokenType::D, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
      Token{.tokenType = TokenType::L, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 2},
  };

  auto parseResult = parse(input);
  auto executeResult = parseResult->execute();

  EXPECT_EQ(2, executeResult.result);
  std::string expectedDescription = "\nRolling 4d1..."
                                    "\nYou rolled: 1"
                                    "\nYou rolled: 1"
                                    "\nYou rolled: 1"
                                    "\nYou rolled: 1"
                                    "\n";
  EXPECT_EQ(expectedDescription, executeResult.description);
}

TEST(Parser, parse_ValidExpressionWithLow0_Returns0)
{
  // 4d1l0
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Integer, .integerValue = 4},
      Token{.tokenType = TokenType::D, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
      Token{.tokenType = TokenType::L, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 0},
  };

  auto parseResult = parse(input);
  auto executeResult = parseResult->execute();

  EXPECT_EQ(0, executeResult.result);
  std::string expectedDescription = "\nRolling 4d1..."
                                    "\nYou rolled: 1"
                                    "\nYou rolled: 1"
                                    "\nYou rolled: 1"
                                    "\nYou rolled: 1"
                                    "\n";
  EXPECT_EQ(expectedDescription, executeResult.description);
}

TEST(Parser, parse_Addition_ReturnsCorrectResult)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Integer, .integerValue = 4},
      Token{.tokenType = TokenType::Add, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 10},
  };

  auto parseResult = parse(input);
  auto executeResult = parseResult->execute();

  EXPECT_EQ(14, executeResult.result);
  std::string expectedDescription = "";
  EXPECT_EQ(expectedDescription, executeResult.description);
}

TEST(Parser, parse_Subtraction_ReturnsCorrectResult)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Integer, .integerValue = 4},
      Token{.tokenType = TokenType::Subtract, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 10},
  };

  auto parseResult = parse(input);
  auto executeResult = parseResult->execute();

  EXPECT_EQ(-6, executeResult.result);
  std::string expectedDescription = "";
  EXPECT_EQ(expectedDescription, executeResult.description);
}

TEST(Parser, parse_Multiplication_ReturnsCorrectResult)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Integer, .integerValue = 4},
      Token{.tokenType = TokenType::Multiply, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 10},
  };

  auto parseResult = parse(input);
  auto executeResult = parseResult->execute();

  EXPECT_EQ(40, executeResult.result);
  std::string expectedDescription = "";
  EXPECT_EQ(expectedDescription, executeResult.description);
}

TEST(Parser, parse_Division_ReturnsCorrectResult)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Integer, .integerValue = 8},
      Token{.tokenType = TokenType::Divide, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 2},
  };

  auto parseResult = parse(input);
  auto executeResult = parseResult->execute();

  EXPECT_EQ(4, executeResult.result);
  std::string expectedDescription = "";
  EXPECT_EQ(expectedDescription, executeResult.description);
}

TEST(Parser, parse_DivisionWithRemainder_FloorsTheResult)
{
  std::vector<Token> input = {
      Token{.tokenType = TokenType::Integer, .integerValue = 8},
      Token{.tokenType = TokenType::Divide, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 3},
  };

  auto parseResult = parse(input);
  auto executeResult = parseResult->execute();

  EXPECT_EQ(2, executeResult.result);
  std::string expectedDescription = "";
  EXPECT_EQ(expectedDescription, executeResult.description);
}

TEST(
    Parser,
    parse_DivisionMultiplicationAdditionSubtraction_DivMultGoBeforeAddSub
)
{
  std::vector<Token> input = {
      // 1 + 2 * 3 - 4 / 2
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
      Token{.tokenType = TokenType::Add, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 2},
      Token{.tokenType = TokenType::Multiply, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 3},
      Token{.tokenType = TokenType::Subtract, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 4},
      Token{.tokenType = TokenType::Divide, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 2},
  };

  auto parseResult = parse(input);
  auto executeResult = parseResult->execute();

  EXPECT_EQ(5, executeResult.result);
  std::string expectedDescription = "";
  EXPECT_EQ(expectedDescription, executeResult.description);
}

TEST(Parser, parse_Parentheticals_ParentheticalsGoBeforeEverythingElse)
{
  std::vector<Token> input = {
      // (1 + 2) * (8 - 4) / 2
      Token{.tokenType = TokenType::OpenParenthesis, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 1},
      Token{.tokenType = TokenType::Add, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 2},
      Token{.tokenType = TokenType::CloseParenthesis, .integerValue = 0},
      Token{.tokenType = TokenType::Multiply, .integerValue = 0},
      Token{.tokenType = TokenType::OpenParenthesis, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 8},
      Token{.tokenType = TokenType::Subtract, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 4},
      Token{.tokenType = TokenType::CloseParenthesis, .integerValue = 0},
      Token{.tokenType = TokenType::Divide, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 2},
  };

  auto parseResult = parse(input);
  auto executeResult = parseResult->execute();

  EXPECT_EQ(6, executeResult.result);
  std::string expectedDescription = "";
  EXPECT_EQ(expectedDescription, executeResult.description);
}

TEST(Parser, parse_NestedParentheticals_DeeperParentheticalsGoFirst)
{
  std::vector<Token> input = {
      // (2 * (3 - (4 / 2)))
      Token{.tokenType = TokenType::OpenParenthesis, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 2},
      Token{.tokenType = TokenType::Multiply, .integerValue = 0},
      Token{.tokenType = TokenType::OpenParenthesis, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 3},
      Token{.tokenType = TokenType::Subtract, .integerValue = 0},
      Token{.tokenType = TokenType::OpenParenthesis, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 4},
      Token{.tokenType = TokenType::Divide, .integerValue = 0},
      Token{.tokenType = TokenType::Integer, .integerValue = 2},
      Token{.tokenType = TokenType::CloseParenthesis, .integerValue = 0},
      Token{.tokenType = TokenType::CloseParenthesis, .integerValue = 0},
      Token{.tokenType = TokenType::CloseParenthesis, .integerValue = 0},
  };

  auto parseResult = parse(input);
  auto executeResult = parseResult->execute();

  EXPECT_EQ(2, executeResult.result);
  std::string expectedDescription = "";
  EXPECT_EQ(expectedDescription, executeResult.description);
}
