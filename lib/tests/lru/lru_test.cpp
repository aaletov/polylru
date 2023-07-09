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
    "ЕСЛИ конструктор вызывается с отрицательным параметром size, ТО "
    "выбрасывается исключение std::length_error"
  );
  ASSERT_THROW(DefaultLRU(-1), std::length_error);
}

TEST(LRUTest, ZeroSizeErrors) {
  TEST_DESCRIPTION(
    "ЕСЛИ конструктор вызывается с параметром size равным 0, ТО выбрасывается "
    "исключение std::length_error"
  );
  ASSERT_THROW(DefaultLRU(0), std::length_error);
}

TEST(LRUTest, PositiveSizeOK) {
  TEST_DESCRIPTION(
    "ЕСЛИ конструктор вызывается с положительным параметром size, ТО "
    "создаётся новый объект"
  );
  ASSERT_NO_THROW(DefaultLRU(1));
}

TEST(LRUTest, SingleElementContainsNonExistingFalse) {
  TEST_DESCRIPTION(
    "ЕСЛИ ёмкость кэша равна 1 И элемент с ключом k1 не присутствует в кэше И "
    "метод contains вызывается с ключом k1, ТО метод возвращает false"
  );
  DefaultLRU lru = DefaultLRU(1);
  std::pair<std::string, int> item = get_default_item();
  lru.set(std::get<0>(item), std::get<1>(item));
  ASSERT_FALSE(lru.contains(std::get<0>(item) + "another"));
}

TEST(LRUTest, SingleElementContainsExistingTrue) {
  TEST_DESCRIPTION(
    "ЕСЛИ ёмкость кэша равна 1 И элемент с ключом k1 присутствует в кэше И "
    "метод contains вызывается с ключом k1 ТО метод возвращает TRUE"
  );
  DefaultLRU lru = DefaultLRU(1);
  std::pair<std::string, int> item = get_default_item();
  lru.set(std::get<0>(item), std::get<1>(item));
  ASSERT_TRUE(lru.contains(std::get<0>(item)));
}

TEST(LRUTest, UnknownKeyUB) {
  TEST_DESCRIPTION(
    "ЕСЛИ метод get вызывается с неизвестным ключом, ТО поведение не "
    "определено"
  );
}

TEST(LRUTest, SingleElementSetOK) {
  TEST_DESCRIPTION(
    "ЕСЛИ ёмкость кэша равна 1 И элемент с ключом k1 и значением v1 "
    "присутствует в кэше И метод get вызывается с ключом k1, ТО метод "
    "возвращает v1"
  );
  DefaultLRU lru = DefaultLRU(1);
  std::pair<std::string, int> item = get_default_item();
  lru.set(std::get<0>(item), std::get<1>(item));
  ASSERT_EQ(lru.get(std::get<0>(item)), std::get<1>(item));
}

TEST(LRUTest, SingleElementDisplaceOneOK) {
  TEST_DESCRIPTION(
    "ЕСЛИ ёмкость кэша равна 1 И кэш заполнен И метод set вызывается с новым "
    "ключом ТО из кэша вытесняется самый старый элемент"
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
    "ЕСЛИ ёмкость кэша равна 1 И в кэше присутствует элемент с ключом k1 и "
    "значением v1 И вызывается метод set с ключом k1 и значением v2, ТО v2 "
    "перезаписывает v1"
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
    "ЕСЛИ ёмкость кэша >1 И кэш не заполнен И вызывается метод set с новым "
    "ключом, ТО элемент сохраняется в кэше"
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
    "ЕСЛИ ёмкость кэша >1 И в кэше присутствует элемент с ключом k1 и "
    "значением v1 И вызывается метод set с ключом k1 и значением v2, ТО v2 "
    "перезаписывает v1"
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
    "ЕСЛИ ёмкость кэша >1 И кэш заполнен И элемент с ключом k1 не присутсвует "
    "в кэше И вызывается метод set с ключом k1 и значением v1, ТО новый "
    "элемент вымещает самый старый элемент"
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
    "ЕСЛИ объект был инициализирован с параметром capacity равном x И "
    "вызывается метод capacity, ТО метод возвращает x"
  );
  DefaultLRU lru = DefaultLRU(3);
  ASSERT_EQ(lru.capacity(), 3);
}

TEST(LRUTest, SizeOK) {
  TEST_DESCRIPTION(
    "ЕСЛИ в кэш было добавлено n элементов с разными ключами И n <= capacity "
    "И вызывается метод size, ТО метод возвращает n"
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
    "ЕСЛИ в кэш было добавлено n элементов с разными ключами И n > capacity И"
    "И вызывается метод size, ТО метод возвращает значение, равное capacity"
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
    "ЕСЛИ x элементов находится в кэше с ёмкостью y И вызывается метод resize "
    "с параметром z > y, ТО все элементы копируются в кэш с новым размером И "
    "значение capacity меняется на z"
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
    "ЕСЛИ x элементов находится в кэше с ёмкостью y И вызывается метод resize "
    "с параметром z < y, ТО y - z старших элементов вымещаются И z младших "
    "элементов копируются в кэш с новым размером И значение capacity меняется "
    "на z"
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
