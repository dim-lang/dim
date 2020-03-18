// Copyright 2019- <coli-lang>
// Apache License Version 2.0
#pragma once
#include "boost/core/noncopyable.hpp"
#include "interface/Stringify.h"
#include <functional>
#include <utility>

namespace detail {

template <typename K, typename V> class LinkedNode;

// linked list
template <typename K, typename V>
class LinkedList : private boost::noncopyable {
public:
  LinkedList();
  ~LinkedList();

  // copy
  LinkedList(const LinkedList<K, V> &other);
  LinkedList<K, V> &operator=(const LinkedList<K, V> &other);

  // attribute
  LinkedNode<K, V> *&prev();
  const LinkedNode<K, V> *prev() const;
  LinkedNode<K, V> *&next();
  const LinkedNode<K, V> *next() const;
  LinkedNode<K, V> *&seq_prev();
  const LinkedNode<K, V> *seq_prev() const;
  LinkedNode<K, V> *&seq_next();
  const LinkedNode<K, V> *seq_next() const;

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
  bool empty() const;
  bool seq_empty() const;

private:
  LinkedNode<K, V> *prev_;
  LinkedNode<K, V> *next_;
  LinkedNode<K, V> *seq_prev_;
  LinkedNode<K, V> *seq_next_;
};

// linked node
template <typename K, typename V>
class LinkedNode : private boost::noncopyable {
public:
  LinkedNode(const std::pair<const K, V> &value);
  ~LinkedNode();

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

private:
  LinkedNode<K, V> *prev_;
  LinkedNode<K, V> *next_;
  LinkedNode<K, V> *seq_prev_;
  LinkedNode<K, V> *seq_next_;
  std::pair<const K, V> value_;
};

// linked iterator
template <typename K, typename V> class LinkedIterator : public Stringify {
public:
  LinkedIterator();
  LinkedIterator(LinkedNode<K, V> *node);
  ~LinkedIterator();
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

// linked reverse iterator
template <typename K, typename V> class LinkedRIterator : public Stringify {
public:
  LinkedRIterator();
  LinkedRIterator(LinkedNode<K, V> *node);
  ~LinkedRIterator();
  LinkedRIterator(const LinkedRIterator<K, V> &other);
  LinkedRIterator<K, V> &operator=(const LinkedRIterator<K, V> &other);
  LinkedRIterator<K, V> &operator++();
  LinkedRIterator<K, V> &operator--();
  LinkedRIterator<K, V> operator++(int);
  LinkedRIterator<K, V> operator--(int);
  bool operator==(const LinkedRIterator<K, V> &other) const;
  bool operator!=(const LinkedRIterator<K, V> &other) const;
  bool operator!() const;
  std::pair<const K, V> &operator*();
  const std::pair<const K, V> &operator*() const;
  std::pair<const K, V> *operator->();
  const std::pair<const K, V> *operator->() const;
  virtual std::string toString() const;

private:
  LinkedNode<K, V> *node_;
};

// linked hashtable
template <typename K, typename V, typename H, typename E>
class LinkedHt : private boost::noncopyable {
public:
  LinkedHt();
  LinkedHt(int bucket);
  ~LinkedHt();
  bool empty() const;
  int size() const;
  int bucket() const;
  double load() const;
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
  LinkedRIterator<K, V> rbegin();
  const LinkedRIterator<K, V> rbegin() const;
  LinkedRIterator<K, V> rend();
  const LinkedRIterator<K, V> rend() const;

private:
  int getBucket(const K &key, int bucket) const;
  int alignBucket(int n) const;
  void extend(int n);
  void destroyList(int i);

  H hasher_;
  E equal_;
  LinkedList<K, V> head_;
  LinkedList<K, V> *ht_; // hash table head node
  int *count_;           // node count in list
  int bucket_;           // hash table array capacity
  int size_;             // hash table node count
};

} // namespace detail

// linked hashmap
template <typename K, typename V, typename H = std::hash<K>,
          typename E = std::equal_to<K>>
class LinkedHashMap : private boost::noncopyable {
public:
  using Iterator = typename detail::LinkedIterator<K, V>;
  using CIterator = typename detail::LinkedIterator<K, V> const;
  using RIterator = typename detail::LinkedRIterator<K, V>;
  using CRIterator = typename detail::LinkedRIterator<K, V> const;

  LinkedHashMap();
  LinkedHashMap(int bucket);
  ~LinkedHashMap();
  bool empty() const;
  int size() const;
  int bucket() const;
  double load() const;
  void clear();
  void release();
  Iterator begin();
  CIterator begin() const;
  Iterator end();
  CIterator end() const;
  RIterator rbegin();
  CRIterator rbegin() const;
  RIterator rend();
  CRIterator rend() const;

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

  // return   end() if not found, other if found
  Iterator find(const K &key) const;

  V &operator[](const K &key);
  const V &operator[](const K &key) const;

private:
  detail::LinkedHt<K, V, H, E> hm_;
};
