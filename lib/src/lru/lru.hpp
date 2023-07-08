namespace polylru {
  /*!
    \brief Данный шаблон класса реализует кэш с методом вытесенения LRU(Least
    recently used).

    Элементы хранятся ассоциативно, в формате ключ-значение, ёмкость кэша
    задаётся при создании объекта. Размером кэша считается количество элементов,
    сохранённое в контейнере. Если размер кэша равен его ёмкости, он считается
    \b заполненным. Возраст элементов определяется количеством обращений к кэшу,
    в течение которого они не были использованы. Добавление элемента с новым
    ключом в заполненный кэш влечёт за собой вытесенение из кэша по методу LRU,
    самый старый элемент удаляется.

    Функция \ref resize(size_t) позволяет динамически изменить размер
    контейнера. 
  */
  template <typename Key, typename Value>
  class LRU {
  public:
    LRU(size_t capacity):
      capacity_(capacity)
    {};
    Value get(Key key);
    void set(Key key, Value value);
    bool contains(Key key) const noexcept;
    size_t size() const noexcept;
    size_t capacity() const noexcept;
    void resize(size_t new_cap); 

    typedef std::pair<Key, Value> value_type;
    typedef Key key_type;
    typedef Value mapped_type;
  private:
    size_t capacity_;
  };

  /*!
    \brief Возвращает значение, установленное для указанного ключа.

    В случае, если элемент с указанным ключом присутствует в кэше, возвращает
    значение элемента. Попытка доступа по неизвестному ключу влечёт
    неопределенное поведение.
  */
  template <typename Key, typename Value>
  Value LRU<Key, Value>::get(Key key) {
    return 0;
  }

  /*!
    \brief Устанавлияет в кэше значение с указанным ключом.

    В случае, если элемент с указанным ключом сохранён в кэше, устанавливает
    новое значение для данного ключа. В случае, если элемент с указанным ключом
    отсутствует в кэше, добавляет элемент в кэш. Добавление элемента в
    заполненный кэш приводит к вытеснению самого старого элемента.
  */
  template <typename Key, typename Value>
  void LRU<Key, Value>::set(Key key, Value value) {
    return;
  }

  /*!
    \brief Проверяет, сохранён ли в кэше элемент с указанным ключом.
  */
  template <typename Key, typename Value>
  bool LRU<Key, Value>::contains(Key key) const noexcept {
    return false;
  }

  /*!
    \brief Возвращает текущее количество элементов в кэше.

    В случае, если размер кэша равен его ёмкости, добавление элемента с новым
    ключом привёдет к вытеснению самого старого элемента.
  */
  template <typename Key, typename Value>
  size_t LRU<Key, Value>::size() const noexcept {
    return 0;
  }

  /*!
    \brief Возвращает текущую ёмкость кэша.
  */
  template <typename Key, typename Value>
  size_t LRU<Key, Value>::capacity() const noexcept {
    return this->capacity_;
  }

  /*!
    \brief Изменяет ёмкость кэша.

    Метод уменьшает размер кэша в случае, если переданный размер меньше
    текущего, при этом происходит удаление самых старых элементов. В случае,
    если переданный размер больше текущего, кэш увеличивается до указанного
    размера. Если переданный размер равен текущему, изменений не происходит.
  */
  template <typename Key, typename Value>
  void LRU<Key, Value>::resize(size_t new_cap) {
    return;
  }
}