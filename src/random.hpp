
// This file stolen from...
// https://www.learncpp.com/cpp-tutorial/global-random-numbers-random-h/
#pragma once

#include <chrono>
#include <random>

namespace Random
{
inline std::mt19937 generate()
{
  std::random_device rd{};

  std::seed_seq ss{
      static_cast<std::seed_seq::result_type>(
          std::chrono::steady_clock::now().time_since_epoch().count()
      ),
      rd(),
      rd(),
      rd(),
      rd(),
      rd(),
      rd(),
      rd()
  };

  return std::mt19937{ss};
}

inline std::mt19937 mt{generate()};

unsigned long get(unsigned long min, unsigned long max)
{
  return std::uniform_int_distribution<unsigned long>{min, max}(mt);
}
} // namespace Random
