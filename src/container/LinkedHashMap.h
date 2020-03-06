// Copyright 2019- <coli-lang>
// Apache License Version 2.0
#pragma once
#include "interface/Stringify.h"
#include <functional>
#include <utility>

namespace detail {

// node
template <typename K, typename V> class LinkedNode {
public:
  LinkedNode();
  LinkedNode(const std::pair<const K, V> &value);
  virtual ~LinkedNode();

  // copy
  LinkedNode(const LinkedNode<K, V> &other);
  LinkedNode<K, V> &operator=(const LinkedNode<K, V> &other);

  // attribute
  const K &key() const;
  V &mapped();
  const V &mapped() const;
  const std::pair<const K, V> &value() const;
  std::pair<const K, V> &value();
  LinkedNode<K, V> *&prev();
  const LinkedNode<K, V> *prev() const;
  LinkedNode<K, V> *&next();
  const LinkedNode<K, V> *next() const;
  LinkedNode<K, V> *&seq_prev();
  const LinkedNode<K, V> *seq_prev() const;
  LinkedNode<K, V> *&seq_next();
  const LinkedNode<K, V> *seq_next() const;

  // list operation
  template <typename K1, typename V1>
  static void initializeList(LinkedNode<K1, V1> &l);
  LinkedNode<K, V> *&head();
  const LinkedNode<K, V> *head() const;
  LinkedNode<K, V> *&tail();
  const LinkedNode<K, V> *tail() const;
  void insertTail(LinkedNode<K, V> *e);
  void insertHead(LinkedNode<K, V> *e);
  LinkedNode<K, V> *removeTail();
  LinkedNode<K, V> *removeHead();
  void remove(LinkedNode<K, V> *e);
  void seq_insert(LinkedNode<K, V> *e);
  void seq_remove(LinkedNode<K, V> *e);
  bool isNull() const;

private:
  std::pair<const K, V> value_;
  LinkedNode<K, V> *prev_;
  LinkedNode<K, V> *next_;
  LinkedNode<K, V> *seq_prev_;
  LinkedNode<K, V> *seq_next_;
};

// iterator
template <typename K, typename V> class LinkedIterator : public Stringify {
public:
  LinkedIterator(LinkedNode<K, V> *node);
  virtual ~LinkedIterator();
  LinkedIterator(const LinkedIterator<K, V> &other);
  LinkedIterator<K, V> &operator=(const LinkedIterator<K, V> &other);
  LinkedIterator<K, V> &operator++();
  LinkedIterator<K, V> &operator--();
  LinkedIterator<K, V> operator++(int);
  LinkedIterator<K, V> operator--(int);
  bool operator==(const LinkedIterator<K, V> &other) const;
  bool operator!=(const LinkedIterator<K, V> &other) const;
  bool operator!() const;
  std::pair<const K, V> &operator*();
  const std::pair<const K, V> &operator*() const;
  std::pair<const K, V> *operator->();
  const std::pair<const K, V> *operator->() const;
  virtual std::string toString() const;

private:
  LinkedNode<K, V> *node_;
};

// hash table
template <typename K, typename V, typename H, typename E> class LinkedHt {
public:
  LinkedHt(double threshold);
  LinkedHt(int bucket, double threshold);
  virtual ~LinkedHt();
  bool empty() const;
  int size() const;
  int bucket() const;
  double load() const;
  const double &threshold() const;
  double &threshold();
  void clear();
  void release();
  void insert(const std::pair<const K, V> &value);
  int insertOrAssign(const std::pair<const K, V> &value);
  LinkedIterator<K, V> find(const K &key) const;
  int remove(LinkedIterator<K, V> position);
  LinkedIterator<K, V> begin();
  const LinkedIterator<K, V> begin() const;
  LinkedIterator<K, V> end();
  const LinkedIterator<K, V> end() const;

private:
  bool isNotNull();
  void extend(int n);
  void destroyList(int i);

  H hasher_;
  E equal_;
  LinkedNode<K, V> head_;
  LinkedNode<K, V> *ht_; // hash table head node
  int *count_;           // node count in list
  int bucket_;           // hash table array capacity
  int size_;             // hash table node count
  double threshold_;     // load factor threshold
};

} // namespace detail

template <typename K, typename V, typename H = std::hash<K>,
          typename E = std::equal_to<K>>
class LinkedHashMap {
public:
  using Iterator = typename detail::LinkedIterator<K, V>;
  using CIterator = typename detail::LinkedIterator<K, V> const;

  LinkedHashMap(double load = 4.0);
  LinkedHashMap(int bucket, double load = 4.0);
  virtual ~LinkedHashMap();
  bool empty() const;
  int size() const;
  int bucket() const;
  double load() const;
  const double &threshold() const;
  double &threshold();
  void clear();
  void release();
  Iterator begin();
  CIterator begin() const;
  Iterator end();
  CIterator end() const;

  void insert(const K &key, const V &mapped);
  void insert(const std::pair<const K, V> &value);

  // return   1 if insert, 0 if assign
  int insertOrAssign(const K &key, const V &mapped);
  int insertOrAssign(const std::pair<const K, V> &value);

  // return   0 if success, -1 if not exist
  int remove(const K &key);
  int remove(Iterator position);

  // return   true if exist, false if not exist
  bool exist(const K &key) const;

  // return   this->end() if not found, other if found
  Iterator find(const K &key) const;

  V &operator[](const K &key);
  const V &operator[](const K &key) const;

private:
  detail::LinkedHt<K, V, H, E> hm_;
};
