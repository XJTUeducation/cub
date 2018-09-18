#ifndef HF41FC0CC_15A0_4F16_92AE_15D0759215A8
#define HF41FC0CC_15A0_4F16_92AE_15D0759215A8

#include <cub/repo/map/HashFn.h>
#include <cub/repo/map/EqualFn.h>
#include <cub/repo/map/MapVisitor.h>
#include <cub/repo/list/List.h>
#include <cub/repo/list/ListElem.h>
#include <cub/base/StaticAssert.h>
#include <cub/base/EqHelper.h>
#include <cub/mem/ObjectAllocator.h>
#include <cub/algo/loop.h>

CUB_NS_BEGIN

template<typename KEY, typename VALUE, size_t ELEM_SIZE = 1024,
    size_t HASH_SIZE = ELEM_SIZE, typename HASH_FN = HashFn<KEY, HASH_SIZE>,
    typename EQUAL_FN = EqualFn<KEY> >
struct HashMap {
  STATIC_ASSERT(ELEM_SIZE > 0);
  STATIC_ASSERT(HASH_SIZE > 0);

  template<typename K, typename V>
  struct HashNode: ListElem<HashNode<K, V> > {
    HashNode(const K& k, const V& v)
        : key(k), value(v) {
    }

    K key;
    V value;
  };

  typedef HashNode<KEY, VALUE> Node;

  struct Iterator {
    Iterator() : Iterator(0, 0, nullptr) {
    }

    Iterator(Node* elem, size_t index, const List<Node>* nodeList)
        : elem(elem), index(index), nodeList(nodeList) {
    }

    Iterator(const Iterator& rhs)
        : elem(rhs.elem), index(rhs.index), nodeList(rhs.nodeList) {
    }

    Iterator& operator=(const Iterator& rhs) {
      elem = rhs.elem;
      index = rhs.index;
      nodeList = rhs.nodeList;
      return *this;
    }

    __INLINE_EQUALS(Iterator)
    {
      return (elem == rhs.elem) && (index == rhs.index)
          && (nodeList == rhs.nodeList);
    }

    void reset() {
      elem = 0;
      index = 0;
      nodeList = 0;
    }

    bool isNull() const {
      return elem == 0;
    }

    Node* operator->() {
      return elem;
    }

    Node& operator*() {
      return *elem;
    }

    Node* getValue() const {
      return elem;
    }

    Iterator operator++(int) {
      auto i = *this;
      goNext();
      return i;
    }

    Iterator& operator++() {
      goNext();
      return *this;
    }

  private:
    void goNext() {
      if (elem == 0)
        return;
      isNeighborValid() ? goNeighbor() : goNextValid();
    }

    bool isNeighborValid() const {
      typename List<Node>::Iterator current(elem);
      auto next = nodeList[index].getNext(current);

      return next != nodeList[index].end();
    }

    void goNeighbor() {
      typename List<Node>::Iterator current(elem);
      elem = nodeList[index].getNext(current).getValue();
    }

    void goNextValid() {
      while (++index < HASH_SIZE) {
        if (!nodeList[index].isEmpty()) {
          elem = nodeList[index].getFirst();
          return;
        }
      }

      elem = 0;
    }

  private:
    Node* elem;
    size_t index;
    const List<Node>* nodeList;
  };

  HashMap() : num(0) {
  }

  Iterator begin() const {
    FOREACH(i, HASH_SIZE) {
      if (!buckets[i].isEmpty())
        return {buckets[i].getFirst(), i, buckets};
    }
    return {0, HASH_SIZE, buckets};
  }

  Iterator end() const {
    return {0, HASH_SIZE, buckets};
  }

  size_t size() const {
    return num;
  }

  size_t maxSize() const {
    return ELEM_SIZE;
  }

  bool empty() const {
    return num == 0;
  }

  bool full() const {
    return !allocator.hasFreeSlot();
  }

  const VALUE* get(const KEY& key) const {
    if (auto node = find(key)) {
      return &(node->value);
    }
    return nullptr;
  }

  Status put(const KEY& key, const VALUE& value) {
    if (auto v = get(key)) {
      *const_cast<VALUE*>(v) = value;
      return CUB_SUCCESS;
    }
    return insert(key, value) == nullptr ? CUB_FAILURE : CUB_SUCCESS;
  }

  VALUE& operator[](const KEY& key) {
    if (auto v = get(key)) {
      return *const_cast<VALUE*>(v);
    }
    return insert(key, VALUE())->value;
  }

  void erase(const KEY& key) {
    if (auto node = find(key)) {
      buckets[getIndex(key)].remove(*node);
      allocator.free(node);
      num--;
    }
  }

  void clear() {
    FOREACH(i, HASH_SIZE) {
      while (auto p = buckets[i].popFront()) {
        allocator.free(p);
      }
    }
    num = 0;
  }

  Status visit(MapVisitor<KEY, VALUE>& visitor) {
    return access(visitor);
  }

  Status visit(ConstMapVisitor<KEY, VALUE>& visitor) const {
    return const_cast<HashMap&>(*this).access(visitor);
  }

private:
  template<typename VISITOR>
  Status access(VISITOR& visitor) {
    FOREACH(i, HASH_SIZE) {
      LIST_FOREACH(node, buckets[i]) {
        CUB_ASSERT_SUCC_CALL(visitor.visit(node->key, node->value));
      }
    }

    return CUB_SUCCESS;
  }

  size_t getIndex(const KEY &key) const {
    return hashFn(key) % HASH_SIZE;
  }

  Node* insert(const KEY& key, const VALUE& value) {
    void* p = allocator.alloc();
    if(p == nullptr) return nullptr;

    Node* node = new (p) Node(key, value);
    buckets[getIndex(key)].pushBack(*node);

    num++;

    return node;
  }

  Node* find(const KEY& key) const
  {
    Node* node = buckets[getIndex(key)].search([&](const Node& node) {
      return equalFn(node.key, key);
    });

    if(node == nullptr) {
      return nullptr;
    }
    return node;
  }

private:
  typedef List<Node> Bucket;
  Bucket buckets[HASH_SIZE];
  unsigned int num;

  ObjectAllocator<Node, ELEM_SIZE> allocator;

private:
  HASH_FN hashFn;
  EQUAL_FN equalFn;
};

/////////////////////////////////////////////////////////////
#define MAP_FOREACH_FROM(i, from, items) \
   for(auto i=from; i!=items.end(); ++i)

#define MAP_FOREACH(i, items) \
   MAP_FOREACH_FROM(i, items.begin(), items)

CUB_NS_END

#endif
