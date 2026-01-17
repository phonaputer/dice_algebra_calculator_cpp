#include "dice_exception.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <format>
#include <iostream>
#include <stdexcept>
#include <string>

int main(int argc, char *argv[])
{
  std::cout << "Please enter a dice algebra expression: ";

  std::string userInput;
  std::getline(std::cin, userInput);

  try
  {
    auto tokens = tokenize(userInput);
    auto abstractSyntaxTree = parse(tokens);
    auto result = abstractSyntaxTree->execute();

    bool verbose = argc > 1 && std::string(argv[1]) == "--v";
    if (verbose)
    {
      std::cout << result.description;
    }

    std::cout << std::format("\nYour result is: {}", result.result)
              << std::endl;
  }
  catch (DiceException &e)
  {
    std::cout << "Error: " << e.what() << std::endl;
    return 1;
  }
  catch (std::exception &e)
  {
    std::cout << "An unexpected error has occurred!\n" << e.what() << std::endl;
    return 2;
  }
  catch (...)
  {
    std::cout << "An unexpected error has occurred!" << std::endl;
    return 2;
  }

  return 0;
}
