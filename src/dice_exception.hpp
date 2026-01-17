#include <stdexcept>
#include <string>

class DiceException : public std::runtime_error
{
public:
  DiceException(std::string msg) : std::runtime_error(msg) {}
};
