#include "lexer.hpp"
#include "parser.hpp"
#include <format>
#include <iostream>
#include <string>

using namespace std;

int main()
{
  string userInput;

  cout << "Please enter a dice algebra expression: ";

  getline(cin, userInput);

  auto tokens = tokenize(userInput);

  auto abstractSyntaxTree = parse(tokens);

  auto result = abstractSyntaxTree->execute();

  cout << result.description << format("\nYour result is: {}", result.result)
       << endl;

  // TODO add exception handling here to print nicer error messages.

  return 0;
}
