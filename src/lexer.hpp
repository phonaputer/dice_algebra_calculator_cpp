#pragma once

#include <string>
#include <vector>

enum class TokenType
{
    D,
    H,
    L,
    Add,
    Subtract,
    Multiply,
    Divide,
    OpenParenthesis,
    CloseParenthesis,
    Integer
};

struct Token
{
    TokenType tokenType;
    unsigned long integerValue;
};

std::vector<Token> tokenize(std::string input);
