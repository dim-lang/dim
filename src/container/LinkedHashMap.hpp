// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "LinkedHashMap.h"
#include "Log.h"
#include <cstdlib>

namespace detail {

template <typename K, typename V>
LinkedNode<K, V>::LinkedNode(LinkedNode<K, V> *prev, LinkedNode<K, V> *next,
                             LinkedNode<K, V> *tr_prev,
                             LinkedNode<K, V> *tr_next)
    : prev_(prev), next_(next), tr_prev_(tr_prev), tr_next_(tr_next) {}

template <typename K, typename V>
LinkedNode<K, V>::LinkedNode(const std::pair<const K, V> &value,
                             LinkedNode<K, V> *prev, LinkedNode<K, V> *next,
                             LinkedNode<K, V> *tr_prev,
                             LinkedNode<K, V> *tr_next)
    : value_(value), prev_(prev), next_(next), tr_prev_(tr_prev),
      tr_next_(tr_next) {}

template <typename K, typename V> LinkedNode<K, V>::~LinkedNode() {
  prev_ = nullptr;
  next_ = nullptr;
  tr_prev_ = nullptr;
  tr_next_ = nullptr;
}

template <typename K, typename V>
LinkedNode<K, V>::LinkedNode(const LinkedNode<K, V> &other)
    : value_(other.value_), prev_(other.prev_), next_(other.next_),
      tr_prev_(other.tr_prev_), tr_next_(other.tr_next_) {}

template <typename K, typename V>
LinkedNode<K, V> &LinkedNode<K, V>::operator=(const LinkedNode<K, V> &other) {
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

template <typename K, typename V> const K &LinkedNode<K, V>::key() const {
  return value_.first;
}

template <typename K, typename V> V &LinkedNode<K, V>::mapped() {
  return value_.second;
}

template <typename K, typename V> const V &LinkedNode<K, V>::mapped() const {
  return value_.second;
}

template <typename K, typename V>
const std::pair<const K, V> &LinkedNode<K, V>::value() const {
  return value_;
}

template <typename K, typename V>
std::pair<const K, V> &LinkedNode<K, V>::value() {
  return value_;
}

template <typename K, typename V> LinkedNode<K, V> *&LinkedNode<K, V>::prev() {
  return prev_;
}

template <typename K, typename V>
const LinkedNode<K, V> *LinkedNode<K, V>::prev() const {
  return prev_;
}

template <typename K, typename V> LinkedNode<K, V> *&LinkedNode<K, V>::next() {
  return next_;
}

template <typename K, typename V>
const LinkedNode<K, V> *LinkedNode<K, V>::next() const {
  return next_;
}

template <typename K, typename V>
LinkedNode<K, V> *&LinkedNode<K, V>::tr_prev() {
  return tr_prev_;
}

template <typename K, typename V>
const LinkedNode<K, V> *LinkedNode<K, V>::tr_prev() const {
  return tr_prev_;
}

template <typename K, typename V>
LinkedNode<K, V> *&LinkedNode<K, V>::tr_next() {
  return tr_next_;
}

template <typename K, typename V>
const LinkedNode<K, V> *LinkedNode<K, V>::tr_next() const {
  return tr_next_;
}

// list operation
template <typename K, typename V>
template <typename K1, typename V1>
void LinkedNode<K, V>::initializeList(LinkedNode<K1, V1> &l) {
  CASSERT(!l.prev(), "l#prev {} is null", (void *)l.prev());
  CASSERT(!l.next(), "l#next {} is null", (void *)l.next());
  l.prev() = &l;
  l.next() = &l;
  l.tr_prev() = &l;
  l.tr_next() = &l;
}

template <typename K, typename V> LinkedNode<K, V> *&LinkedNode<K, V>::head() {
  return next_;
}

template <typename K, typename V>
const LinkedNode<K, V> *LinkedNode<K, V>::head() const {
  return next_;
}

template <typename K, typename V> LinkedNode<K, V> *&LinkedNode<K, V>::tail() {
  return prev_;
}

template <typename K, typename V>
const LinkedNode<K, V> *LinkedNode<K, V>::tail() const {
  return prev_;
}

template <typename K, typename V>
void LinkedNode<K, V>::insertTail(LinkedNode<K, V> *e) {
  e->prev() = prev_;
  e->next() = this;
  prev_->next() = e;
  prev_ = e;
}

template <typename K, typename V>
void LinkedNode<K, V>::insertHead(LinkedNode<K, V> *e) {
  e->next() = next_;
  e->prev() = this;
  next_->prev() = e;
  next_ = e;
}

template <typename K, typename V>
LinkedNode<K, V> *LinkedNode<K, V>::removeTail() {
  if (isNull()) {
    return this;
  }
  LinkedNode<K, V> *e = prev_;
  prev_->prev()->next() = this;
  prev_ = prev_->prev();
  return e;
}

template <typename K, typename V>
LinkedNode<K, V> *LinkedNode<K, V>::removeHead() {
  if (isNull()) {
    return this;
  }
  LinkedNode<K, V> *e = next_;
  next_->next()->prev() = this;
  next_ = next_->next();
  return e;
}

template <typename K, typename V>
void LinkedNode<K, V>::remove(LinkedNode<K, V> *e) {
  if (isNull()) {
    return;
  }
  e->prev()->next() = e->next();
  e->next()->prev() = e->prev();
  e->prev() = nullptr;
  e->next() = nullptr;
}

template <typename K, typename V>
void LinkedNode<K, V>::tr_insertTail(LinkedNode<K, V> *e) {
  e->tr_prev() = tr_prev_;
  e->tr_next() = this;
  tr_prev_->tr_next() = e;
  tr_prev_ = e;
}

template <typename K, typename V>
void LinkedNode<K, V>::tr_remove(LinkedNode<K, V> *e) {
  if (isNull()) {
    return;
  }
  e->tr_prev()->tr_next() = e->tr_next();
  e->tr_next()->tr_prev() = e->tr_prev();
  e->tr_prev() = nullptr;
  e->tr_next() = nullptr;
}

template <typename K, typename V> bool LinkedNode<K, V>::isNull() const {
  CASSERT(prev_ && next_ && tr_prev_ && tr_next_,
          "prev_ {}, next_ {}, tr_prev_ {}, tr_next_ {} is null", (void *)prev_,
          (void *)next_, (void *)tr_prev_, (void *)tr_next_);
  CASSERT((prev_ == this && next_ == this && tr_prev_ == this &&
           tr_next_ == this) ||
              (prev_ != this && next_ != this && tr_prev_ != this &&
               tr_next_ != this),
          "this: {}, prev_: {}, next_: {}, tr_prev_: {}, tr_next_: {}",
          (void *)this, (void *)prev_, (void *)next_, (void *)tr_prev_,
          (void *)tr_next_);
  return prev_ == this && next_ == this && tr_prev_ == this && tr_next_ == this;
}

template <typename K, typename V, typename H, typename E>
LinkedHt<K, V, H, E>::LinkedHt()
    : hasher_(), equal_(), head_(), ht_(nullptr), count_(nullptr), bucket_(0),
      size_(0) {}

template <typename K, typename V, typename H, typename E>
LinkedHt<K, V, H, E>::LinkedHt(int bucket) : LinkedHt() {
  CASSERT(bucket > 0, "bucket {} > 0", bucket);
  extend(bucket);
}

template <typename K, typename V, typename H, typename E>
LinkedHt<K, V, H, E>::~LinkedHt() {
  release();
}

template <typename K, typename V, typename H, typename E>
bool LinkedHt<K, V, H, E>::empty() const {
  CASSERT(size_ >= 0, "size_ {} >= 0", size_);
  return size_ == 0;
}

template <typename K, typename V, typename H, typename E>
int LinkedHt<K, V, H, E>::size() const {
  return size_;
}

template <typename K, typename V, typename H, typename E>
int LinkedHt<K, V, H, E>::bucket() const {
  return bucket_;
}

template <typename K, typename V, typename H, typename E>
double LinkedHt<K, V, H, E>::loadFactor() const {
  return empty() ? 0.0 : ((double)size_ / (double)bucket_);
}

template <typename K, typename V, typename H, typename E>
void LinkedHt<K, V, H, E>::clear() {
  if (isNotNull()) {
    for (int i = 0; i < bucket_; i++) {
      destroyList(i);
    }
  }
  size_ = 0;
}

template <typename K, typename V, typename H, typename E>
void LinkedHt<K, V, H, E>::release() {
  if (isNotNull()) {
    for (int i = 0; i < bucket_; i++) {
      destroyList(i);
    }
    CASSERT(head_.isNull(), "head_#isNull: {}", head_.isNull());
    std::free(ht_);
    std::free(count_);
    ht_ = nullptr;
    count_ = nullptr;
  }
  bucket_ = 0;
  size_ = 0;
}

template <typename K, typename V, typename H, typename E>
void LinkedHt<K, V, H, E>::insert(const std::pair<const K, V> &value) {
  extend(bucket_ * 2);
  LinkedNode<K, V> *e = new LinkedNode<K, V>(value);
  int b = (int)hasher_(value.first) % bucket_;
  ht_[b].insertHead(e);
  CASSERT(count_[b] >= 0, "count_[{}] {} >= 0", b, count_[b]);
  CASSERT(size_ >= 0, "size_ {} >= 0", size_);
  ++count_[b];
  ++size_;
}

template <typename K, typename V, typename H, typename E>
int LinkedHt<K, V, H, E>::insertOrAssign(const std::pair<const K, V> &value) {
  extend(bucket_ * 2);
  LinkedIterator<K, V> position = find(value.first);
  if (position == end()) {
    LinkedNode<K, V> *e = new LinkedNode<K, V>(value);
    int b = (int)hasher_(value.first) % bucket_;
    ht_[b].insertHead(e);
    CASSERT(count_[b] >= 0, "count_[{}] {} >= 0", b, count_[b]);
    CASSERT(size_ >= 0, "size_ {} >= 0", size_);
    ++count_[b];
    ++size_;
    return 1;
  } else {
    position->second = value.second;
    return 0;
  }
}

template <typename K, typename V, typename H, typename E>
LinkedIterator<K, V> LinkedHt<K, V, H, E>::find(const K &key) const {
  int b = (int)hasher_(key) % bucket_;
  LinkedNode<K, V> *e = ht_[b].next();
  while (e != &ht_[b]) {
    if (equal_(e->key(), key)) {
      return LinkedIterator<K, V>(e);
    }
  }
  return end();
}

template <typename K, typename V, typename H, typename E>
int LinkedHt<K, V, H, E>::remove(LinkedIterator<K, V> position) {
  int b = (int)hasher_(position->first) % bucket_;
  LinkedNode<K, V> *e = ht_[b].next();
  while (e != &ht_[b]) {
    if (equal_(e->key(), position->first)) {
      ht_[b].remove(e);
      head_.tr_remove(e);
      delete e;
      --count_[b];
      --size_;
      CASSERT(count_[b] >= 0, "count_[{}] {} >= 0", b, count_[b]);
      CASSERT(size_ >= 0, "size_ {} >= 0", size_);
      return 0;
    }
  }
  return -1;
}

template <typename K, typename V, typename H, typename E>
LinkedIterator<K, V> LinkedHt<K, V, H, E>::begin() {
  return LinkedIterator<K, V>(head_.next());
}

template <typename K, typename V, typename H, typename E>
const LinkedIterator<K, V> LinkedHt<K, V, H, E>::begin() const {
  return LinkedIterator<K, V>(head_.next());
}

template <typename K, typename V, typename H, typename E>
LinkedIterator<K, V> LinkedHt<K, V, H, E>::end() {
  return LinkedIterator<K, V>(&head_);
}

template <typename K, typename V, typename H, typename E>
const LinkedIterator<K, V> LinkedHt<K, V, H, E>::end() const {
  return LinkedIterator<K, V>(&head_);
}

template <typename K, typename V, typename H, typename E>
bool LinkedHt<K, V, H, E>::isNotNull() {
  CASSERT((ht_ && count_) || (!ht_ && !count_), "ht_ {} null, count_ {} null",
          (void *)ht_, (void *)count_);
  return ht_ && count_;
}

template <typename K, typename V, typename H, typename E>
void LinkedHt<K, V, H, E>::extend(int n) {
  CASSERT(size_ >= 0, "size_ {} >= 0", size_);
  CASSERT(bucket_ >= 0, "bucket_ {} >= 0", bucket_);
  CASSERT(n > 0, "n {} > 0", n);
  n = std::max(16, n);
  if (n <= bucket_) {
    return;
  }
  if (!isNotNull() && loadFactor() < 4.0) {
    return;
  }
  LinkedNode<K, V> *new_ht =
      (LinkedNode<K, V> *)std::malloc(n * sizeof(LinkedNode<K, V>));
  int *new_count = (int *)std::malloc(n * sizeof(int));
  CASSERT(new_ht, "new_ht is null");
  CASSERT(new_count, "new_ht is null");
  for (int i = 0; i < n; i++) {
    LinkedNode<K, V>::initializeList(new_ht[i]);
    new_count[i] = 0;
  }
  for (int i = 0; i < bucket_; i++) {
    while (count_[i] > 0) {
      LinkedNode<K, V> *e = ht_[i].removeHead();
      int b = (int)hasher_(e->key()) % n;
      new_ht[b].insertHead(e);
      --count_[i];
    }
  }
  if (ht_) {
    std::free(ht_);
    ht_ = nullptr;
  }
  if (count_) {
    std::free(count_);
    count_ = nullptr;
  }
  ht_ = new_ht;
  count_ = new_count;
  bucket_ = n;
}

template <typename K, typename V, typename H, typename E>
void LinkedHt<K, V, H, E>::destroyList(int i) {
  while (ht_[i].next() != &ht_[i]) {
    LinkedNode<K, V> *e = ht_[i].removeTail();
    head_.tr_remove(e);
    delete e;
    --count_[i];
    --size_;
    CASSERT(count_[i] >= 0, "count_[{}] {} >= 0", i, count_[i]);
    CASSERT(size_ >= 0, "size_ {} >= 0", size_);
  }
  CASSERT(ht_[i].isNull(), "ht_[{}]#isNull: {}", i, ht_[i].isNull());
  CASSERT(count_[i] == 0, "count_[{}] {} == 0", i, count_[i]);
}

template <typename K, typename V>
LinkedIterator<K, V>::LinkedIterator(LinkedNode<K, V> *node) : node_(node) {}

template <typename K, typename V> LinkedIterator<K, V>::~LinkedIterator() {
  node_ = nullptr;
}

template <typename K, typename V>
LinkedIterator<K, V>::LinkedIterator(const LinkedIterator<K, V> &other)
    : node_(other.node_) {}

template <typename K, typename V>
LinkedIterator<K, V> &
LinkedIterator<K, V>::operator=(const LinkedIterator<K, V> &other) {
  if (this == &other) {
    return *this;
  }
  node_ = other.node_;
  return *this;
}

template <typename K, typename V>
LinkedIterator<K, V> &LinkedIterator<K, V>::operator++() {
  node_ = node_->next();
  return *this;
}

template <typename K, typename V>
LinkedIterator<K, V> &LinkedIterator<K, V>::operator--() {
  node_ = node_->prev();
  return *this;
}

template <typename K, typename V>
LinkedIterator<K, V> LinkedIterator<K, V>::operator++(int) {
  LinkedIterator<K, V> save = node_;
  node_ = node_->next();
  return LinkedIterator<K, V>(save);
}

template <typename K, typename V>
LinkedIterator<K, V> LinkedIterator<K, V>::operator--(int) {
  LinkedIterator<K, V> save = node_;
  node_ = node_->prev();
  return LinkedIterator<K, V>(save);
}

template <typename K, typename V>
bool LinkedIterator<K, V>::operator==(const LinkedIterator<K, V> &other) const {
  return node_ == other.node_;
}

template <typename K, typename V>
bool LinkedIterator<K, V>::operator!=(const LinkedIterator<K, V> &other) const {
  return node_ != other.node_;
}
template <typename K, typename V> bool LinkedIterator<K, V>::operator!() const {
  return !node_;
}

template <typename K, typename V>
LinkedNode<K, V> &LinkedIterator<K, V>::operator*() {
  return *node_;
}

template <typename K, typename V>
LinkedNode<K, V> *LinkedIterator<K, V>::operator->() {
  return node_;
}

} // namespace detail

template <typename K, typename V, typename H, typename E>
LinkedHashMap<K, V, H, E>::LinkedHashMap() : ht_() {}

template <typename K, typename V, typename H, typename E>
LinkedHashMap<K, V, H, E>::LinkedHashMap(int bucket) : ht_(bucket) {}

template <typename K, typename V, typename H, typename E>
LinkedHashMap<K, V, H, E>::~LinkedHashMap() {}

template <typename K, typename V, typename H, typename E>
bool LinkedHashMap<K, V, H, E>::empty() const {
  return ht_.empty();
}

template <typename K, typename V, typename H, typename E>
int LinkedHashMap<K, V, H, E>::size() const {
  return ht_.size();
}

template <typename K, typename V, typename H, typename E>
int LinkedHashMap<K, V, H, E>::bucket() const {
  return ht_.bucket();
}

template <typename K, typename V, typename H, typename E>
double LinkedHashMap<K, V, H, E>::loadFactor() const {
  return ht_.loadFactor();
}

template <typename K, typename V, typename H, typename E>
void LinkedHashMap<K, V, H, E>::clear() {
  ht_.clear();
}

template <typename K, typename V, typename H, typename E>
void LinkedHashMap<K, V, H, E>::release() {
  ht_.release();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::Iterator
LinkedHashMap<K, V, H, E>::begin() {
  return ht_.begin();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::CIterator
LinkedHashMap<K, V, H, E>::begin() const {
  return ht_.begin();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::Iterator LinkedHashMap<K, V, H, E>::end() {
  return ht_.end();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::CIterator
LinkedHashMap<K, V, H, E>::end() const {
  return ht_.end();
}

template <typename K, typename V, typename H, typename E>
void LinkedHashMap<K, V, H, E>::insert(const K &key, const V &mapped) {
  ht_.insert(std::pair<const K, V>(key, mapped));
}

template <typename K, typename V, typename H, typename E>
void LinkedHashMap<K, V, H, E>::insert(const std::pair<const K, V> &value) {
  ht_.insert(value);
}

template <typename K, typename V, typename H, typename E>
int LinkedHashMap<K, V, H, E>::insertOrAssign(const K &key, const V &mapped) {
  return ht_.insertOrAssign(std::pair<const K, V>(key, mapped));
}

template <typename K, typename V, typename H, typename E>
int LinkedHashMap<K, V, H, E>::insertOrAssign(
    const std::pair<const K, V> &value) {
  return ht_.insertOrAssign(value);
}

template <typename K, typename V, typename H, typename E>
int LinkedHashMap<K, V, H, E>::remove(const K &key) {
  auto position = find(key);
  return position == end() ? -1 : ht_.remove(position);
}

template <typename K, typename V, typename H, typename E>
int LinkedHashMap<K, V, H, E>::remove(
    LinkedHashMap<K, V, H, E>::Iterator position) {
  return ht_.remove(position);
}

template <typename K, typename V, typename H, typename E>
bool LinkedHashMap<K, V, H, E>::exist(const K &key) const {
  return find(key) != end();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::Iterator
LinkedHashMap<K, V, H, E>::find(const K &key) const {
  return ht_.find(key);
}

template <typename K, typename V, typename H, typename E>
V &LinkedHashMap<K, V, H, E>::operator[](const K &key) {
  return ht_.find(key)->second;
}

template <typename K, typename V, typename H, typename E>
const V &LinkedHashMap<K, V, H, E>::operator[](const K &key) const {
  LinkedHashMap<K, V, H, E>::Iterator position = ht_.find(key);
  return ht_.find(key)->second;
}
