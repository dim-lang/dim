// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "boost/core/noncopyable.hpp"
#include "infra/Cowstr.h"
#include <functional>
#include <utility>

namespace detail {

// linked node
template <typename K, typename V>
class LinkedNode : private boost::noncopyable {
public:
  LinkedNode(const std::pair<const K, V> &value);
  virtual ~LinkedNode();

  // attribute
  LinkedNode<K, V> *prev;
  LinkedNode<K, V> *next;
  LinkedNode<K, V> *traverse_prev;
  LinkedNode<K, V> *traverse_next;
  std::pair<const K, V> value;

  const K &key() const;
};

// linked list
template <typename K, typename V>
class LinkedList : private boost::noncopyable {
public:
  LinkedList();
  virtual ~LinkedList();

  // attribute
  LinkedNode<K, V> *prev;
  LinkedNode<K, V> *next;
  LinkedNode<K, V> *traverse_prev;
  LinkedNode<K, V> *traverse_next;

  LinkedNode<K, V> *&head();
  const LinkedNode<K, V> *head() const;
  LinkedNode<K, V> *&tail();
  const LinkedNode<K, V> *tail() const;
  void insertTail(LinkedNode<K, V> *e);
  void insertHead(LinkedNode<K, V> *e);
  LinkedNode<K, V> *removeTail();
  LinkedNode<K, V> *removeHead();
  void remove(LinkedNode<K, V> *e);
  void traverse_insert(LinkedNode<K, V> *e);
  void traverse_remove(LinkedNode<K, V> *e);
  bool empty() const;
  bool traverse_empty() const;
};

template <typename K, typename V, typename NodePtr> class LinkedIterator;
template <typename K, typename V, typename NodePtr> class LinkedConstIterator;
template <typename K, typename V, typename NodePtr> class LinkedReverseIterator;
template <typename K, typename V, typename NodePtr>
class LinkedConstReverseIterator;

// linked iterator
template <typename K, typename V, typename NodePtr> class LinkedIterator {
public:
  LinkedIterator(NodePtr node = nullptr);
  virtual ~LinkedIterator() = default;

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
  virtual Cowstr str() const;

private:
  friend class LinkedConstIterator<K, V, NodePtr>;
  friend class LinkedReverseIterator<K, V, NodePtr>;
  friend class LinkedConstReverseIterator<K, V, NodePtr>;
  NodePtr node_;
};

// linked const iterator
template <typename K, typename V, typename NodePtr> class LinkedConstIterator {
public:
  LinkedConstIterator(const NodePtr node = nullptr);
  virtual ~LinkedConstIterator() = default;

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
  virtual Cowstr str() const;

private:
  friend class LinkedIterator<K, V, NodePtr>;
  friend class LinkedReverseIterator<K, V, NodePtr>;
  friend class LinkedConstReverseIterator<K, V, NodePtr>;
  NodePtr node_;
};

// linked reverse iterator
template <typename K, typename V, typename NodePtr>
class LinkedReverseIterator {
public:
  LinkedReverseIterator(NodePtr node = nullptr);
  virtual ~LinkedReverseIterator() = default;

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
  virtual Cowstr str() const;

private:
  friend class LinkedIterator<K, V, NodePtr>;
  friend class LinkedConstIterator<K, V, NodePtr>;
  friend class LinkedConstReverseIterator<K, V, NodePtr>;
  NodePtr node_;
};

// linked const reverse iterator
template <typename K, typename V, typename NodePtr>
class LinkedConstReverseIterator {
public:
  LinkedConstReverseIterator(const NodePtr node = nullptr);
  virtual ~LinkedConstReverseIterator() = default;

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
  virtual Cowstr str() const;

private:
  friend class LinkedIterator<K, V, NodePtr>;
  friend class LinkedConstIterator<K, V, NodePtr>;
  friend class LinkedReverseIterator<K, V, NodePtr>;
  NodePtr node_;
};

// linked hashmap
template <typename K, typename V, typename H, typename E>
class LinkedHm : private boost::noncopyable {
public:
  using iterator = LinkedIterator<K, V, LinkedNode<K, V> *>;
  using const_iterator = LinkedConstIterator<K, V, LinkedNode<K, V> *>;
  using reverse_iterator = LinkedReverseIterator<K, V, LinkedNode<K, V> *>;
  using const_reverse_iterator =
      LinkedConstReverseIterator<K, V, LinkedNode<K, V> *>;

  LinkedHm();
  LinkedHm(int bucket);
  virtual ~LinkedHm();

  bool empty() const;
  int size() const;
  int bucket() const;
  double loadFactor() const;

  void clear();
  void release();

  iterator insert(const std::pair<const K, V> &value);
  iterator insertOrAssign(const std::pair<const K, V> &value);

  iterator find(const K &key);
  const_iterator find(const K &key) const;

  iterator remove(iterator position);
  iterator remove(const_iterator position);

  iterator begin();
  const_iterator begin() const;
  const_iterator cbegin() const;
  iterator end();
  const_iterator end() const;
  const_iterator cend() const;
  reverse_iterator rbegin();
  const_reverse_iterator rbegin() const;
  const_reverse_iterator crbegin() const;
  reverse_iterator rend();
  const_reverse_iterator rend() const;
  const_reverse_iterator crend() const;

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
  using iterator = typename detail::LinkedHm<K, V, H, E>::iterator;
  using const_iterator = typename detail::LinkedHm<K, V, H, E>::const_iterator;
  using reverse_iterator =
      typename detail::LinkedHm<K, V, H, E>::reverse_iterator;
  using const_reverse_iterator =
      typename detail::LinkedHm<K, V, H, E>::const_reverse_iterator;

  LinkedHashMap();
  LinkedHashMap(int bucket);
  virtual ~LinkedHashMap();

  bool empty() const;
  int size() const;
  int bucket() const;
  double loadFactor() const;

  void clear();
  void release();

  iterator insert(const K &key, const V &mapped);
  iterator insert(const std::pair<const K, V> &value);

  // return   1 if insert, 0 if assign
  iterator insertOrAssign(const K &key, const V &mapped);
  iterator insertOrAssign(const std::pair<const K, V> &value);

  // return   0 if success, -1 if not exist
  iterator remove(const K &key);
  iterator remove(iterator position);
  iterator remove(const_iterator position);

  // return   true if exist, false if not exist
  bool contains(const K &key) const;

  // return   end() if not found, other if found
  iterator find(const K &key);
  const_iterator find(const K &key) const;

  V &operator[](const K &key);
  const V &operator[](const K &key) const;

  iterator begin();
  const_iterator begin() const;
  const_iterator cbegin() const;
  iterator end();
  const_iterator end() const;
  const_iterator cend() const;
  reverse_iterator rbegin();
  const_reverse_iterator rbegin() const;
  const_reverse_iterator crbegin() const;
  reverse_iterator rend();
  const_reverse_iterator rend() const;
  const_reverse_iterator crend() const;

private:
  detail::LinkedHm<K, V, H, E> hm_;
};
