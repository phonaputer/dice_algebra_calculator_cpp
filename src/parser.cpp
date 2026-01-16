#include "parser.hpp"
#include <stdexcept>

template <typename T> class Iterator
{
private:
  std::vector<T> vec;
  unsigned int currentElement = 0;

public:
  Iterator(std::vector<T> v) : vec{v} {}

  std::optional<T> next()
  {
    bool haveElement = currentElement < vec.size();
    if (!haveElement)
    {
      return std::nullopt;
    }

    T result = vec.at(currentElement);

    currentElement++;

    return result;
  }

  std::optional<T> peek()
  {
    bool haveElement = currentElement < vec.size();
    if (!haveElement)
    {
      return std::nullopt;
    }

    return vec.at(currentElement);
  }

  std::optional<T> peekNext()
  {
    unsigned int peekIdx = currentElement + 1;

    bool haveElement = peekIdx < vec.size();
    if (!haveElement)
    {
      return std::nullopt;
    }

    return vec.at(peekIdx);
  }

  void reset() { currentElement = 0; }
};

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
    // FIXME
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
    // FIXME
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
    // FIXME
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
    // FIXME
  }
};

unsigned long parse_integer_raw(Iterator<Token> tokens)
{
  auto nextResult = tokens.next();

  if (!nextResult.has_value() ||
      nextResult.value().tokenType != TokenType::Integer)
  {
    throw std::invalid_argument("Input expression is not valid.");
  }

  return nextResult.value().integerValue;
}

std::unique_ptr<Tree> parse_integer(Iterator<Token> tokens)
{
  return std::make_unique<IntegerTreeNode>(parse_integer_raw(tokens));
}

std::unique_ptr<Tree> parse_shortroll(Iterator<Token> tokens)
{
  auto nextResult = tokens.next();
  if (!nextResult.has_value() || nextResult.value().tokenType != TokenType::D)
  {
    throw std::invalid_argument("Unexpected end of input.");
  }

  return std::make_unique<ShortRollTreeNode>(parse_integer_raw(tokens));
}

std::unique_ptr<Tree> parse_longroll(Iterator<Token> tokens)
{
  auto die = parse_integer_raw(tokens);

  auto nextResult = tokens.next();
  if (!nextResult.has_value() || nextResult.value().tokenType != TokenType::D)
  {
    throw std::invalid_argument("Unexpected end of input.");
  }

  auto faces = parse_integer_raw(tokens);

  nextResult = tokens.peek();
  if (nextResult.has_value())
  {
    switch (nextResult.value().tokenType)
    {
    case TokenType::L:
      // discard L token
      tokens.next();
      return std::make_unique<LongRollTreeNode>((LongRollTreeNodeArgs){
          .die = die,
          .faces = faces,
          .high = std::nullopt,
          .low = parse_integer_raw(tokens),
      });

    case TokenType::H:
      // discard H token
      tokens.next();
      return std::make_unique<LongRollTreeNode>((LongRollTreeNodeArgs){
          .die = die,
          .faces = faces,
          .high = parse_integer_raw(tokens),
          .low = std::nullopt,
      });
    }
  }

  return std::make_unique<LongRollTreeNode>((LongRollTreeNodeArgs){
      .die = die,
      .faces = faces,
      .high = std::nullopt,
      .low = std::nullopt,
  });
}

std::unique_ptr<Tree> parse_roll(Iterator<Token> tokens)
{
  auto nextToken = tokens.peek();
  if (nextToken.has_value() && nextToken.value().tokenType == TokenType::D)
  {
    return parse_shortroll(tokens);
  }

  auto nextNextToken = tokens.peek();
  if (nextNextToken.has_value() &&
      nextNextToken.value().tokenType == TokenType::D)
  {
    return parse_longroll(tokens);
  }

  return parse_integer(tokens);
}

std::unique_ptr<Tree> parse_add(Iterator<Token> tokens);

std::unique_ptr<Tree> parse_atom(Iterator<Token> tokens)
{
  auto nextToken = tokens.peek();
  if (nextToken.has_value() &&
      nextToken.value().tokenType != TokenType::OpenParenthesis)
  {
    return parse_roll(tokens);
  }

  tokens.next(); // discard ( token
  auto result = parse_add(tokens);
  tokens.next(); // discard ) token

  return result;
}

std::unique_ptr<Tree> parse_mult(Iterator<Token> tokens)
{
  auto leftOperand = parse_atom(tokens);

  auto peekResult = tokens.peek();
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

    tokens.next(); // discard * or / token

    auto rightOperand = parse_atom(tokens);

    leftOperand = std::make_unique<MathTreeNode>((MathTreeNodeArgs){
        .leftOperand = std::move(leftOperand),
        .rightOperand = std::move(rightOperand),
        .operation = op,
    });
  }

  return leftOperand;
}

std::unique_ptr<Tree> parse_add(Iterator<Token> tokens)
{
  auto leftOperand = parse_mult(tokens);

  auto peekResult = tokens.peek();
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

    tokens.next(); // discard + or - token

    auto rightOperand = parse_mult(tokens);

    leftOperand = std::make_unique<MathTreeNode>((MathTreeNodeArgs){
        .leftOperand = std::move(leftOperand),
        .rightOperand = std::move(rightOperand),
        .operation = op,
    });
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
    throw std::invalid_argument(
        "Expression contains an unclosed parenthetical."
    );
  }
}

std::unique_ptr<Tree> parse(std::vector<Token> tokens)
{
  validate_parenthesis_count(tokens);

  auto iterator = Iterator<Token>(tokens);

  return parse_add(tokens);
}
