#include "parser.hpp"
#include "dice_exception.hpp"
#include "iterator.hpp"
#include "random.hpp"
#include <algorithm>
#include <chrono>
#include <format>
#include <random>
#include <string>

enum class MathOperation
{
  Add,
  Subtract,
  Multiply,
  Divide
};

struct MathTreeNodeArgs
{
  std::unique_ptr<Tree> leftOperand;
  std::unique_ptr<Tree> rightOperand;
  MathOperation operation;
};

class MathTreeNode : public Tree
{
private:
  std::unique_ptr<Tree> leftOperand;
  std::unique_ptr<Tree> rightOperand;
  MathOperation operation;

public:
  MathTreeNode(MathTreeNodeArgs args)
      : leftOperand{std::move(args.leftOperand)},
        rightOperand{std::move(args.rightOperand)},
        operation{args.operation}
  {
  }

  TreeExecutionResult execute()
  {
    auto leftResult = leftOperand->execute();
    auto rightResult = rightOperand->execute();

    long result = 0;

    switch (operation)
    {
    case MathOperation::Add:
      result = leftResult.result + rightResult.result;
      break;

    case MathOperation::Subtract:
      result = leftResult.result - rightResult.result;
      break;

    case MathOperation::Multiply:
      result = leftResult.result * rightResult.result;
      break;

    case MathOperation::Divide:
      if (rightResult.result == 0)
      {
        throw DiceException("Division by zero is not allowed.");
      }
      result = leftResult.result / rightResult.result;
      break;
    }

    return {
        .result = result,
        .description = leftResult.description + rightResult.description,
    };
  }
};

struct LongRollTreeNodeArgs
{
  unsigned long die;
  unsigned long faces;
  std::optional<unsigned long> high;
  std::optional<unsigned long> low;
};

class LongRollTreeNode : public Tree
{
private:
  unsigned long die;
  unsigned long faces;
  std::optional<unsigned long> high;
  std::optional<unsigned long> low;

public:
  LongRollTreeNode(LongRollTreeNodeArgs args)
      : die{args.die}, faces{args.faces}, high{args.high}, low{args.low}
  {
  }

  TreeExecutionResult execute()
  {
    if (faces < 1 || die < 1)
    {
      return {
          .result = 0,
          .description = std::format(
              "\nRolling {}d{}...\nYou rolled: {}\n", die, faces, 0
          ),
      };
    }

    std::vector<long> rolls;
    std::string description = std::format("\nRolling {}d{}...\n", die, faces);

    long sum = 0;
    for (unsigned int i = 0; i < die; i++)
    {
      long roll = Random::get(1, faces);
      description += std::format("You rolled: {}\n", roll);
      rolls.push_back(roll);
      sum += roll;
    }

    if (low.has_value() && low.value() < rolls.size())
    {
      sum = 0;
      std::sort(rolls.begin(), rolls.end());
      for (unsigned int i = 0; i < low.value(); i++)
      {
        sum += rolls.at(i);
      }
    }
    else if (high.has_value() && high.value() < rolls.size())
    {
      sum = 0;
      std::sort(rolls.begin(), rolls.end(), std::greater<long>());
      for (unsigned int i = 0; i < high.value(); i++)
      {
        sum += rolls.at(i);
      }
    }

    return {
        .result = sum,
        .description = description,
    };
  }
};

class ShortRollTreeNode : public Tree
{
private:
  unsigned long faces;

public:
  explicit ShortRollTreeNode(unsigned long f) : faces{f} {}

  TreeExecutionResult execute()
  {
    if (faces < 1)
    {
      return {
          .result = 0,
          .description =
              std::format("\nRolling d{}...\nYou rolled: {}\n", faces, 0)
      };
    }

    long result = Random::get(1, faces);
    auto description =
        std::format("\nRolling d{}...\nYou rolled: {}\n", faces, result);

    return {
        .result = result,
        .description = description,
    };
  }
};

class IntegerTreeNode : public Tree
{
private:
  unsigned long integer;

public:
  explicit IntegerTreeNode(unsigned long i) : integer{i} {}

  TreeExecutionResult execute()
  {
    return {
        .result = static_cast<long>(integer),
        .description = "",
    };
  }
};

unsigned long parse_integer_raw(std::unique_ptr<Iterator<Token>> &tokens)
{
  auto nextResult = tokens->next();

  if (!nextResult.has_value() ||
      nextResult.value().tokenType != TokenType::Integer)
  {
    throw DiceException("Input expression is not valid.");
  }

  return nextResult.value().integerValue;
}

std::unique_ptr<Tree> parse_integer(std::unique_ptr<Iterator<Token>> &tokens)
{
  return std::make_unique<IntegerTreeNode>(parse_integer_raw(tokens));
}

std::unique_ptr<Tree> parse_shortroll(std::unique_ptr<Iterator<Token>> &tokens)
{
  auto nextResult = tokens->next();
  if (!nextResult.has_value() || nextResult.value().tokenType != TokenType::D)
  {
    throw std::logic_error(
        "Parse shortroll should not be called when the next token is not D."
    );
  }

  return std::make_unique<ShortRollTreeNode>(parse_integer_raw(tokens));
}

std::unique_ptr<Tree> parse_longroll(std::unique_ptr<Iterator<Token>> &tokens)
{
  auto die = parse_integer_raw(tokens);

  auto nextResult = tokens->next();
  if (!nextResult.has_value() || nextResult.value().tokenType != TokenType::D)
  {
    throw std::logic_error(
        "Parse longroll should not be called when the 2nd token is not D."
    );
  }

  auto faces = parse_integer_raw(tokens);

  nextResult = tokens->peek();
  if (nextResult.has_value())
  {
    switch (nextResult.value().tokenType)
    {
    case TokenType::L:
      // discard L token
      tokens->next();
      return std::make_unique<LongRollTreeNode>(LongRollTreeNodeArgs{
          .die = die,
          .faces = faces,
          .high = std::nullopt,
          .low = parse_integer_raw(tokens),
      });

    case TokenType::H:
      // discard H token
      tokens->next();
      return std::make_unique<LongRollTreeNode>(LongRollTreeNodeArgs{
          .die = die,
          .faces = faces,
          .high = parse_integer_raw(tokens),
          .low = std::nullopt,
      });

    default:
        // do nothing and return at the bottom of this function
        ;
    }
  }

  return std::make_unique<LongRollTreeNode>(LongRollTreeNodeArgs{
      .die = die,
      .faces = faces,
      .high = std::nullopt,
      .low = std::nullopt,
  });
}

std::unique_ptr<Tree> parse_roll(std::unique_ptr<Iterator<Token>> &tokens)
{
  auto nextToken = tokens->peek();
  if (nextToken.has_value() && nextToken.value().tokenType == TokenType::D)
  {
    return parse_shortroll(tokens);
  }

  auto nextNextToken = tokens->peekNext();
  if (nextNextToken.has_value() &&
      nextNextToken.value().tokenType == TokenType::D)
  {
    return parse_longroll(tokens);
  }

  return parse_integer(tokens);
}

std::unique_ptr<Tree> parse_add(std::unique_ptr<Iterator<Token>> &tokens);

std::unique_ptr<Tree> parse_atom(std::unique_ptr<Iterator<Token>> &tokens)
{
  auto nextToken = tokens->peek();
  if (nextToken.has_value() &&
      nextToken.value().tokenType != TokenType::OpenParenthesis)
  {
    return parse_roll(tokens);
  }

  tokens->next(); // discard ( token
  auto result = parse_add(tokens);
  tokens->next(); // discard ) token

  return result;
}

std::unique_ptr<Tree> parse_mult(std::unique_ptr<Iterator<Token>> &tokens)
{
  auto leftOperand = parse_atom(tokens);

  auto peekResult = tokens->peek();
  while (peekResult.has_value())
  {
    MathOperation op;
    switch (peekResult.value().tokenType)
    {
    case TokenType::Multiply:
      op = MathOperation::Multiply;
      break;
    case TokenType::Divide:
      op = MathOperation::Divide;
      break;
    default:
      return leftOperand;
    }

    tokens->next(); // discard * or / token

    auto rightOperand = parse_atom(tokens);

    leftOperand = std::make_unique<MathTreeNode>(MathTreeNodeArgs{
        .leftOperand = std::move(leftOperand),
        .rightOperand = std::move(rightOperand),
        .operation = op,
    });

    peekResult = tokens->peek();
  }

  return leftOperand;
}

std::unique_ptr<Tree> parse_add(std::unique_ptr<Iterator<Token>> &tokens)
{
  auto leftOperand = parse_mult(tokens);

  auto peekResult = tokens->peek();
  while (peekResult.has_value())
  {
    MathOperation op;
    switch (peekResult.value().tokenType)
    {
    case TokenType::Add:
      op = MathOperation::Add;
      break;
    case TokenType::Subtract:
      op = MathOperation::Subtract;
      break;
    default:
      return leftOperand;
    }

    tokens->next(); // discard + or - token

    auto rightOperand = parse_mult(tokens);

    leftOperand = std::make_unique<MathTreeNode>(MathTreeNodeArgs{
        .leftOperand = std::move(leftOperand),
        .rightOperand = std::move(rightOperand),
        .operation = op,
    });

    peekResult = tokens->peek();
  }

  return leftOperand;
}

void validate_parenthesis_count(std::vector<Token> tokens)
{
  int openCount = 0;
  int closeCount = 0;

  for (Token token : tokens)
  {
    if (token.tokenType == TokenType::OpenParenthesis)
    {
      openCount++;
    }
    else if (token.tokenType == TokenType::CloseParenthesis)
    {
      closeCount++;
    }
  }

  if (openCount != closeCount)
  {
    throw DiceException("Expression contains an unclosed parenthetical.");
  }
}

std::unique_ptr<Tree> parse(std::vector<Token> tokens)
{
  validate_parenthesis_count(tokens);

  auto iterator = std::make_unique<Iterator<Token>>(tokens);

  return parse_add(iterator);
}
