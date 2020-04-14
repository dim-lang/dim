// Copyright 2019- <ac-lang>
// Apache License Version 2.0

#pragma once
#include "LinkedHashMap.h"
#include "Log.h"

#define CLN(x) ((LinkedNode<K, V> *)x)

namespace detail {

// linked list

template <typename K, typename V>
LinkedList<K, V>::LinkedList()
    : prev_(CLN(this)), next_(CLN(this)), seq_prev_(CLN(this)),
      seq_next_(CLN(this)) {}

template <typename K, typename V> LinkedList<K, V>::~LinkedList() {
  prev_ = nullptr;
  next_ = nullptr;
  seq_prev_ = nullptr;
  seq_next_ = nullptr;
}

template <typename K, typename V> LinkedNode<K, V> *&LinkedList<K, V>::prev() {
  return prev_;
}

template <typename K, typename V>
const LinkedNode<K, V> *LinkedList<K, V>::prev() const {
  return prev_;
}

template <typename K, typename V> LinkedNode<K, V> *&LinkedList<K, V>::next() {
  return next_;
}

template <typename K, typename V>
const LinkedNode<K, V> *LinkedList<K, V>::next() const {
  return next_;
}

template <typename K, typename V>
LinkedNode<K, V> *&LinkedList<K, V>::seq_prev() {
  return seq_prev_;
}

template <typename K, typename V>
const LinkedNode<K, V> *LinkedList<K, V>::seq_prev() const {
  return seq_prev_;
}

template <typename K, typename V>
LinkedNode<K, V> *&LinkedList<K, V>::seq_next() {
  return seq_next_;
}

template <typename K, typename V>
const LinkedNode<K, V> *LinkedList<K, V>::seq_next() const {
  return seq_next_;
}

template <typename K, typename V> LinkedNode<K, V> *&LinkedList<K, V>::head() {
  return next_;
}

template <typename K, typename V>
const LinkedNode<K, V> *LinkedList<K, V>::head() const {
  return next_;
}

template <typename K, typename V> LinkedNode<K, V> *&LinkedList<K, V>::tail() {
  return prev_;
}

template <typename K, typename V>
const LinkedNode<K, V> *LinkedList<K, V>::tail() const {
  return prev_;
}

template <typename K, typename V>
void LinkedList<K, V>::insertTail(LinkedNode<K, V> *e) {
  e->prev() = prev_;
  e->next() = CLN(this);
  prev_->next() = e;
  prev_ = e;
}

template <typename K, typename V>
void LinkedList<K, V>::insertHead(LinkedNode<K, V> *e) {
  e->next() = next_;
  e->prev() = CLN(this);
  next_->prev() = e;
  next_ = e;
}

template <typename K, typename V>
LinkedNode<K, V> *LinkedList<K, V>::removeTail() {
  if (empty()) {
    return CLN(this);
  }
  LinkedNode<K, V> *e = prev_;
  prev_->prev()->next() = CLN(this);
  prev_ = prev_->prev();
  return e;
}

template <typename K, typename V>
LinkedNode<K, V> *LinkedList<K, V>::removeHead() {
  if (empty()) {
    return CLN(this);
  }
  LinkedNode<K, V> *e = next_;
  next_->next()->prev() = CLN(this);
  next_ = next_->next();
  return e;
}

template <typename K, typename V>
void LinkedList<K, V>::remove(LinkedNode<K, V> *e) {
  if (empty()) {
    return;
  }
  e->prev()->next() = e->next();
  e->next()->prev() = e->prev();
  e->prev() = nullptr;
  e->next() = nullptr;
}

template <typename K, typename V>
void LinkedList<K, V>::seq_insert(LinkedNode<K, V> *e) {
  e->seq_prev() = seq_prev_;
  e->seq_next() = CLN(this);
  seq_prev_->seq_next() = e;
  seq_prev_ = e;
}

template <typename K, typename V>
void LinkedList<K, V>::seq_remove(LinkedNode<K, V> *e) {
  if (seq_empty()) {
    return;
  }
  e->seq_prev()->seq_next() = e->seq_next();
  e->seq_next()->seq_prev() = e->seq_prev();
  e->seq_prev() = nullptr;
  e->seq_next() = nullptr;
}

template <typename K, typename V> bool LinkedList<K, V>::empty() const {
  LOG_ASSERT(prev_ && next_, "prev_ {}, next_ {} is null", (void *)prev_,
          (void *)next_);
  LOG_ASSERT((prev_ == CLN(this) && next_ == CLN(this)) ||
              (prev_ != CLN(this) && next_ != CLN(this)),
          "this: {}, prev_: {}, next_: {}", (void *)this, (void *)prev_,
          (void *)next_);
  return prev_ == CLN(this) && next_ == CLN(this);
}

template <typename K, typename V> bool LinkedList<K, V>::seq_empty() const {
  LOG_ASSERT(seq_prev_ && seq_next_, "seq_prev_ {}, seq_next_ {} is null",
          (void *)prev_, (void *)next_);
  LOG_ASSERT((seq_prev_ == CLN(this) && seq_next_ == CLN(this)) ||
              (seq_prev_ != CLN(this) && seq_next_ != CLN(this)),
          "this: {}, seq_prev_: {}, seq_next_: {}", (void *)this,
          (void *)seq_prev_, (void *)seq_next_);
  return seq_prev_ == CLN(this) && seq_next_ == CLN(this);
}

// linked node

template <typename K, typename V>
LinkedNode<K, V>::LinkedNode(const std::pair<const K, V> &value)
    : prev_(nullptr), next_(nullptr), seq_prev_(nullptr), seq_next_(nullptr),
      value_(value) {}

template <typename K, typename V> LinkedNode<K, V>::~LinkedNode() {
  prev_ = nullptr;
  next_ = nullptr;
  seq_prev_ = nullptr;
  seq_next_ = nullptr;
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
LinkedNode<K, V> *&LinkedNode<K, V>::seq_prev() {
  return seq_prev_;
}

template <typename K, typename V>
const LinkedNode<K, V> *LinkedNode<K, V>::seq_prev() const {
  return seq_prev_;
}

template <typename K, typename V>
LinkedNode<K, V> *&LinkedNode<K, V>::seq_next() {
  return seq_next_;
}

template <typename K, typename V>
const LinkedNode<K, V> *LinkedNode<K, V>::seq_next() const {
  return seq_next_;
}

// linked iterator

template <typename K, typename V, typename T>
LinkedIterator<K, V, T>::LinkedIterator() : node_(nullptr) {}

template <typename K, typename V, typename T>
LinkedIterator<K, V, T>::LinkedIterator(T node) : node_(node) {}

template <typename K, typename V, typename T>
LinkedIterator<K, V, T>::~LinkedIterator() {
  node_ = nullptr;
}

template <typename K, typename V, typename T>
LinkedIterator<K, V, T>::LinkedIterator(const LinkedIterator<K, V, T> &other)
    : node_(other.node_) {}

template <typename K, typename V, typename T>
LinkedIterator<K, V, T> &
LinkedIterator<K, V, T>::operator=(const LinkedIterator<K, V, T> &other) {
  if (this == &other) {
    return *this;
  }
  node_ = other.node_;
  return *this;
}

template <typename K, typename V, typename T>
LinkedIterator<K, V, T>::LinkedIterator(const LinkedRIterator<K, V, T> &other)
    : node_(other.get()) {}

template <typename K, typename V, typename T>
LinkedIterator<K, V, T> &
LinkedIterator<K, V, T>::operator=(const LinkedRIterator<K, V, T> &other) {
  if (node_ == other.get()) {
    return *this;
  }
  node_ = other.get();
  return *this;
}

template <typename K, typename V, typename T>
LinkedIterator<K, V, T> &LinkedIterator<K, V, T>::operator++() {
  node_ = node_->seq_next();
  return *this;
}

template <typename K, typename V, typename T>
LinkedIterator<K, V, T> &LinkedIterator<K, V, T>::operator--() {
  node_ = node_->seq_prev();
  return *this;
}

template <typename K, typename V, typename T>
LinkedIterator<K, V, T> LinkedIterator<K, V, T>::operator++(int) {
  LinkedIterator<K, V, T> save = node_;
  node_ = node_->seq_next();
  return LinkedIterator<K, V, T>(save);
}

template <typename K, typename V, typename T>
LinkedIterator<K, V, T> LinkedIterator<K, V, T>::operator--(int) {
  LinkedIterator<K, V, T> save = node_;
  node_ = node_->seq_prev();
  return LinkedIterator<K, V, T>(save);
}

template <typename K, typename V, typename T>
T LinkedIterator<K, V, T>::get() const {
  return node_;
}

template <typename K, typename V, typename T>
bool LinkedIterator<K, V, T>::operator==(
    const LinkedIterator<K, V, T> &other) const {
  return node_ == other.node_;
}

template <typename K, typename V, typename T>
bool LinkedIterator<K, V, T>::operator!=(
    const LinkedIterator<K, V, T> &other) const {
  return node_ != other.node_;
}
template <typename K, typename V, typename T>
bool LinkedIterator<K, V, T>::operator!() const {
  return !node_;
}

template <typename K, typename V, typename T>
std::pair<const K, V> &LinkedIterator<K, V, T>::operator*() {
  return node_->value();
}

template <typename K, typename V, typename T>
const std::pair<const K, V> &LinkedIterator<K, V, T>::operator*() const {
  return node_->value();
}

template <typename K, typename V, typename T>
std::pair<const K, V> *LinkedIterator<K, V, T>::operator->() {
  return &(node_->value());
}

template <typename K, typename V, typename T>
const std::pair<const K, V> *LinkedIterator<K, V, T>::operator->() const {
  return &(node_->value());
}

template <typename K, typename V, typename T>
std::string LinkedIterator<K, V, T>::toString() const {
  return fmt::format("[ @LinkedIterator node_:{} ]", (void *)node_);
}

// linked reverse iterator

template <typename K, typename V, typename T>
LinkedRIterator<K, V, T>::LinkedRIterator() : node_(nullptr) {}

template <typename K, typename V, typename T>
LinkedRIterator<K, V, T>::LinkedRIterator(T node) : node_(node) {}

template <typename K, typename V, typename T>
LinkedRIterator<K, V, T>::~LinkedRIterator() {
  node_ = nullptr;
}

template <typename K, typename V, typename T>
LinkedRIterator<K, V, T>::LinkedRIterator(const LinkedRIterator<K, V, T> &other)
    : node_(other.node_) {}

template <typename K, typename V, typename T>
LinkedRIterator<K, V, T> &
LinkedRIterator<K, V, T>::operator=(const LinkedRIterator<K, V, T> &other) {
  if (this == &other) {
    return *this;
  }
  node_ = other.node_;
  return *this;
}

template <typename K, typename V, typename T>
LinkedRIterator<K, V, T>::LinkedRIterator(const LinkedIterator<K, V, T> &other)
    : node_(other.get()) {}

template <typename K, typename V, typename T>
LinkedRIterator<K, V, T> &
LinkedRIterator<K, V, T>::operator=(const LinkedIterator<K, V, T> &other) {
  if (node_ == &other.get()) {
    return *this;
  }
  node_ = other.get();
  return *this;
}

template <typename K, typename V, typename T>
LinkedRIterator<K, V, T> &LinkedRIterator<K, V, T>::operator++() {
  node_ = node_->seq_prev();
  return *this;
}

template <typename K, typename V, typename T>
LinkedRIterator<K, V, T> &LinkedRIterator<K, V, T>::operator--() {
  node_ = node_->seq_next();
  return *this;
}

template <typename K, typename V, typename T>
LinkedRIterator<K, V, T> LinkedRIterator<K, V, T>::operator++(int) {
  LinkedRIterator<K, V, T> save = node_;
  node_ = node_->seq_prev();
  return LinkedRIterator<K, V, T>(save);
}

template <typename K, typename V, typename T>
LinkedRIterator<K, V, T> LinkedRIterator<K, V, T>::operator--(int) {
  LinkedRIterator<K, V, T> save = node_;
  node_ = node_->seq_next();
  return LinkedRIterator<K, V, T>(save);
}

template <typename K, typename V, typename T>
T LinkedRIterator<K, V, T>::get() const {
  return node_;
}

template <typename K, typename V, typename T>
bool LinkedRIterator<K, V, T>::operator==(
    const LinkedRIterator<K, V, T> &other) const {
  return node_ == other.node_;
}

template <typename K, typename V, typename T>
bool LinkedRIterator<K, V, T>::operator!=(
    const LinkedRIterator<K, V, T> &other) const {
  return node_ != other.node_;
}

template <typename K, typename V, typename T>
bool LinkedRIterator<K, V, T>::operator!() const {
  return !node_;
}

template <typename K, typename V, typename T>
std::pair<const K, V> &LinkedRIterator<K, V, T>::operator*() {
  return node_->value();
}

template <typename K, typename V, typename T>
const std::pair<const K, V> &LinkedRIterator<K, V, T>::operator*() const {
  return node_->value();
}

template <typename K, typename V, typename T>
std::pair<const K, V> *LinkedRIterator<K, V, T>::operator->() {
  return &(node_->value());
}

template <typename K, typename V, typename T>
const std::pair<const K, V> *LinkedRIterator<K, V, T>::operator->() const {
  return &(node_->value());
}

template <typename K, typename V, typename T>
std::string LinkedRIterator<K, V, T>::toString() const {
  return fmt::format("[ @LinkedRIterator node_:{} ]", (void *)node_);
}

// linked hashtable

template <typename K, typename V, typename H, typename E>
LinkedHt<K, V, H, E>::LinkedHt()
    : hasher_(), equal_(), head_(), ht_(nullptr), count_(nullptr), bucket_(0),
      size_(0) {}

template <typename K, typename V, typename H, typename E>
LinkedHt<K, V, H, E>::LinkedHt(int bucket) : LinkedHt() {
  LOG_ASSERT(bucket > 0, "bucket {} > 0", bucket);
  extend(bucket);
}

template <typename K, typename V, typename H, typename E>
LinkedHt<K, V, H, E>::~LinkedHt() {
  release();
}

template <typename K, typename V, typename H, typename E>
bool LinkedHt<K, V, H, E>::empty() const {
  LOG_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  LOG_ASSERT((ht_ && count_) || (!ht_ && !count_ && size_ == 0 && bucket_ == 0),
          "ht_ {} null, count_ {} null, size_: {}, bucket_: {}", (void *)ht_,
          (void *)count_, size_, bucket_);
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
double LinkedHt<K, V, H, E>::load() const {
  return empty() ? 0.0 : ((double)size_ / (double)bucket_);
}

template <typename K, typename V, typename H, typename E>
void LinkedHt<K, V, H, E>::clear() {
  if (!empty()) {
    for (int i = 0; i < bucket_; i++) {
      destroyList(i);
    }
  }
  size_ = 0;
}

template <typename K, typename V, typename H, typename E>
void LinkedHt<K, V, H, E>::release() {
  if (!empty()) {
    for (int i = 0; i < bucket_; i++) {
      destroyList(i);
    }
    LOG_ASSERT(head_.seq_empty(), "head_#seq_empty: {}", head_.seq_empty());
    delete[] ht_;
    delete[] count_;
    ht_ = nullptr;
    count_ = nullptr;
  }
  bucket_ = 0;
  size_ = 0;
}

template <typename K, typename V, typename H, typename E>
void LinkedHt<K, V, H, E>::insert(const std::pair<const K, V> &value) {
  extend(2 * bucket_);
  LinkedNode<K, V> *e = new LinkedNode<K, V>(value);
  int b = getBucket(value.first, bucket_);
  ht_[b].insertHead(e);
  head_.seq_insert(e);
  LOG_ASSERT(count_[b] >= 0, "count_[{}] {} >= 0", b, count_[b]);
  LOG_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  ++count_[b];
  ++size_;
}

template <typename K, typename V, typename H, typename E>
int LinkedHt<K, V, H, E>::insertOrAssign(const std::pair<const K, V> &value) {
  extend(2 * bucket_);
  Iterator position = find(value.first);
  if (position == end()) {
    LinkedNode<K, V> *e = new LinkedNode<K, V>(value);
    int b = getBucket(value.first, bucket_);
    ht_[b].insertHead(e);
    head_.seq_insert(e);
    LOG_ASSERT(count_[b] >= 0, "count_[{}] {} >= 0", b, count_[b]);
    LOG_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
    ++count_[b];
    ++size_;
    return 1;
  } else {
    position->second = value.second;
    return 0;
  }
}

template <typename K, typename V, typename H, typename E>
typename LinkedHt<K, V, H, E>::Iterator
LinkedHt<K, V, H, E>::find(const K &key) {
  if (empty()) {
    return end();
  }
  int b = getBucket(key, bucket_);
  LinkedNode<K, V> *e = ht_[b].next();
  while (e != CLN(&ht_[b])) {
    if (equal_(e->key(), key)) {
      return LinkedHt<K, V, H, E>::Iterator(e);
    }
    e = e->next();
  }
  return end();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHt<K, V, H, E>::CIterator
LinkedHt<K, V, H, E>::find(const K &key) const {
  if (empty()) {
    return end();
  }
  int b = getBucket(key, bucket_);
  LinkedNode<K, V> *e = ht_[b].next();
  while (e != CLN(&ht_[b])) {
    if (equal_(e->key(), key)) {
      return LinkedHt<K, V, H, E>::CIterator(e);
    }
    e = e->next();
  }
  return end();
}

template <typename K, typename V, typename H, typename E>
int LinkedHt<K, V, H, E>::remove(
    typename LinkedHt<K, V, H, E>::Iterator position) {
  if (empty()) {
    return -1;
  }
  int b = getBucket(position->first, bucket_);
  LinkedNode<K, V> *e = ht_[b].next();
  while (e != CLN(&ht_[b])) {
    if (equal_(e->key(), position->first)) {
      ht_[b].remove(e);
      head_.seq_remove(e);
      delete e;
      --count_[b];
      --size_;
      LOG_ASSERT(count_[b] >= 0, "count_[{}] {} >= 0", b, count_[b]);
      LOG_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
      return 0;
    }
    e = e->next();
  }
  return -1;
}

template <typename K, typename V, typename H, typename E>
int LinkedHt<K, V, H, E>::remove(
    typename LinkedHt<K, V, H, E>::RIterator position) {
  if (empty()) {
    return -1;
  }
  int b = getBucket(position->first, bucket_);
  LinkedNode<K, V> *e = ht_[b].next();
  while (e != CLN(&ht_[b])) {
    if (equal_(e->key(), position->first)) {
      ht_[b].remove(e);
      head_.seq_remove(e);
      delete e;
      --count_[b];
      --size_;
      LOG_ASSERT(count_[b] >= 0, "count_[{}] {} >= 0", b, count_[b]);
      LOG_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
      return 0;
    }
    e = e->next();
  }
  return -1;
}

template <typename K, typename V, typename H, typename E>
typename LinkedHt<K, V, H, E>::Iterator LinkedHt<K, V, H, E>::begin() {
  return typename LinkedHt<K, V, H, E>::Iterator(CLN(head_.seq_next()));
}

template <typename K, typename V, typename H, typename E>
typename LinkedHt<K, V, H, E>::CIterator LinkedHt<K, V, H, E>::begin() const {
  return typename LinkedHt<K, V, H, E>::CIterator(CLN(head_.seq_next()));
}

template <typename K, typename V, typename H, typename E>
typename LinkedHt<K, V, H, E>::Iterator LinkedHt<K, V, H, E>::end() {
  return typename LinkedHt<K, V, H, E>::Iterator(CLN(&head_));
}

template <typename K, typename V, typename H, typename E>
typename LinkedHt<K, V, H, E>::CIterator LinkedHt<K, V, H, E>::end() const {
  return typename LinkedHt<K, V, H, E>::CIterator(CLN(&head_));
}

template <typename K, typename V, typename H, typename E>
typename LinkedHt<K, V, H, E>::RIterator LinkedHt<K, V, H, E>::rbegin() {
  return typename LinkedHt<K, V, H, E>::RIterator(CLN(head_.seq_prev()));
}

template <typename K, typename V, typename H, typename E>
typename LinkedHt<K, V, H, E>::CRIterator LinkedHt<K, V, H, E>::rbegin() const {
  return typename LinkedHt<K, V, H, E>::CRIterator(CLN(head_.seq_prev()));
}

template <typename K, typename V, typename H, typename E>
typename LinkedHt<K, V, H, E>::RIterator LinkedHt<K, V, H, E>::rend() {
  return typename LinkedHt<K, V, H, E>::RIterator(CLN(&head_));
}

template <typename K, typename V, typename H, typename E>
typename LinkedHt<K, V, H, E>::CRIterator LinkedHt<K, V, H, E>::rend() const {
  return typename LinkedHt<K, V, H, E>::CRIterator(CLN(&head_));
}

template <typename K, typename V, typename H, typename E>
void LinkedHt<K, V, H, E>::extend(int n) {
  LOG_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  LOG_ASSERT(bucket_ >= 0, "bucket_ {} >= 0", bucket_);
  if (!empty() && load() < 4.0) {
    return;
  }
  n = alignBucket(n);
  LOG_ASSERT(n > bucket_, "n {} > bucket_ {}", n, bucket_);
  LinkedList<K, V> *newHt = new LinkedList<K, V>[n];
  int *newCount = new int[n];
  LOG_ASSERT(newHt, "newHt is null");
  LOG_ASSERT(newCount, "newCount is null");
  std::memset(newCount, 0, n * sizeof(int));

  // first try rehash old hashtable
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

  // then assign new hashtable
  ht_ = newHt;
  count_ = newCount;
  bucket_ = n;
}

template <typename K, typename V, typename H, typename E>
void LinkedHt<K, V, H, E>::destroyList(int i) {
  while (ht_[i].next() != CLN(&ht_[i])) {
    LinkedNode<K, V> *e = ht_[i].removeTail();
    head_.seq_remove(e);
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
int LinkedHt<K, V, H, E>::alignBucket(int n) const {
  if (n <= 0)
    return 8;
  if (n < (std::numeric_limits<int>::max() - 7) / 2)
    return (n + 7) / 8 * 8;
  return std::numeric_limits<int>::max();
}

template <typename K, typename V, typename H, typename E>
int LinkedHt<K, V, H, E>::getBucket(const K &key, int bucket) const {
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
double LinkedHashMap<K, V, H, E>::load() const {
  return hm_.load();
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
typename LinkedHashMap<K, V, H, E>::Iterator
LinkedHashMap<K, V, H, E>::begin() {
  return hm_.begin();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::CIterator
LinkedHashMap<K, V, H, E>::begin() const {
  return hm_.begin();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::Iterator LinkedHashMap<K, V, H, E>::end() {
  return hm_.end();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::CIterator
LinkedHashMap<K, V, H, E>::end() const {
  return hm_.end();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::RIterator
LinkedHashMap<K, V, H, E>::rbegin() {
  return hm_.rbegin();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::CRIterator
LinkedHashMap<K, V, H, E>::rbegin() const {
  return hm_.rbegin();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::RIterator
LinkedHashMap<K, V, H, E>::rend() {
  return hm_.rend();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::CRIterator
LinkedHashMap<K, V, H, E>::rend() const {
  return hm_.rend();
}

template <typename K, typename V, typename H, typename E>
void LinkedHashMap<K, V, H, E>::insert(const K &key, const V &mapped) {
  hm_.insert(std::pair<const K, V>(key, mapped));
}

template <typename K, typename V, typename H, typename E>
void LinkedHashMap<K, V, H, E>::insert(const std::pair<const K, V> &value) {
  hm_.insert(value);
}

template <typename K, typename V, typename H, typename E>
int LinkedHashMap<K, V, H, E>::insertOrAssign(const K &key, const V &mapped) {
  return hm_.insertOrAssign(std::pair<const K, V>(key, mapped));
}

template <typename K, typename V, typename H, typename E>
int LinkedHashMap<K, V, H, E>::insertOrAssign(
    const std::pair<const K, V> &value) {
  return hm_.insertOrAssign(value);
}

template <typename K, typename V, typename H, typename E>
int LinkedHashMap<K, V, H, E>::remove(const K &key) {
  auto position = find(key);
  return position == end() ? -1 : hm_.remove(position);
}

template <typename K, typename V, typename H, typename E>
int LinkedHashMap<K, V, H, E>::remove(
    LinkedHashMap<K, V, H, E>::Iterator position) {
  return hm_.remove(position);
}

template <typename K, typename V, typename H, typename E>
int LinkedHashMap<K, V, H, E>::remove(
    LinkedHashMap<K, V, H, E>::RIterator position) {
  return hm_.remove(position);
}

template <typename K, typename V, typename H, typename E>
bool LinkedHashMap<K, V, H, E>::exist(const K &key) const {
  return find(key) != end();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::Iterator
LinkedHashMap<K, V, H, E>::find(const K &key) {
  return hm_.find(key);
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::CIterator
LinkedHashMap<K, V, H, E>::find(const K &key) const {
  return hm_.find(key);
}

template <typename K, typename V, typename H, typename E>
V &LinkedHashMap<K, V, H, E>::operator[](const K &key) {
  return hm_.find(key)->second;
}

template <typename K, typename V, typename H, typename E>
const V &LinkedHashMap<K, V, H, E>::operator[](const K &key) const {
  LinkedHashMap<K, V, H, E>::Iterator position = hm_.find(key);
  return hm_.find(key)->second;
}

#undef CLN
