namespace polylru {
  template <typename Key, typename Value>
  class LRU {
  public:
    LRU(int size):
      size_(size)
    {};
    void add(Key key, Value value);
    Value remove(Key key);
  private:
    int size_;
  };

  template <typename Key, typename Value>
  void LRU<Key, Value>::add(Key key, Value value) {
    return;
  }

  template <typename Key, typename Value>
  Value LRU<Key, Value>::remove(Key key) {
    return 0;
  }
}