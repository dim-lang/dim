// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once

namespace detail {

template <class T> class LinkedNode {
public:
  LinkedNode(const T &value, LinkedNode *prev = nullptr,
             LinkedNode *next = nullptr);
  virtual ~LinkedNode() = default;

private:
  T value_;
  LinkedNode *prev_;
  LinkedNode *next_;
};

} // namespace detail

template <class U, class V> class LinkedHashMap {
public:
  LinkedHashMap();
  virtual ~LinkedHashMap() = default;

private:
  detail::LinkedNode<V> head_;
};
