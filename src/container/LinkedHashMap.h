// Copyright 2019- <ac-lang>
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
  // no copy
  LinkedList(const LinkedList<K, V> &other) = delete;
  LinkedList<K, V> &operator=(const LinkedList<K, V> &other) = delete;

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
  // copy
  LinkedNode(const LinkedNode<K, V> &other) = delete;
  LinkedNode<K, V> &operator=(const LinkedNode<K, V> &other) = delete;

  LinkedNode<K, V> *prev_;
  LinkedNode<K, V> *next_;
  LinkedNode<K, V> *seq_prev_;
  LinkedNode<K, V> *seq_next_;
  std::pair<const K, V> value_;
};

template <typename K, typename V, typename T> class LinkedRIterator;

// linked iterator
template <typename K, typename V, typename T>
class LinkedIterator : public Stringify {
public:
  LinkedIterator();
  LinkedIterator(T node);
  ~LinkedIterator();
  LinkedIterator(const LinkedIterator<K, V, T> &other);
  LinkedIterator<K, V, T> &operator=(const LinkedIterator<K, V, T> &other);
  LinkedIterator(const LinkedRIterator<K, V, T> &other);
  LinkedIterator<K, V, T> &operator=(const LinkedRIterator<K, V, T> &other);
  LinkedIterator<K, V, T> &operator++();
  LinkedIterator<K, V, T> &operator--();
  LinkedIterator<K, V, T> operator++(int);
  LinkedIterator<K, V, T> operator--(int);
  T get() const;
  bool operator==(const LinkedIterator<K, V, T> &other) const;
  bool operator!=(const LinkedIterator<K, V, T> &other) const;
  bool operator!() const;
  std::pair<const K, V> &operator*();
  const std::pair<const K, V> &operator*() const;
  std::pair<const K, V> *operator->();
  const std::pair<const K, V> *operator->() const;
  virtual std::string toString() const;

private:
  T node_;
};

// linked reverse iterator
template <typename K, typename V, typename T>
class LinkedRIterator : public Stringify {
public:
  LinkedRIterator();
  LinkedRIterator(T node);
  ~LinkedRIterator();
  LinkedRIterator(const LinkedRIterator<K, V, T> &other);
  LinkedRIterator<K, V, T> &operator=(const LinkedRIterator<K, V, T> &other);
  LinkedRIterator(const LinkedIterator<K, V, T> &other);
  LinkedRIterator<K, V, T> &operator=(const LinkedIterator<K, V, T> &other);
  LinkedRIterator<K, V, T> &operator++();
  LinkedRIterator<K, V, T> &operator--();
  LinkedRIterator<K, V, T> operator++(int);
  LinkedRIterator<K, V, T> operator--(int);
  T get() const;
  bool operator==(const LinkedRIterator<K, V, T> &other) const;
  bool operator!=(const LinkedRIterator<K, V, T> &other) const;
  bool operator!() const;
  std::pair<const K, V> &operator*();
  const std::pair<const K, V> &operator*() const;
  std::pair<const K, V> *operator->();
  const std::pair<const K, V> *operator->() const;
  virtual std::string toString() const;

private:
  T node_;
};

// linked hashtable
template <typename K, typename V, typename H, typename E>
class LinkedHt : private boost::noncopyable {
public:
  using Iterator = LinkedIterator<K, V, LinkedNode<K, V> *>;
  using CIterator = LinkedIterator<K, V, const LinkedNode<K, V> *>;
  using RIterator = LinkedRIterator<K, V, LinkedNode<K, V> *>;
  using CRIterator = LinkedRIterator<K, V, const LinkedNode<K, V> *>;

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

  Iterator find(const K &key);
  CIterator find(const K &key) const;

  int remove(Iterator position);
  int remove(RIterator position);

  Iterator begin();
  CIterator begin() const;
  Iterator end();
  CIterator end() const;
  RIterator rbegin();
  CRIterator rbegin() const;
  RIterator rend();
  CRIterator rend() const;

private:
  int getBucket(const K &key, int bucket) const;
  int alignBucket(int n) const;
  void extend(int n);
  void destroyList(int i);

  LinkedHt(const LinkedHt<K, V, H, E> &other) = delete;
  LinkedHt<K, V, H, E> &operator=(const LinkedHt<K, V, H, E> &other) = delete;

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
  using Iterator = typename detail::LinkedHt<K, V, H, E>::Iterator;
  using CIterator = typename detail::LinkedHt<K, V, H, E>::CIterator;
  using RIterator = typename detail::LinkedHt<K, V, H, E>::RIterator;
  using CRIterator = typename detail::LinkedHt<K, V, H, E>::CRIterator;

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
  int remove(RIterator position);

  // return   true if exist, false if not exist
  bool exist(const K &key) const;

  // return   end() if not found, other if found
  Iterator find(const K &key);
  CIterator find(const K &key) const;

  V &operator[](const K &key);
  const V &operator[](const K &key) const;

private:
  LinkedHashMap(const LinkedHashMap<K, V, H, E> &other) = delete;
  LinkedHashMap<K, V, H, E> &
  operator=(const LinkedHashMap<K, V, H, E> &other) = delete;

  detail::LinkedHt<K, V, H, E> hm_;
};
