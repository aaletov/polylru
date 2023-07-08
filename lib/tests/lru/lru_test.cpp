#include <gtest/gtest.h>
#include <stdexcept>
#include "../../src/lru/lru.hpp"

typedef polylru::LRU<std::string, int> DefaultLRU;

#define TEST_DESCRIPTION(desc) RecordProperty("description", desc)

// TEST(LRUTest, PositiveSizeOK) {
//   TEST_DESCRIPTION(
//     "IF ..., THEN ..."
//   );
// }

std::pair<std::string, int> get_default_item() {
  return {"Bebrus", 1312};
}

template <typename Key, typename Value>
void init_lru(polylru::LRU<Key, Value>& lru, std::vector<std::pair<Key, Value>> items) {
  for (std::pair<Key, Value> item : items) {
    lru.set(std::get<0>(item), std::get<1>(item));
  }
}

TEST(LRUTest, NegativeSizeErrors) {
  TEST_DESCRIPTION(
    "IF the constructor is called with negative size, THEN it throws "
    "std::length_error"
  );
  ASSERT_THROW(DefaultLRU(-1), std::length_error);
}

TEST(LRUTest, ZeroSizeErrors) {
  TEST_DESCRIPTION(
    "IF the constructor is called with size 0, THEN it throws "
    "std::length_error"
  );
  ASSERT_THROW(DefaultLRU(0), std::length_error);
}

TEST(LRUTest, PositiveSizeOK) {
  TEST_DESCRIPTION(
    "IF the constructor is called with positive size, THEN it doesn't throw"
  );
  ASSERT_NO_THROW(DefaultLRU(1));
}

TEST(LRUTest, SingleElementContainsNonExistingFalse) {
  TEST_DESCRIPTION(
    "IF the size of cache is 1 AND the value with key k1 is not present in"
    "cache AND contains is called with key k1, THEN it returns false"
  );
  DefaultLRU lru = DefaultLRU(1);
  std::pair<std::string, int> item = get_default_item();
  lru.set(std::get<0>(item), std::get<1>(item));
  ASSERT_FALSE(lru.contains(std::get<0>(item) + "another"));
}

TEST(LRUTest, SingleElementContainsExistingTrue) {
  TEST_DESCRIPTION(
    "IF the size of cache is 1 AND the value with key k1 is present in cache "
    "AND contains is called with key k1 THEN it returns true"
  );
  DefaultLRU lru = DefaultLRU(1);
  std::pair<std::string, int> item = get_default_item();
  lru.set(std::get<0>(item), std::get<1>(item));
  ASSERT_TRUE(lru.contains(std::get<0>(item)));
}

TEST(LRUTest, UnknownKeyUB) {
  TEST_DESCRIPTION(
    "IF get with unknown key is called, THEN the behaviour is undefined"
  );
}

TEST(LRUTest, SingleElementSetOK) {
  TEST_DESCRIPTION(
    "IF the size of cache is 1 AND the value with key k1 was set AND get is "
    "called with key k1, THEN it returns the same value"
  );
  DefaultLRU lru = DefaultLRU(1);
  std::pair<std::string, int> item = get_default_item();
  lru.set(std::get<0>(item), std::get<1>(item));
  ASSERT_EQ(lru.get(std::get<0>(item)), std::get<1>(item));
}

TEST(LRUTest, SingleElementDisplaceOneOK) {
  TEST_DESCRIPTION(
    "IF the size of cache is 1 AND there is no more free space in cache AND "
    "set is called with new key THEN it displaces least recently used item"
  );
  DefaultLRU lru = DefaultLRU(1);
  std::pair<std::string, int> item = get_default_item();
  lru.set(std::get<0>(item), std::get<1>(item));
  std::string new_key = std::get<0>(item) + "another";
  int new_value = std::get<1>(item) + 1;
  lru.set(new_key, new_value);

  ASSERT_TRUE(!(lru.contains(std::get<0>(item))));
  ASSERT_EQ(lru.get(new_key), new_value);
}

TEST(LRUTest, SingleElementOverwriteOneOK) {
  TEST_DESCRIPTION(
    "IF the size of cache is 1 AND there is key k1 with value v1 present in "
    "cache AND set with k1 v2 is called, THEN v2 overwrites v1"
  );
  DefaultLRU lru = DefaultLRU(1);
  std::pair<std::string, int> item = get_default_item();
  lru.set(std::get<0>(item), std::get<1>(item));
  int new_value = std::get<1>(item) + 1;
  lru.set(std::get<0>(item), new_value);
  int ow_value = lru.get(std::get<0>(item));

  ASSERT_NE(ow_value, std::get<1>(item));
  ASSERT_EQ(ow_value, new_value);
}

TEST(LRUTest, MultiElementFreeSetOK) {
    TEST_DESCRIPTION(
    "IF the size of cache is >1 AND there is free space in cache AND set with "
    "new key is called, THEN it stores item in cache"
  );
  DefaultLRU lru = DefaultLRU(3);
  std::vector<std::pair<std::string, int>> items = {
    {"a", 1},
    {"b", 2},
  };
  init_lru(lru, items);
  std::pair<std::string, int> new_item = {"c", 3};
  lru.set(std::get<0>(new_item), std::get<1>(new_item));
  items.push_back(new_item);

  for (std::pair<std::string, int> item : items) {
    ASSERT_TRUE(lru.contains(std::get<0>(item)));
  }
}

TEST(LRUTest, MultiElementOverwriteOK) {
    TEST_DESCRIPTION(
    "IF the size of cache is >1 AND there is item k1 v1 in cache AND set with "
    "k1 v2 is called, THEN v2 overwrites v1"
  );
  DefaultLRU lru = DefaultLRU(3);
  std::vector<std::pair<std::string, int>> items = {
    {"a", 1},
    {"b", 2},
    {"c", 3},
  };
  init_lru(lru, items);
  std::pair<std::string, int> new_item = {"c", 4};
  lru.set(std::get<0>(new_item), std::get<1>(new_item));

  ASSERT_EQ(lru.get(std::get<0>(new_item)), std::get<1>(new_item));
  for (std::pair<std::string, int> item : items) {
    ASSERT_TRUE(lru.contains(std::get<0>(item)));
  }
}

TEST(LRUTest, MultiElementSetDisplacesLRU) {
  TEST_DESCRIPTION(
    "IF the size of cache is >1 AND there is no free space in cache AND k1 is "
    "not present in cache AND set k1 v1 is called, THEN new item displaces "
    "the least recently used item"
  );

  DefaultLRU lru = DefaultLRU(3);
  std::vector<std::pair<std::string, int>> items = {
    {"a", 1},
    {"b", 2},
    {"c", 3},
  };
  init_lru(lru, items);
  lru.get("a");
  lru.get("a");
  lru.get("b");
  lru.get("c");
  lru.get("c");
  lru.get("c");

  std::pair<std::string, int> new_item = {"d", 4};
  lru.set(std::get<0>(new_item), std::get<1>(new_item));
    std::vector<std::pair<std::string, int>> expected = {
    {"b", 2},
    {"c", 3},
    new_item,
  };

  for (std::pair<std::string, int> item : expected) {
    ASSERT_EQ(lru.get(std::get<0>(item)), std::get<1>(item));
  }
}

TEST(LRUTest, CapacityOK) {
  TEST_DESCRIPTION(
    "IF the cache was constructed with capacity x AND capacity is called, THEN "
    "it returns x"
  );
  DefaultLRU lru = DefaultLRU(3);
  ASSERT_EQ(lru.capacity(), 3);
}

TEST(LRUTest, SizeOK) {
  TEST_DESCRIPTION(
    "IF x elements with different keys were set AND x <= capacity AND size is "
    "called, THEN it returns x"
  );
  DefaultLRU lru = DefaultLRU(3);
  int size = 0;
  ASSERT_EQ(lru.size(), size);
  std::vector<std::pair<std::string, int>> items = {
    {"a", 1},
    {"b", 2},
    {"c", 3},
  };
  for (std::pair<std::string, int> item : items) {
    lru.set(std::get<0>(item), std::get<1>(item));
    size += 1;
    ASSERT_EQ(lru.size(), size);
  }
}

TEST(LRUTest, SizeDisplacementOK) {
  TEST_DESCRIPTION(
    "IF x elements with different keys were set AND x > capacity AND size is "
    "called, THEN it returns capacity"
  );
  DefaultLRU lru = DefaultLRU(3);
  std::vector<std::pair<std::string, int>> items = {
    {"a", 1},
    {"b", 2},
    {"c", 3},
    {"d", 4},
  };
  init_lru(lru, items);
  ASSERT_EQ(lru.size(), 3);
}

TEST(LRUTest, ResizeExtendOK) {
  TEST_DESCRIPTION(
    "IF x elements stored in cache with capacity y AND resize with z > y is "
    "called, THEN all elements are copied to the resized cache AND capacity "
    "is changed to z"
  );
  DefaultLRU lru = DefaultLRU(3);
  std::vector<std::pair<std::string, int>> items = {
    {"a", 1},
    {"b", 2},
    {"c", 3},
  };
  init_lru(lru, items);
  lru.resize(6);
  std::vector<std::pair<std::string, int>> new_items = {
    {"d", 4},
    {"e", 5},
    {"f", 6},
  };
  items.insert(items.end(), new_items.cbegin(), new_items.cend());
  for (std::pair<std::string, int> item : items) {
    ASSERT_TRUE(lru.contains(std::get<0>(item)));
  }
  ASSERT_EQ(lru.capacity(), 6);
}

TEST(LRUTest, ResizeShrinkOK) {
  TEST_DESCRIPTION(
    "IF x elements stored in cache with capacity y AND resize with z < y is "
    "called, THEN y - z oldest elements are eliminated AND z newest elements "
    "are copied to the resized cache AND capacity is changed to z"
  );
  DefaultLRU lru = DefaultLRU(6);
  std::vector<std::pair<std::string, int>> items = {
    {"a", 1},
    {"b", 2},
    {"c", 3},
    {"d", 4},
    {"e", 5},
    {"f", 6},
  };
  init_lru(lru, items);
  std::vector<std::string> newest_keys = {"b", "e", "f"};
  std::vector<std::string> oldest_keys = {"a", "c", "d"};
  lru.resize(3);
  for (std::string key : newest_keys) {
    ASSERT_TRUE(lru.contains(key));
  }
  for (std::string key : oldest_keys) {
    ASSERT_FALSE(lru.contains(key));
  }
  ASSERT_EQ(lru.capacity(), 3);
}



// TEST(LRUTest, )

