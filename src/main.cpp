#include "dice_exception.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <format>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

int main()
{
  string userInput;

  cout << "Please enter a dice algebra expression: ";

  getline(cin, userInput);

  try
  {
    auto tokens = tokenize(userInput);
    auto abstractSyntaxTree = parse(tokens);
    auto result = abstractSyntaxTree->execute();

    cout << result.description << format("\nYour result is: {}", result.result)
         << endl;
  }
  catch (DiceException &e)
  {
    cout << "Error: " << e.what() << endl;
    return 1;
  }
  catch (std::exception &e)
  {
    cout << "An unexpected error has occurred!\n" << e.what() << endl;
    return 2;
  }
  catch (...)
  {
    cout << "An unexpected error has occurred!" << endl;
    return 2;
  }

  return 0;
}
