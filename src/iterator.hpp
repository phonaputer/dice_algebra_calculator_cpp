#include <optional>
#include <vector>

template <typename T> class Iterator
{
private:
  std::vector<T> vec;
  unsigned int currentElement = 0;

public:
  explicit Iterator(std::vector<T> v) : vec{v} {}

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
};
