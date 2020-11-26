// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#pragma once
#include "infra/LinkedHashMap.h"
#include "infra/Log.h"

#define LN(x) ((LinkedNode<K, V> *)x)

namespace detail {

// linked node

template <typename K, typename V>
LinkedNode<K, V>::LinkedNode(const std::pair<const K, V> &value)
    : prev(nullptr), next(nullptr), traverse_prev(nullptr),
      traverse_next(nullptr), value(value) {}

template <typename K, typename V> LinkedNode<K, V>::~LinkedNode() {
  prev = nullptr;
  next = nullptr;
  traverse_prev = nullptr;
  traverse_next = nullptr;
}

template <typename K, typename V> const K &LinkedNode<K, V>::key() const {
  return value.first;
}

// linked list

template <typename K, typename V>
LinkedList<K, V>::LinkedList()
    : prev(LN(this)), next(LN(this)), traverse_prev(LN(this)),
      traverse_next(LN(this)) {}

template <typename K, typename V> LinkedList<K, V>::~LinkedList() {
  prev = nullptr;
  next = nullptr;
  traverse_prev = nullptr;
  traverse_next = nullptr;
}

template <typename K, typename V> LinkedNode<K, V> *&LinkedList<K, V>::head() {
  return next;
}

template <typename K, typename V>
const LinkedNode<K, V> *LinkedList<K, V>::head() const {
  return next;
}

template <typename K, typename V> LinkedNode<K, V> *&LinkedList<K, V>::tail() {
  return prev;
}

template <typename K, typename V>
const LinkedNode<K, V> *LinkedList<K, V>::tail() const {
  return prev;
}

template <typename K, typename V>
void LinkedList<K, V>::insertTail(LinkedNode<K, V> *e) {
  e->prev = prev;
  e->next = LN(this);
  prev->next = e;
  prev = e;
}

template <typename K, typename V>
void LinkedList<K, V>::insertHead(LinkedNode<K, V> *e) {
  e->next = next;
  e->prev = LN(this);
  next->prev = e;
  next = e;
}

template <typename K, typename V>
LinkedNode<K, V> *LinkedList<K, V>::removeTail() {
  if (empty()) {
    return LN(this);
  }
  LinkedNode<K, V> *e = prev;
  prev->prev->next = LN(this);
  prev = prev->prev;
  return e;
}

template <typename K, typename V>
LinkedNode<K, V> *LinkedList<K, V>::removeHead() {
  if (empty()) {
    return LN(this);
  }
  LinkedNode<K, V> *e = next;
  next->next->prev = LN(this);
  next = next->next;
  return e;
}

template <typename K, typename V>
void LinkedList<K, V>::remove(LinkedNode<K, V> *e) {
  if (empty()) {
    return;
  }
  e->prev->next = e->next;
  e->next->prev = e->prev;
  e->prev = nullptr;
  e->next = nullptr;
}

template <typename K, typename V>
void LinkedList<K, V>::traverse_insert(LinkedNode<K, V> *e) {
  e->traverse_prev = traverse_prev;
  e->traverse_next = LN(this);
  traverse_prev->traverse_next = e;
  traverse_prev = e;
}

template <typename K, typename V>
void LinkedList<K, V>::traverse_remove(LinkedNode<K, V> *e) {
  if (traverse_empty()) {
    return;
  }
  e->traverse_prev->traverse_next = e->traverse_next;
  e->traverse_next->traverse_prev = e->traverse_prev;
  e->traverse_prev = nullptr;
  e->traverse_next = nullptr;
}

template <typename K, typename V> bool LinkedList<K, V>::empty() const {
  LOG_ASSERT(prev && next, "prev {}, next {} is null", (void *)prev,
             (void *)next);
  LOG_ASSERT((prev == LN(this) && next == LN(this)) ||
                 (prev != LN(this) && next != LN(this)),
             "this: {}, prev: {}, next: {}", (void *)this, (void *)prev,
             (void *)next);
  return prev == LN(this) && next == LN(this);
}

template <typename K, typename V>
bool LinkedList<K, V>::traverse_empty() const {
  LOG_ASSERT(traverse_prev && traverse_next,
             "traverse_prev {}, traverse_next {} is null", (void *)prev,
             (void *)next);
  LOG_ASSERT((traverse_prev == LN(this) && traverse_next == LN(this)) ||
                 (traverse_prev != LN(this) && traverse_next != LN(this)),
             "this: {}, traverse_prev: {}, traverse_next: {}", (void *)this,
             (void *)traverse_prev, (void *)traverse_next);
  return traverse_prev == LN(this) && traverse_next == LN(this);
}

// { linked iterator start

template <typename K, typename V, typename NodePtr>
LinkedIterator<K, V, NodePtr>::LinkedIterator(NodePtr node) : node_(node) {}

// convert between other iterators

template <typename K, typename V, typename NodePtr>
LinkedIterator<K, V, NodePtr>::LinkedIterator(
    const LinkedIterator<K, V, NodePtr> &other)
    : node_(other.node_) {}

template <typename K, typename V, typename NodePtr>
LinkedIterator<K, V, NodePtr>::LinkedIterator(
    const LinkedConstIterator<K, V, NodePtr> &other)
    : node_(other.node_) {}

template <typename K, typename V, typename NodePtr>
LinkedIterator<K, V, NodePtr>::LinkedIterator(
    const LinkedReverseIterator<K, V, NodePtr> &other)
    : node_(other.node_) {}

template <typename K, typename V, typename NodePtr>
LinkedIterator<K, V, NodePtr>::LinkedIterator(
    const LinkedConstReverseIterator<K, V, NodePtr> &other)
    : node_(other.node_) {}

template <typename K, typename V, typename NodePtr>
LinkedIterator<K, V, NodePtr> &LinkedIterator<K, V, NodePtr>::operator=(
    const LinkedIterator<K, V, NodePtr> &other) {
  if (this == &other) {
    return *this;
  }
  node_ = other.node_;
  return *this;
}

template <typename K, typename V, typename NodePtr>
LinkedIterator<K, V, NodePtr> &LinkedIterator<K, V, NodePtr>::operator=(
    const LinkedConstIterator<K, V, NodePtr> &other) {
  if (this == &other) {
    return *this;
  }
  node_ = other.node_;
  return *this;
}

template <typename K, typename V, typename NodePtr>
LinkedIterator<K, V, NodePtr> &LinkedIterator<K, V, NodePtr>::operator=(
    const LinkedReverseIterator<K, V, NodePtr> &other) {
  if (this == &other) {
    return *this;
  }
  node_ = other.node_;
  return *this;
}

template <typename K, typename V, typename NodePtr>
LinkedIterator<K, V, NodePtr> &LinkedIterator<K, V, NodePtr>::operator=(
    const LinkedConstReverseIterator<K, V, NodePtr> &other) {
  if (this == &other) {
    return *this;
  }
  node_ = other.node_;
  return *this;
}

template <typename K, typename V, typename NodePtr>
LinkedIterator<K, V, NodePtr> &LinkedIterator<K, V, NodePtr>::operator++() {
  node_ = node_->traverse_next;
  return *this;
}

template <typename K, typename V, typename NodePtr>
LinkedIterator<K, V, NodePtr> &LinkedIterator<K, V, NodePtr>::operator--() {
  node_ = node_->traverse_prev;
  return *this;
}

template <typename K, typename V, typename NodePtr>
LinkedIterator<K, V, NodePtr> LinkedIterator<K, V, NodePtr>::operator++(int) {
  LinkedIterator<K, V, NodePtr> save = *this;
  node_ = node_->traverse_next;
  return save;
}

template <typename K, typename V, typename NodePtr>
LinkedIterator<K, V, NodePtr> LinkedIterator<K, V, NodePtr>::operator--(int) {
  LinkedIterator<K, V, NodePtr> save = *this;
  node_ = node_->traverse_prev;
  return save;
}

template <typename K, typename V, typename NodePtr>
bool LinkedIterator<K, V, NodePtr>::operator==(
    const LinkedIterator<K, V, NodePtr> &other) const {
  return node_ == other.node_;
}

template <typename K, typename V, typename NodePtr>
bool LinkedIterator<K, V, NodePtr>::operator!=(
    const LinkedIterator<K, V, NodePtr> &other) const {
  return node_ != other.node_;
}

template <typename K, typename V, typename NodePtr>
bool LinkedIterator<K, V, NodePtr>::operator!() const {
  return !node_;
}

template <typename K, typename V, typename NodePtr>
std::pair<const K, V> &LinkedIterator<K, V, NodePtr>::operator*() {
  return node_->value;
}

/* template <typename K, typename V, typename NodePtr> */
/* const std::pair<const K, V> &LinkedIterator<K, V, NodePtr>::operator*() const
 * { */
/*   return (const std::pair<const K, V> &)node_->value; */
/* } */

template <typename K, typename V, typename NodePtr>
std::pair<const K, V> *LinkedIterator<K, V, NodePtr>::operator->() {
  return &(node_->value);
}

/* template <typename K, typename V, typename NodePtr> */
/* const std::pair<const K, V> *LinkedIterator<K, V, NodePtr>::operator->()
 * const { */
/*   return (const std::pair<const K, V> *)&(node_->value); */
/* } */

template <typename K, typename V, typename NodePtr>
Cowstr LinkedIterator<K, V, NodePtr>::str() const {
  return fmt::format("[@LinkedIterator node_:{}]", (void *)node_);
}

// } linked iterator end

// { linked const iterator start

template <typename K, typename V, typename NodePtr>
LinkedConstIterator<K, V, NodePtr>::LinkedConstIterator(NodePtr node)
    : node_(node) {}

template <typename K, typename V, typename NodePtr>
LinkedConstIterator<K, V, NodePtr>::LinkedConstIterator(
    const LinkedIterator<K, V, NodePtr> &other)
    : node_(other.node_) {}

template <typename K, typename V, typename NodePtr>
LinkedConstIterator<K, V, NodePtr>::LinkedConstIterator(
    const LinkedConstIterator<K, V, NodePtr> &other)
    : node_(other.node_) {}

template <typename K, typename V, typename NodePtr>
LinkedConstIterator<K, V, NodePtr>::LinkedConstIterator(
    const LinkedReverseIterator<K, V, NodePtr> &other)
    : node_(other.node_) {}

template <typename K, typename V, typename NodePtr>
LinkedConstIterator<K, V, NodePtr>::LinkedConstIterator(
    const LinkedConstReverseIterator<K, V, NodePtr> &other)
    : node_(other.node_) {}

template <typename K, typename V, typename NodePtr>
LinkedConstIterator<K, V, NodePtr> &
LinkedConstIterator<K, V, NodePtr>::operator=(
    const LinkedIterator<K, V, NodePtr> &other) {
  if (this == &other) {
    return *this;
  }
  node_ = other.node_;
  return *this;
}

template <typename K, typename V, typename NodePtr>
LinkedConstIterator<K, V, NodePtr> &
LinkedConstIterator<K, V, NodePtr>::operator=(
    const LinkedConstIterator<K, V, NodePtr> &other) {
  if (this == &other) {
    return *this;
  }
  node_ = other.node_;
  return *this;
}

template <typename K, typename V, typename NodePtr>
LinkedConstIterator<K, V, NodePtr> &
LinkedConstIterator<K, V, NodePtr>::operator=(
    const LinkedReverseIterator<K, V, NodePtr> &other) {
  if (this == &other) {
    return *this;
  }
  node_ = other.node_;
  return *this;
}

template <typename K, typename V, typename NodePtr>
LinkedConstIterator<K, V, NodePtr> &
LinkedConstIterator<K, V, NodePtr>::operator=(
    const LinkedConstReverseIterator<K, V, NodePtr> &other) {
  if (this == &other) {
    return *this;
  }
  node_ = other.node_;
  return *this;
}

template <typename K, typename V, typename NodePtr>
LinkedConstIterator<K, V, NodePtr> &
LinkedConstIterator<K, V, NodePtr>::operator++() {
  node_ = node_->traverse_next;
  return *this;
}

template <typename K, typename V, typename NodePtr>
LinkedConstIterator<K, V, NodePtr> &
LinkedConstIterator<K, V, NodePtr>::operator--() {
  node_ = node_->traverse_prev;
  return *this;
}

template <typename K, typename V, typename NodePtr>
LinkedConstIterator<K, V, NodePtr>
LinkedConstIterator<K, V, NodePtr>::operator++(int) {
  LinkedConstIterator<K, V, NodePtr> save = *this;
  node_ = node_->traverse_next;
  return save;
}

template <typename K, typename V, typename NodePtr>
LinkedConstIterator<K, V, NodePtr>
LinkedConstIterator<K, V, NodePtr>::operator--(int) {
  LinkedConstIterator<K, V, NodePtr> save = *this;
  node_ = node_->traverse_prev;
  return save;
}

template <typename K, typename V, typename NodePtr>
bool LinkedConstIterator<K, V, NodePtr>::operator==(
    const LinkedConstIterator<K, V, NodePtr> &other) const {
  return node_ == other.node_;
}

template <typename K, typename V, typename NodePtr>
bool LinkedConstIterator<K, V, NodePtr>::operator!=(
    const LinkedConstIterator<K, V, NodePtr> &other) const {
  return node_ != other.node_;
}

template <typename K, typename V, typename NodePtr>
bool LinkedConstIterator<K, V, NodePtr>::operator!() const {
  return !node_;
}

template <typename K, typename V, typename NodePtr>
const std::pair<const K, V> &
LinkedConstIterator<K, V, NodePtr>::operator*() const {
  return node_->value;
}

template <typename K, typename V, typename NodePtr>
const std::pair<const K, V> *
LinkedConstIterator<K, V, NodePtr>::operator->() const {
  return &(node_->value);
}

template <typename K, typename V, typename NodePtr>
Cowstr LinkedConstIterator<K, V, NodePtr>::str() const {
  return fmt::format("[@LinkedConstIterator node_:{}]", (void *)node_);
}

// { linked const iterator end

// { linked reverse iterator start

template <typename K, typename V, typename NodePtr>
LinkedReverseIterator<K, V, NodePtr>::LinkedReverseIterator(NodePtr node)
    : node_(node) {}

template <typename K, typename V, typename NodePtr>
LinkedReverseIterator<K, V, NodePtr>::LinkedReverseIterator(
    const LinkedIterator<K, V, NodePtr> &other)
    : node_(other.node_) {}

template <typename K, typename V, typename NodePtr>
LinkedReverseIterator<K, V, NodePtr>::LinkedReverseIterator(
    const LinkedConstIterator<K, V, NodePtr> &other)
    : node_(other.node_) {}

template <typename K, typename V, typename NodePtr>
LinkedReverseIterator<K, V, NodePtr>::LinkedReverseIterator(
    const LinkedReverseIterator<K, V, NodePtr> &other)
    : node_(other.node_) {}

template <typename K, typename V, typename NodePtr>
LinkedReverseIterator<K, V, NodePtr>::LinkedReverseIterator(
    const LinkedConstReverseIterator<K, V, NodePtr> &other)
    : node_(other.node_) {}

template <typename K, typename V, typename NodePtr>
LinkedReverseIterator<K, V, NodePtr> &
LinkedReverseIterator<K, V, NodePtr>::operator=(
    const LinkedIterator<K, V, NodePtr> &other) {
  if (this == &other) {
    return *this;
  }
  node_ = other.node_;
  return *this;
}

template <typename K, typename V, typename NodePtr>
LinkedReverseIterator<K, V, NodePtr> &
LinkedReverseIterator<K, V, NodePtr>::operator=(
    const LinkedConstIterator<K, V, NodePtr> &other) {
  if (this == &other) {
    return *this;
  }
  node_ = other.node_;
  return *this;
}

template <typename K, typename V, typename NodePtr>
LinkedReverseIterator<K, V, NodePtr> &
LinkedReverseIterator<K, V, NodePtr>::operator=(
    const LinkedReverseIterator<K, V, NodePtr> &other) {
  if (this == &other) {
    return *this;
  }
  node_ = other.node_;
  return *this;
}

template <typename K, typename V, typename NodePtr>
LinkedReverseIterator<K, V, NodePtr> &
LinkedReverseIterator<K, V, NodePtr>::operator=(
    const LinkedConstReverseIterator<K, V, NodePtr> &other) {
  if (this == &other) {
    return *this;
  }
  node_ = other.node_;
  return *this;
}

template <typename K, typename V, typename NodePtr>
LinkedReverseIterator<K, V, NodePtr> &
LinkedReverseIterator<K, V, NodePtr>::operator++() {
  node_ = node_->traverse_prev;
  return *this;
}

template <typename K, typename V, typename NodePtr>
LinkedReverseIterator<K, V, NodePtr> &
LinkedReverseIterator<K, V, NodePtr>::operator--() {
  node_ = node_->traverse_next;
  return *this;
}

template <typename K, typename V, typename NodePtr>
LinkedReverseIterator<K, V, NodePtr>
LinkedReverseIterator<K, V, NodePtr>::operator++(int) {
  LinkedReverseIterator<K, V, NodePtr> save = *this;
  node_ = node_->traverse_prev;
  return save;
}

template <typename K, typename V, typename NodePtr>
LinkedReverseIterator<K, V, NodePtr>
LinkedReverseIterator<K, V, NodePtr>::operator--(int) {
  LinkedReverseIterator<K, V, NodePtr> save = *this;
  node_ = node_->traverse_next;
  return save;
}

template <typename K, typename V, typename NodePtr>
bool LinkedReverseIterator<K, V, NodePtr>::operator==(
    const LinkedReverseIterator<K, V, NodePtr> &other) const {
  return node_ == other.node_;
}

template <typename K, typename V, typename NodePtr>
bool LinkedReverseIterator<K, V, NodePtr>::operator!=(
    const LinkedReverseIterator<K, V, NodePtr> &other) const {
  return node_ != other.node_;
}

template <typename K, typename V, typename NodePtr>
bool LinkedReverseIterator<K, V, NodePtr>::operator!() const {
  return !node_;
}

template <typename K, typename V, typename NodePtr>
std::pair<const K, V> &LinkedReverseIterator<K, V, NodePtr>::operator*() {
  return node_->value;
}

template <typename K, typename V, typename NodePtr>
std::pair<const K, V> *LinkedReverseIterator<K, V, NodePtr>::operator->() {
  return &(node_->value);
}

template <typename K, typename V, typename NodePtr>
Cowstr LinkedReverseIterator<K, V, NodePtr>::str() const {
  return fmt::format("[@LinkedReverseIterator node_:{}]", (void *)node_);
}

// } linked reverse iterator end

// { linked const reverse iterator start

template <typename K, typename V, typename NodePtr>
LinkedConstReverseIterator<K, V, NodePtr>::LinkedConstReverseIterator(
    NodePtr node)
    : node_(node) {}

template <typename K, typename V, typename NodePtr>
LinkedConstReverseIterator<K, V, NodePtr>::LinkedConstReverseIterator(
    const LinkedIterator<K, V, NodePtr> &other)
    : node_(other.node_) {}

template <typename K, typename V, typename NodePtr>
LinkedConstReverseIterator<K, V, NodePtr>::LinkedConstReverseIterator(
    const LinkedConstIterator<K, V, NodePtr> &other)
    : node_(other.node_) {}

template <typename K, typename V, typename NodePtr>
LinkedConstReverseIterator<K, V, NodePtr>::LinkedConstReverseIterator(
    const LinkedReverseIterator<K, V, NodePtr> &other)
    : node_(other.node_) {}

template <typename K, typename V, typename NodePtr>
LinkedConstReverseIterator<K, V, NodePtr>::LinkedConstReverseIterator(
    const LinkedConstReverseIterator<K, V, NodePtr> &other)
    : node_(other.node_) {}

template <typename K, typename V, typename NodePtr>
LinkedConstReverseIterator<K, V, NodePtr> &
LinkedConstReverseIterator<K, V, NodePtr>::operator=(
    const LinkedIterator<K, V, NodePtr> &other) {
  if (this == &other) {
    return *this;
  }
  node_ = other.node_;
  return *this;
}

template <typename K, typename V, typename NodePtr>
LinkedConstReverseIterator<K, V, NodePtr> &
LinkedConstReverseIterator<K, V, NodePtr>::operator=(
    const LinkedConstIterator<K, V, NodePtr> &other) {
  if (this == &other) {
    return *this;
  }
  node_ = other.node_;
  return *this;
}

template <typename K, typename V, typename NodePtr>
LinkedConstReverseIterator<K, V, NodePtr> &
LinkedConstReverseIterator<K, V, NodePtr>::operator=(
    const LinkedReverseIterator<K, V, NodePtr> &other) {
  if (this == &other) {
    return *this;
  }
  node_ = other.node_;
  return *this;
}

template <typename K, typename V, typename NodePtr>
LinkedConstReverseIterator<K, V, NodePtr> &
LinkedConstReverseIterator<K, V, NodePtr>::operator=(
    const LinkedConstReverseIterator<K, V, NodePtr> &other) {
  if (this == &other) {
    return *this;
  }
  node_ = other.node_;
  return *this;
}

template <typename K, typename V, typename NodePtr>
LinkedConstReverseIterator<K, V, NodePtr> &
LinkedConstReverseIterator<K, V, NodePtr>::operator++() {
  node_ = node_->traverse_prev;
  return *this;
}

template <typename K, typename V, typename NodePtr>
LinkedConstReverseIterator<K, V, NodePtr> &
LinkedConstReverseIterator<K, V, NodePtr>::operator--() {
  node_ = node_->traverse_next;
  return *this;
}

template <typename K, typename V, typename NodePtr>
LinkedConstReverseIterator<K, V, NodePtr>
LinkedConstReverseIterator<K, V, NodePtr>::operator++(int) {
  LinkedConstReverseIterator<K, V, NodePtr> save = *this;
  node_ = node_->traverse_prev;
  return save;
}

template <typename K, typename V, typename NodePtr>
LinkedConstReverseIterator<K, V, NodePtr>
LinkedConstReverseIterator<K, V, NodePtr>::operator--(int) {
  LinkedConstReverseIterator<K, V, NodePtr> save = *this;
  node_ = node_->traverse_next;
  return save;
}

template <typename K, typename V, typename NodePtr>
bool LinkedConstReverseIterator<K, V, NodePtr>::operator==(
    const LinkedConstReverseIterator<K, V, NodePtr> &other) const {
  return node_ == other.node_;
}

template <typename K, typename V, typename NodePtr>
bool LinkedConstReverseIterator<K, V, NodePtr>::operator!=(
    const LinkedConstReverseIterator<K, V, NodePtr> &other) const {
  return node_ != other.node_;
}

template <typename K, typename V, typename NodePtr>
bool LinkedConstReverseIterator<K, V, NodePtr>::operator!() const {
  return !node_;
}

template <typename K, typename V, typename NodePtr>
const std::pair<const K, V> &
LinkedConstReverseIterator<K, V, NodePtr>::operator*() const {
  return node_->value;
}

template <typename K, typename V, typename NodePtr>
const std::pair<const K, V> *
LinkedConstReverseIterator<K, V, NodePtr>::operator->() const {
  return &(node_->value);
}

template <typename K, typename V, typename NodePtr>
Cowstr LinkedConstReverseIterator<K, V, NodePtr>::str() const {
  return fmt::format("[@LinkedConstReverseIterator node_:{}]", (void *)node_);
}

// } linked const reverse iterator end

// linked hashmap

template <typename K, typename V, typename H, typename E>
LinkedHm<K, V, H, E>::LinkedHm()
    : hasher_(), equal_(), head_(), ht_(nullptr), count_(nullptr), bucket_(0),
      size_(0) {}

template <typename K, typename V, typename H, typename E>
LinkedHm<K, V, H, E>::LinkedHm(int bucket) : LinkedHm() {
  LOG_ASSERT(bucket > 0, "bucket {} > 0", bucket);
  extend(bucket);
}

template <typename K, typename V, typename H, typename E>
LinkedHm<K, V, H, E>::~LinkedHm() {
  release();
}

template <typename K, typename V, typename H, typename E>
bool LinkedHm<K, V, H, E>::empty() const {
  LOG_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  LOG_ASSERT((ht_ && count_) || (!ht_ && !count_ && size_ == 0 && bucket_ == 0),
             "ht_ {} null, count_ {} null, size_: {}, bucket_: {}", (void *)ht_,
             (void *)count_, size_, bucket_);
  return size_ == 0;
}

template <typename K, typename V, typename H, typename E>
int LinkedHm<K, V, H, E>::size() const {
  return size_;
}

template <typename K, typename V, typename H, typename E>
int LinkedHm<K, V, H, E>::bucket() const {
  return bucket_;
}

template <typename K, typename V, typename H, typename E>
double LinkedHm<K, V, H, E>::loadFactor() const {
  return empty() ? 0.0 : ((double)size_ / (double)bucket_);
}

template <typename K, typename V, typename H, typename E>
void LinkedHm<K, V, H, E>::clear() {
  if (!empty()) {
    for (int i = 0; i < bucket_; i++) {
      destroyList(i);
    }
  }
  size_ = 0;
}

template <typename K, typename V, typename H, typename E>
void LinkedHm<K, V, H, E>::release() {
  if (!empty()) {
    for (int i = 0; i < bucket_; i++) {
      destroyList(i);
    }
    LOG_ASSERT(head_.traverse_empty(), "head_#traverse_empty: {}",
               head_.traverse_empty());
    delete[] ht_;
    delete[] count_;
    ht_ = nullptr;
    count_ = nullptr;
  }
  bucket_ = 0;
  size_ = 0;
}

template <typename K, typename V, typename H, typename E>
typename LinkedHm<K, V, H, E>::iterator
LinkedHm<K, V, H, E>::insert(const std::pair<const K, V> &value) {
  extend(2 * bucket_);
  LinkedNode<K, V> *e = new LinkedNode<K, V>(value);
  int b = getBucket(value.first, bucket_);
  ht_[b].insertHead(e);
  head_.traverse_insert(e);
  LOG_ASSERT(count_[b] >= 0, "count_[{}] {} >= 0", b, count_[b]);
  LOG_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  ++count_[b];
  ++size_;
  return LinkedHm<K, V, H, E>::iterator(e);
}

template <typename K, typename V, typename H, typename E>
typename LinkedHm<K, V, H, E>::iterator
LinkedHm<K, V, H, E>::insertOrAssign(const std::pair<const K, V> &value) {
  extend(2 * bucket_);
  iterator position = find(value.first);
  if (position == end()) {
    LinkedNode<K, V> *e = new LinkedNode<K, V>(value);
    int b = getBucket(value.first, bucket_);
    ht_[b].insertHead(e);
    head_.traverse_insert(e);
    LOG_ASSERT(count_[b] >= 0, "count_[{}] {} >= 0", b, count_[b]);
    LOG_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
    ++count_[b];
    ++size_;
    return LinkedHm<K, V, H, E>::iterator(e);
  } else {
    position->second = value.second;
    return position;
  }
}

template <typename K, typename V, typename H, typename E>
typename LinkedHm<K, V, H, E>::iterator
LinkedHm<K, V, H, E>::find(const K &key) {
  if (empty()) {
    return end();
  }
  int b = getBucket(key, bucket_);
  LinkedNode<K, V> *e = ht_[b].next;
  while (e != LN(&ht_[b])) {
    if (equal_(e->key(), key)) {
      return LinkedHm<K, V, H, E>::iterator(e);
    }
    e = e->next;
  }
  return end();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHm<K, V, H, E>::const_iterator
LinkedHm<K, V, H, E>::find(const K &key) const {
  if (empty()) {
    return end();
  }
  int b = getBucket(key, bucket_);
  LinkedNode<K, V> *e = ht_[b].next;
  while (e != LN(&ht_[b])) {
    if (equal_(e->key(), key)) {
      return LinkedHm<K, V, H, E>::const_iterator(e);
    }
    e = e->next;
  }
  return end();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHm<K, V, H, E>::iterator
LinkedHm<K, V, H, E>::remove(typename LinkedHm<K, V, H, E>::iterator position) {
  if (empty()) {
    return end();
  }
  int b = getBucket(position->first, bucket_);
  LinkedNode<K, V> *e = ht_[b].next;
  while (e != LN(&ht_[b])) {
    if (equal_(e->key(), position->first)) {
      LinkedHm<K, V, H, E>::iterator next(e->traverse_next);
      ht_[b].remove(e);
      head_.traverse_remove(e);
      delete e;
      --count_[b];
      --size_;
      LOG_ASSERT(count_[b] >= 0, "count_[{}] {} >= 0", b, count_[b]);
      LOG_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
      return next;
    }
    e = e->next;
  }
  return end();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHm<K, V, H, E>::iterator LinkedHm<K, V, H, E>::remove(
    typename LinkedHm<K, V, H, E>::const_iterator position) {
  if (empty()) {
    return end();
  }
  int b = getBucket(position->first, bucket_);
  LinkedNode<K, V> *e = ht_[b].next;
  while (e != LN(&ht_[b])) {
    if (equal_(e->key(), position->first)) {
      LinkedHm<K, V, H, E>::iterator next(e->traverse_next);
      ht_[b].remove(e);
      head_.traverse_remove(e);
      delete e;
      --count_[b];
      --size_;
      LOG_ASSERT(count_[b] >= 0, "count_[{}] {} >= 0", b, count_[b]);
      LOG_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
      return next;
    }
    e = e->next;
  }
  return end();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHm<K, V, H, E>::iterator LinkedHm<K, V, H, E>::begin() {
  return typename LinkedHm<K, V, H, E>::iterator(LN(head_.traverse_next));
}

template <typename K, typename V, typename H, typename E>
typename LinkedHm<K, V, H, E>::const_iterator
LinkedHm<K, V, H, E>::begin() const {
  return typename LinkedHm<K, V, H, E>::const_iterator(LN(head_.traverse_next));
}

template <typename K, typename V, typename H, typename E>
typename LinkedHm<K, V, H, E>::const_iterator
LinkedHm<K, V, H, E>::cbegin() const {
  return typename LinkedHm<K, V, H, E>::const_iterator(LN(head_.traverse_next));
}

template <typename K, typename V, typename H, typename E>
typename LinkedHm<K, V, H, E>::iterator LinkedHm<K, V, H, E>::end() {
  return typename LinkedHm<K, V, H, E>::iterator(LN(&head_));
}

template <typename K, typename V, typename H, typename E>
typename LinkedHm<K, V, H, E>::const_iterator
LinkedHm<K, V, H, E>::end() const {
  return typename LinkedHm<K, V, H, E>::const_iterator(LN(&head_));
}

template <typename K, typename V, typename H, typename E>
typename LinkedHm<K, V, H, E>::const_iterator
LinkedHm<K, V, H, E>::cend() const {
  return typename LinkedHm<K, V, H, E>::const_iterator(LN(&head_));
}

template <typename K, typename V, typename H, typename E>
typename LinkedHm<K, V, H, E>::reverse_iterator LinkedHm<K, V, H, E>::rbegin() {
  return
      typename LinkedHm<K, V, H, E>::reverse_iterator(LN(head_.traverse_prev));
}

template <typename K, typename V, typename H, typename E>
typename LinkedHm<K, V, H, E>::const_reverse_iterator
LinkedHm<K, V, H, E>::rbegin() const {
  return typename LinkedHm<K, V, H, E>::const_reverse_iterator(
      LN(head_.traverse_prev));
}

template <typename K, typename V, typename H, typename E>
typename LinkedHm<K, V, H, E>::const_reverse_iterator
LinkedHm<K, V, H, E>::crbegin() const {
  return typename LinkedHm<K, V, H, E>::const_reverse_iterator(
      LN(head_.traverse_prev));
}

template <typename K, typename V, typename H, typename E>
typename LinkedHm<K, V, H, E>::reverse_iterator LinkedHm<K, V, H, E>::rend() {
  return typename LinkedHm<K, V, H, E>::reverse_iterator(LN(&head_));
}

template <typename K, typename V, typename H, typename E>
typename LinkedHm<K, V, H, E>::const_reverse_iterator
LinkedHm<K, V, H, E>::rend() const {
  return typename LinkedHm<K, V, H, E>::const_reverse_iterator(LN(&head_));
}

template <typename K, typename V, typename H, typename E>
typename LinkedHm<K, V, H, E>::const_reverse_iterator
LinkedHm<K, V, H, E>::crend() const {
  return typename LinkedHm<K, V, H, E>::const_reverse_iterator(LN(&head_));
}

template <typename K, typename V, typename H, typename E>
void LinkedHm<K, V, H, E>::extend(int n) {
  LOG_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  LOG_ASSERT(bucket_ >= 0, "bucket_ {} >= 0", bucket_);
  if (!empty() && loadFactor() < 4.0) {
    return;
  }
  n = alignBucket(n);
  LOG_ASSERT(n > bucket_, "n {} > bucket_ {}", n, bucket_);
  LinkedList<K, V> *newHt = new LinkedList<K, V>[n];
  int *newCount = new int[n];
  LOG_ASSERT(newHt, "newHt is null");
  LOG_ASSERT(newCount, "newCount is null");
  std::memset(newCount, 0, n * sizeof(int));

  // first try rehash old hashmap
  if (!empty()) {
    for (int i = 0; i < bucket_; i++) {
      while (count_[i] > 0) {
        LinkedNode<K, V> *e = ht_[i].removeHead();
        int b = getBucket(e->key(), n);
        newHt[b].insertHead(e);
        --count_[i];
        ++newCount[b];
      }
    }
    if (ht_) {
      delete[] ht_;
      ht_ = nullptr;
    }
    if (count_) {
      delete[] count_;
      count_ = nullptr;
    }
  }

  // then assign new hashmap
  ht_ = newHt;
  count_ = newCount;
  bucket_ = n;
}

template <typename K, typename V, typename H, typename E>
void LinkedHm<K, V, H, E>::destroyList(int i) {
  while (ht_[i].next != LN(&ht_[i])) {
    LinkedNode<K, V> *e = ht_[i].removeTail();
    head_.traverse_remove(e);
    delete e;
    --count_[i];
    --size_;
    LOG_ASSERT(count_[i] >= 0, "count_[{}] {} >= 0", i, count_[i]);
    LOG_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  }
  LOG_ASSERT(ht_[i].empty(), "ht_[{}]#empty: {}", i, ht_[i].empty());
  LOG_ASSERT(count_[i] == 0, "count_[{}] {} == 0", i, count_[i]);
}

template <typename K, typename V, typename H, typename E>
int LinkedHm<K, V, H, E>::alignBucket(int n) const {
  if (n <= 0)
    return 8;
  if (n < (std::numeric_limits<int>::max() - 7) / 2)
    return (n + 7) / 8 * 8;
  return std::numeric_limits<int>::max();
}

template <typename K, typename V, typename H, typename E>
int LinkedHm<K, V, H, E>::getBucket(const K &key, int bucket) const {
  return int((size_t)hasher_(key) % (size_t)bucket);
}

} // namespace detail

template <typename K, typename V, typename H, typename E>
LinkedHashMap<K, V, H, E>::LinkedHashMap() : hm_() {}

template <typename K, typename V, typename H, typename E>
LinkedHashMap<K, V, H, E>::LinkedHashMap(int bucket) : hm_(bucket) {}

template <typename K, typename V, typename H, typename E>
LinkedHashMap<K, V, H, E>::~LinkedHashMap() {}

template <typename K, typename V, typename H, typename E>
bool LinkedHashMap<K, V, H, E>::empty() const {
  return hm_.empty();
}

template <typename K, typename V, typename H, typename E>
int LinkedHashMap<K, V, H, E>::size() const {
  return hm_.size();
}

template <typename K, typename V, typename H, typename E>
int LinkedHashMap<K, V, H, E>::bucket() const {
  return hm_.bucket();
}

template <typename K, typename V, typename H, typename E>
double LinkedHashMap<K, V, H, E>::loadFactor() const {
  return hm_.loadFactor();
}

template <typename K, typename V, typename H, typename E>
void LinkedHashMap<K, V, H, E>::clear() {
  hm_.clear();
}

template <typename K, typename V, typename H, typename E>
void LinkedHashMap<K, V, H, E>::release() {
  hm_.release();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::iterator
LinkedHashMap<K, V, H, E>::begin() {
  return hm_.begin();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::const_iterator
LinkedHashMap<K, V, H, E>::begin() const {
  return hm_.begin();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::const_iterator
LinkedHashMap<K, V, H, E>::cbegin() const {
  return hm_.cbegin();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::iterator LinkedHashMap<K, V, H, E>::end() {
  return hm_.end();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::const_iterator
LinkedHashMap<K, V, H, E>::end() const {
  return hm_.end();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::const_iterator
LinkedHashMap<K, V, H, E>::cend() const {
  return hm_.cend();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::reverse_iterator
LinkedHashMap<K, V, H, E>::rbegin() {
  return hm_.rbegin();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::const_reverse_iterator
LinkedHashMap<K, V, H, E>::rbegin() const {
  return hm_.rbegin();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::const_reverse_iterator
LinkedHashMap<K, V, H, E>::crbegin() const {
  return hm_.crbegin();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::reverse_iterator
LinkedHashMap<K, V, H, E>::rend() {
  return hm_.rend();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::const_reverse_iterator
LinkedHashMap<K, V, H, E>::rend() const {
  return hm_.rend();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::const_reverse_iterator
LinkedHashMap<K, V, H, E>::crend() const {
  return hm_.crend();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::iterator
LinkedHashMap<K, V, H, E>::insert(const K &key, const V &mapped) {
  return hm_.insert(std::pair<const K, V>(key, mapped));
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::iterator
LinkedHashMap<K, V, H, E>::insert(const std::pair<const K, V> &value) {
  return hm_.insert(value);
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::iterator
LinkedHashMap<K, V, H, E>::insertOrAssign(const K &key, const V &mapped) {
  return hm_.insertOrAssign(std::pair<const K, V>(key, mapped));
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::iterator
LinkedHashMap<K, V, H, E>::insertOrAssign(const std::pair<const K, V> &value) {
  return hm_.insertOrAssign(value);
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::iterator
LinkedHashMap<K, V, H, E>::remove(const K &key) {
  auto position = find(key);
  return hm_.remove(position);
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::iterator LinkedHashMap<K, V, H, E>::remove(
    LinkedHashMap<K, V, H, E>::iterator position) {
  return hm_.remove(position);
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::iterator LinkedHashMap<K, V, H, E>::remove(
    LinkedHashMap<K, V, H, E>::const_iterator position) {
  return hm_.remove(position);
}

template <typename K, typename V, typename H, typename E>
bool LinkedHashMap<K, V, H, E>::contains(const K &key) const {
  return find(key) != end();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::iterator
LinkedHashMap<K, V, H, E>::find(const K &key) {
  return hm_.find(key);
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::const_iterator
LinkedHashMap<K, V, H, E>::find(const K &key) const {
  return hm_.find(key);
}

template <typename K, typename V, typename H, typename E>
V &LinkedHashMap<K, V, H, E>::operator[](const K &key) {
  return hm_.find(key)->second;
}

template <typename K, typename V, typename H, typename E>
const V &LinkedHashMap<K, V, H, E>::operator[](const K &key) const {
  return hm_.find(key)->second;
}

#undef LN
