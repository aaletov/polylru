#include <gtest/gtest.h>
#include <stdexcept>
#include "../../src/lru/lru.hpp"

typedef polylru::LRU<std::string, int> DefaultLRU;

#define TEST_DESCRIPTION(desc) RecordProperty("description", desc)

TEST(LRUTest, NegativeSizeErrors) {
  TEST_DESCRIPTION(
    "IF the passed size is negative, THEN throws std::length_error"
  );
  ASSERT_THROW(DefaultLRU(-1), std::length_error);
}
