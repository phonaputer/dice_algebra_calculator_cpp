#include "lexer.hpp"
#include <iostream>
#include <string>

using namespace std;

int main()
{
    string userInput;

    cout << "Please enter a dice algebra expression: ";

    getline(cin, userInput);

    cout << "You wrote: " << userInput << endl;

    auto tokens = tokenize(userInput);

    for (auto token : tokens)
    {
        cout << "Got token: " << static_cast<int>(token.tokenType) << "\n";
    }

    // TODO add exception handling here to print nicer error messages.

    return 0;
}
