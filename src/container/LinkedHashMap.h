// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0
#pragma once
#include "boost/core/noncopyable.hpp"
#include "interface/Stringify.h"
#include <functional>
#include <utility>

namespace detail {

// linked node
template <typename K, typename V>
class LinkedNode : private boost::noncopyable {
public:
  LinkedNode(const std::pair<const K, V> &value);
  ~LinkedNode();

  // attribute
  LinkedNode<K, V> *prev;
  LinkedNode<K, V> *next;
  LinkedNode<K, V> *seq_prev;
  LinkedNode<K, V> *seq_next;
  std::pair<const K, V> value;

  const K &key() const;
};

// linked list
template <typename K, typename V>
class LinkedList : private boost::noncopyable {
public:
  LinkedList();
  ~LinkedList();

  // attribute
  LinkedNode<K, V> *prev;
  LinkedNode<K, V> *next;
  LinkedNode<K, V> *seq_prev;
  LinkedNode<K, V> *seq_next;

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
};

template <typename K, typename V, typename NodePtr> class LinkedIterator;
template <typename K, typename V, typename NodePtr> class LinkedConstIterator;
template <typename K, typename V, typename NodePtr> class LinkedReverseIterator;
template <typename K, typename V, typename NodePtr>
class LinkedConstReverseIterator;

// linked iterator
template <typename K, typename V, typename NodePtr>
class LinkedIterator : public Stringify {
public:
  LinkedIterator(NodePtr node = nullptr);
  ~LinkedIterator() = default;

  // convert with other iterators
  LinkedIterator(const LinkedIterator<K, V, NodePtr> &other);
  LinkedIterator(const LinkedConstIterator<K, V, NodePtr> &other);
  LinkedIterator(const LinkedReverseIterator<K, V, NodePtr> &other);
  LinkedIterator(const LinkedConstReverseIterator<K, V, NodePtr> &other);
  LinkedIterator<K, V, NodePtr> &
  operator=(const LinkedIterator<K, V, NodePtr> &other);
  LinkedIterator<K, V, NodePtr> &
  operator=(const LinkedConstIterator<K, V, NodePtr> &other);
  LinkedIterator<K, V, NodePtr> &
  operator=(const LinkedReverseIterator<K, V, NodePtr> &other);
  LinkedIterator<K, V, NodePtr> &
  operator=(const LinkedConstReverseIterator<K, V, NodePtr> &other);

  LinkedIterator<K, V, NodePtr> &operator++();
  LinkedIterator<K, V, NodePtr> &operator--();
  LinkedIterator<K, V, NodePtr> operator++(int);
  LinkedIterator<K, V, NodePtr> operator--(int);
  bool operator==(const LinkedIterator<K, V, NodePtr> &other) const;
  bool operator!=(const LinkedIterator<K, V, NodePtr> &other) const;
  bool operator!() const;
  std::pair<const K, V> &operator*();
  std::pair<const K, V> *operator->();
  virtual std::string toString() const;

private:
  NodePtr node_;
};

// linked const iterator
template <typename K, typename V, typename NodePtr>
class LinkedConstIterator : public Stringify {
public:
  LinkedConstIterator(const NodePtr node = nullptr);
  ~LinkedConstIterator() = default;

  // convert with other iterators
  LinkedConstIterator(const LinkedIterator<K, V, NodePtr> &other);
  LinkedConstIterator(const LinkedConstIterator<K, V, NodePtr> &other);
  LinkedConstIterator(const LinkedReverseIterator<K, V, NodePtr> &other);
  LinkedConstIterator(const LinkedConstReverseIterator<K, V, NodePtr> &other);
  LinkedConstIterator<K, V, NodePtr> &
  operator=(const LinkedIterator<K, V, NodePtr> &other);
  LinkedConstIterator<K, V, NodePtr> &
  operator=(const LinkedConstIterator<K, V, NodePtr> &other);
  LinkedConstIterator<K, V, NodePtr> &
  operator=(const LinkedReverseIterator<K, V, NodePtr> &other);
  LinkedConstIterator<K, V, NodePtr> &
  operator=(const LinkedConstReverseIterator<K, V, NodePtr> &other);

  LinkedConstIterator<K, V, NodePtr> &operator++();
  LinkedConstIterator<K, V, NodePtr> &operator--();
  LinkedConstIterator<K, V, NodePtr> operator++(int);
  LinkedConstIterator<K, V, NodePtr> operator--(int);
  bool operator==(const LinkedConstIterator<K, V, NodePtr> &other) const;
  bool operator!=(const LinkedConstIterator<K, V, NodePtr> &other) const;
  bool operator!() const;
  const std::pair<const K, V> &operator*() const;
  const std::pair<const K, V> *operator->() const;
  virtual std::string toString() const;

private:
  NodePtr node_;
};

// linked reverse iterator
template <typename K, typename V, typename NodePtr>
class LinkedReverseIterator : public Stringify {
public:
  LinkedReverseIterator(NodePtr node = nullptr);
  ~LinkedReverseIterator() = default;

  // convert to other iterators
  LinkedReverseIterator(const LinkedIterator<K, V, NodePtr> &other);
  LinkedReverseIterator(const LinkedConstIterator<K, V, NodePtr> &other);
  LinkedReverseIterator(const LinkedReverseIterator<K, V, NodePtr> &other);
  LinkedReverseIterator(const LinkedConstReverseIterator<K, V, NodePtr> &other);
  LinkedReverseIterator<K, V, NodePtr> &
  operator=(const LinkedIterator<K, V, NodePtr> &other);
  LinkedReverseIterator<K, V, NodePtr> &
  operator=(const LinkedConstIterator<K, V, NodePtr> &other);
  LinkedReverseIterator<K, V, NodePtr> &
  operator=(const LinkedReverseIterator<K, V, NodePtr> &other);
  LinkedReverseIterator<K, V, NodePtr> &
  operator=(const LinkedConstReverseIterator<K, V, NodePtr> &other);

  LinkedReverseIterator<K, V, NodePtr> &operator++();
  LinkedReverseIterator<K, V, NodePtr> &operator--();
  LinkedReverseIterator<K, V, NodePtr> operator++(int);
  LinkedReverseIterator<K, V, NodePtr> operator--(int);
  bool operator==(const LinkedReverseIterator<K, V, NodePtr> &other) const;
  bool operator!=(const LinkedReverseIterator<K, V, NodePtr> &other) const;
  bool operator!() const;
  std::pair<const K, V> &operator*();
  std::pair<const K, V> *operator->();
  virtual std::string toString() const;

private:
  NodePtr node_;
};

// linked const reverse iterator
template <typename K, typename V, typename NodePtr>
class LinkedConstReverseIterator : public Stringify {
public:
  LinkedConstReverseIterator(const NodePtr node = nullptr);
  ~LinkedConstReverseIterator() = default;

  // convert to other iterators
  LinkedConstReverseIterator(const LinkedIterator<K, V, NodePtr> &other);
  LinkedConstReverseIterator(const LinkedConstIterator<K, V, NodePtr> &other);
  LinkedConstReverseIterator(const LinkedReverseIterator<K, V, NodePtr> &other);
  LinkedConstReverseIterator(
      const LinkedConstReverseIterator<K, V, NodePtr> &other);
  LinkedConstReverseIterator<K, V, NodePtr> &
  operator=(const LinkedIterator<K, V, NodePtr> &other);
  LinkedConstReverseIterator<K, V, NodePtr> &
  operator=(const LinkedConstIterator<K, V, NodePtr> &other);
  LinkedConstReverseIterator<K, V, NodePtr> &
  operator=(const LinkedReverseIterator<K, V, NodePtr> &other);
  LinkedConstReverseIterator<K, V, NodePtr> &
  operator=(const LinkedConstReverseIterator<K, V, NodePtr> &other);

  LinkedConstReverseIterator<K, V, NodePtr> &operator++();
  LinkedConstReverseIterator<K, V, NodePtr> &operator--();
  LinkedConstReverseIterator<K, V, NodePtr> operator++(int);
  LinkedConstReverseIterator<K, V, NodePtr> operator--(int);
  bool operator==(const LinkedConstReverseIterator<K, V, NodePtr> &other) const;
  bool operator!=(const LinkedConstReverseIterator<K, V, NodePtr> &other) const;
  bool operator!() const;
  const std::pair<const K, V> &operator*() const;
  const std::pair<const K, V> *operator->() const;
  virtual std::string toString() const;

private:
  NodePtr node_;
};

// linked hashmap
template <typename K, typename V, typename H, typename E>
class LinkedHM : private boost::noncopyable {
public:
  using Iterator = LinkedIterator<K, V, LinkedNode<K, V> *>;
  using CIterator = LinkedConstIterator<K, V, LinkedNode<K, V> *>;
  using RIterator = LinkedReverseIterator<K, V, LinkedNode<K, V> *>;
  using CRIterator = LinkedConstReverseIterator<K, V, LinkedNode<K, V> *>;

  LinkedHM();
  LinkedHM(int bucket);
  ~LinkedHM();

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

  H hasher_;
  E equal_;
  LinkedList<K, V> head_;
  LinkedList<K, V> *ht_; // hash map head node
  int *count_;           // node count in list
  int bucket_;           // hash map array capacity
  int size_;             // hash map node count
};

} // namespace detail

// linked hashmap
template <typename K, typename V, typename H = std::hash<K>,
          typename E = std::equal_to<K>>
class LinkedHashMap : private boost::noncopyable {
public:
  using Iterator = typename detail::LinkedHM<K, V, H, E>::Iterator;
  using CIterator = typename detail::LinkedHM<K, V, H, E>::CIterator;
  using RIterator = typename detail::LinkedHM<K, V, H, E>::RIterator;
  using CRIterator = typename detail::LinkedHM<K, V, H, E>::CRIterator;

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
  detail::LinkedHM<K, V, H, E> hm_;
};
