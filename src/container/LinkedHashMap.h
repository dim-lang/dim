// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Log.h"
#include <list>
#include <unordered_map>
#include <utility>
#include <vector>

namespace detail {

template <typename K, typename V> class LinkedNode {
public:
  LinkedNode(LinkedNode<K, V> *prev = nullptr, LinkedNode<K, V> *next = nullptr,
             LinkedNode<K, V> *tr_prev = nullptr,
             LinkedNode<K, V> *tr_next = nullptr)
      : prev_(prev), next_(next), tr_prev_(tr_prev), tr_next_(tr_next) {}
  LinkedNode(const std::pair<const K, V> &value,
             LinkedNode<K, V> *prev = nullptr, LinkedNode<K, V> *next = nullptr,
             LinkedNode<K, V> *tr_prev = nullptr,
             LinkedNode<K, V> *tr_next = nullptr)
      : value_(value), prev_(prev), next_(next), tr_prev_(tr_prev),
        tr_next_(tr_next) {}
  virtual ~LinkedNode() {
    prev_ = nullptr;
    next_ = nullptr;
    tr_prev_ = nullptr;
    tr_next_ = nullptr;
  }

  // copy
  LinkedNode(const LinkedNode<K, V> &other)
      : value_(other.value_), prev_(other.prev_), next_(other.next_),
        tr_prev_(other.tr_prev_), tr_next_(other.tr_next_) {}
  LinkedNode<K, V> &operator=(const LinkedNode<K, V> &other) {
    if (this == &other) {
      return *this;
    }
    value_ = other.value();
    prev_ = other.prev();
    next_ = other.next();
    tr_prev_ = other.tr_prev();
    tr_next_ = other.tr_next();
    return *this;
  }

  // attribute
  const K &key() const { return value_.first; }
  V &mapped() { return value_.second; }
  const V &mapped() const { return value_.second; }
  const std::pair<const K, V> &value() const { return value_; }
  std::pair<const K, V> &value() { return value_; }
  LinkedNode<K, V> *&prev() { return prev_; }
  const LinkedNode<K, V> *prev() const { return prev_; }
  LinkedNode<K, V> *&next() { return next_; }
  const LinkedNode<K, V> *next() const { return next_; }
  LinkedNode<K, V> *&tr_prev() { return tr_prev_; }
  const LinkedNode<K, V> *tr_prev() const { return tr_prev_; }
  LinkedNode<K, V> *&tr_next() { return tr_next_; }
  const LinkedNode<K, V> *tr_next() const { return tr_next_; }

  // list operation
  template <typename K, typename V>
  static void initializeList(LinkedNode<K, V> &l) {
    CASSERT(!l.prev(), "l#prev {} is null", (void *)l.prev());
    CASSERT(!l.next(), "l#next {} is null", (void *)l.next());
    l.prev() = &l;
    l.next() = &l;
    l.tr_prev() = &l;
    l.tr_next() = &l;
  }
  LinkedNode<K, V> *&head() { return next_; }
  const LinkedNode<K, V> *head() const { return next_; }
  LinkedNode<K, V> *&tail() { return prev_; }
  const LinkedNode<K, V> *tail() const { return prev_; }
  void insertTail(LinkedNode<K, V> *e) {
    e->prev() = prev_;
    e->next() = this;
    prev_->next() = e;
    prev_ = e;
  }
  void insertHead(LinkedNode<K, V> *e) {
    e->next() = next_;
    e->prev() = this;
    next_->prev() = e;
    next_ = e;
  }
  LinkedNode<K, V> *removeTail() {
    if (empty()) {
      return this;
    }
    LinkedNode<K, V> *e = prev_;
    prev_->prev()->next() = this;
    prev_ = prev_->prev();
    return e;
  }
  LinkedNode<K, V> *removeHead() {
    if (empty()) {
      return this;
    }
    LinkedNode<K, V> *e = next_;
    next_->next()->prev() = this;
    next_ = next_->next();
    return e;
  }
  void remove(LinkedNode<K, V> *e) {
    if (empty()) {
      return;
    }
    e->prev()->next() = e->next();
    e->next()->prev() = e->prev();
    e->prev() = nullptr;
    e->next() = nullptr;
  }

  void tr_insertTail(LinkedNode<K, V> *e) {
    e->tr_prev() = tr_prev_;
    e->tr_next() = this;
    tr_prev_->tr_next() = e;
    tr_prev_ = e;
  }
  void tr_remove(LinkedNode<K, V> *e) {
    if (empty()) {
      return;
    }
    e->tr_prev()->tr_next() = e->tr_next();
    e->tr_next()->tr_prev() = e->tr_prev();
    e->tr_prev() = nullptr;
    e->tr_next() = nullptr;
  }
  bool empty() const {
    CASSERT(prev_ && next_ && tr_prev_ && tr_next_,
            "prev_ {}, next_ {}, tr_prev_ {}, tr_next_ {} is null",
            (void *)prev_, (void *)next_, (void *)tr_prev_, (void *)tr_next_);
    CASSERT((prev_ == this && next_ == this && tr_prev_ == this &&
             tr_next_ == this) ||
                (prev_ != this && next_ != this && tr_prev_ != this &&
                 tr_next_ != this),
            "this: {}, prev_: {}, next_: {}, tr_prev_: {}, tr_next_: {}",
            (void *)this, (void *)prev_, (void *)next_, (void *)tr_prev_,
            (void *)tr_next_);
    return prev_ == this && next_ == this && tr_prev_ == this &&
           tr_next_ == this;
  }

private:
  std::pair<const K, V> value_;
  LinkedNode<K, V> *prev_;
  LinkedNode<K, V> *next_;
  LinkedNode<K, V> *tr_prev_;
  LinkedNode<K, V> *tr_next_;
};

template <typename K, typename V, typename H, typename E> class LinkedHt {
public:
  LinkedHt()
      : hasher_(), equal_(), head_(), ht_(nullptr), capacity_(0),
        count_(nullptr), size_(0) {}
  LinkedHt(int n)
      : hasher_(), equal_(), head_(), ht_(new LinkedNode<K, V>[n]),
        capacity_(n), count_(new int[n]), size_(0) {
    CASSERT(ht_, "ht_ is null: {}", (void *)ht_);
    CASSERT(count_, "count_ is null: {}", (void *)count_);
    for (int i = 0; i < n; i++) {
      LinkedNode<K, V>::initializeList(ht_[i]);
      count_[i] = 0;
    }
  }
  virtual ~LinkedHt() { release(); }
  bool empty() const {
    CASSERT(size_ >= 0, "size_ {} >= 0", size_);
    return size_ == 0;
  }
  int size() const { return size_; }
  int bucket() const { return capacity_; }
  double loadFactor() const { return (double)size_ / (double)capacity_; }
  void clear() {
    if (notNull()) {
      for (int i = 0; i < n; i++) {
        destroyList(i);
      }
    }
    size_ = 0;
  }
  void release() {
    if (notNull()) {
      for (int i = 0; i < n; i++) {
        destroyList(i);
      }
      CASSERT(head_.empty(), "head_#empty: {}", head_.empty());
      delete ht_;
      delete count_;
      ht_ = nullptr;
      count_ = nullptr;
    }
    capacity_ = 0;
    size_ = 0;
  }

  void insert(const std::pair<const K, V> &value) {
    extend();
    LinkedNode<K, V> *e = new LinkedNode<K, V>(value);
    int b = (int)hasher_(value.first) % capacity_;
    ht_[b].insertHead(e);
    count_[b]++;
  }
  int insertOrAssign(const std::pair<const K, V> &value) {
    extend();
    LinkedIterator<K, V> position = find(value.first);
    if (position == end()) {
      LinkedNode<K, V> *e = new LinkedNode<K, V>(value);
      int b = (int)hasher_(value.first) % capacity_;
      ht_[b].insertHead(e);
      count_[b]++;
      return 1;
    } else {
      position->second = value.second;
      return 0;
    }
  }
  LinkedIterator<K, V> find(const K &key) const {
    int b = (int)hasher_(value.first) % capacity_;
    LinkedNode<K, V> *e = ht_[b].next();
    while (e != &ht_[b]) {
      if (equal_(e->key(), key)) {
        return LinkedIterator<K, V>(e);
      }
    }
    return end();
  }
  int remove(LinkedIterator<K, V> position) {
    int b = (int)hasher_(position->first) % capacity_;
    LinkedNode<K, V> *e = ht_[b].next();
    while (e != &ht_[b]) {
      if (equal_(e->key(), position->first)) {
        ht_[b].remove(e);
        head_.tr_remove(e);
        delete e;
        count_[b]--;
        return 0;
      }
    }
    return -1;
  }

  LinkedIterator<K, V> begin() { return LinkedIterator<K, V>(head_.next()); }
  const LinkedIterator<K, V> begin() const {
    return LinkedIterator<K, V>(head_.next());
  }
  LinkedIterator<K, V> end() { return LinkedIterator<K, V>(&head_); }
  const LinkedIterator<K, V> end() const {
    return LinkedIterator<K, V>(&head_);
  }

  // iterator
  template <typename K, typename V> class LinkedIterator {
  public:
    LinkedIterator(LinkedNode<K, V> *node) : node_(node) {}
    virtual ~LinkedIterator() { node_ = nullptr; }
    LinkedIterator(const LinkedIterator<K, V> &other) : node_(other.node_) {}
    LinkedIterator<K, V> &operator=(const LinkedIterator<K, V> &other) {
      if (this == &other) {
        return *this;
      }
      node_ = other.node_;
      return *this;
    }
    LinkedIterator<K, V> &operator++() {
      node_ = node_->next();
      return *this;
    }
    LinkedIterator<K, V> &operator--() {
      node_ = node_->prev();
      return *this;
    }
    LinkedIterator<K, V> operator++(int) {
      LinkedIterator<K, V> save = node_;
      node_ = node_->next();
      return LinkedIterator<K, V>(save);
    }
    LinkedIterator<K, V> operator--(int) {
      LinkedIterator<K, V> save = node_;
      node_ = node_->prev();
      return LinkedIterator<K, V>(save);
    }
    bool operator==(const LinkedIterator<K, V> &other) const {
      return node_ == other.node_;
    }
    bool operator!=(const LinkedIterator<K, V> &other) const {
      return node_ != other.node_;
    }
    bool operator!() const { return !node_; }
    LinkedNode<K, V> &operator*() { return *node_; }
    LinkedNode<K, V> *operator->() { return node_; }

  private:
    LinkedNode<K, V> *node_;
  };

private:
  void notNull() {
    CASSERT((ht_ && count_) || (!ht_ && !count_),
            "ht_ {} nullable, count_ {} nullable", (void *)ht_, (void *)count_);
    return ht_ && count_;
  }

  void extend() {}

  void destroyList(int i) {
    while (ht_[i].next() != &ht_[i]) {
      LinkedNode<K, V> *e = ht_[i].removeTail();
      head_.tr_remove(e);
      delete e;
      count_[i]--;
    }
    CASSERT(ht_[i].empty(), "ht_[{}]#empty: {}", i, ht_[i].empty());
    CASSERT(count_[i] == 0, "count_[{}] {} == 0", i, count_[i]);
  }

  H hasher_;
  E equal_;
  LinkedNode<K, V> head_;
  LinkedNode<K, V> *ht_; // hash table
  int *count_;           // count_[i] is LinkedNode count in list
  int capacity_;         // hash table array capacity
  int size_;             // hash table node count
};

} // namespace detail

template <typename K, typename V, typename H = std::hash<K>,
          typename E = std::equal_to<E>>
class LinkedHashMap {
public:
  typedef typename detail::LinkedHt<K, V, H, E>::LinkedIterator<K, V> Iterator;
  typedef const typename detail::LinkedHt<K, V, H, E>::LinkedIterator<K, V>
      CIterator;

  LinkedHashMap() : ht_() {}
  LinkedHashMap(int n) : ht_(n) {}
  virtual ~LinkedHashMap() {}
  bool empty() const { return ht_.empty(); }
  int size() const { return ht_.size(); }
  void clear() { ht_.clear(); }
  void release() { ht_.release(); }
  Iterator begin() { return ht_.begin(); }
  CIterator begin() const { return ht_.begin(); }
  Iterator end() { return ht_.end(); }
  CIterator end() const { return ht_.end(); }

  void insert(const K &key, const V &mapped) {
    ht_.insert(std::pair<const K, V>(key, mapped));
  }
  void insert(const std::pair<const K, V> &value) { ht_.insert(value); }
  int insertOrAssign(const K &key, const V &mapped) {
    return ht_.insertOrAssign(std::pair<const K, V>(key, mapped));
  }
  int insertOrAssign(const std::pair<const K, V> &value) {
    return ht_.insertOrAssign(value);
  }
  int remove(const K &key) {
    auto position = find(key);
    return position == end() ? -1 : ht_.remove(position);
  }
  int remove(Iterator position) { return ht_.remove(position); }
  int remove(const Iterator &position) { return ht_.remove(position); }
  bool exist(const K &key) const { return find(key) != end(); }
  Iterator find(const K &key) const { return ht_.find(key); }
  V &operator[](const K &key) {
    Iterator position = ht_.find(key);
    return position->second;
  }
  const V &operator[](const K &key) const {
    CIterator position = ht_.find(key);
    return position->second;
  }

private:
  detail::LinkedHt<K, V, H, E> ht_;
};
