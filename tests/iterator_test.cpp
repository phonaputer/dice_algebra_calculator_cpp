#include "iterator.hpp"
#include <gtest/gtest.h>
#include <vector>

TEST(Iterator, Peek_AlreadyReachedEndOfIterator_ReturnsEmpty)
{
  std::vector<int> vec = {};
  auto iterator = Iterator<int>(vec);

  auto result = iterator.peek();

  EXPECT_FALSE(result.has_value());
}

TEST(Iterator, Peek_NotYetReachedEndOfIterator_ReturnsCurrentElement)
{
  std::vector<int> vec = {1};
  auto iterator = Iterator<int>(vec);

  auto result = iterator.peek();

  EXPECT_TRUE(result.has_value());
  EXPECT_EQ(1, result.value());
}

TEST(Iterator, PeekNext_ThereIsNoNextElement_ReturnsEmpty)
{
  std::vector<int> vec = {1};
  auto iterator = Iterator<int>(vec);

  auto result = iterator.peekNext();

  EXPECT_FALSE(result.has_value());
}

TEST(Iterator, PeekNext_ThereIsANextElement_ReturnsNextElement)
{
  std::vector<int> vec = {1, 2};
  auto iterator = Iterator<int>(vec);

  auto result = iterator.peekNext();

  EXPECT_TRUE(result.has_value());
  EXPECT_EQ(2, result.value());
}

TEST(Iterator, Next_AlreadyReachedEndOfIterator_ReturnsEmpty)
{
  std::vector<int> vec = {};
  auto iterator = Iterator<int>(vec);

  auto nextResult = iterator.next();

  EXPECT_FALSE(nextResult.has_value());
}

TEST(Iterator, Next_NotYetReachedEndOfIterator_ReturnsNextElement)
{
  std::vector<int> vec = {1};
  auto iterator = Iterator<int>(vec);

  auto nextResult = iterator.next();

  EXPECT_TRUE(nextResult.has_value());
  EXPECT_EQ(1, nextResult.value());
}

TEST(
    Iterator,
    Next_IteratorHasMultipleRemainingElements_MovesIteratorToTheNextElement
)
{
  std::vector<int> vec = {1, 2};
  auto iterator = Iterator<int>(vec);

  iterator.next();
  auto peekResult = iterator.peek();

  EXPECT_TRUE(peekResult.has_value());
  EXPECT_EQ(2, peekResult.value());
}
