// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "Exception.h"
#include "LinkedHashMap.h"

#define CLN(x) ((LinkedNode<K, V> *)x)
#define CCLN(x) ((const LinkedNode<K, V> *)x)

namespace detail {

// linked node

template <typename K, typename V>
LinkedNode<K, V>::LinkedNode(const std::pair<const K, V> &value)
    : prev(nullptr), next(nullptr), seq_prev(nullptr), seq_next(nullptr),
      value(value) {}

template <typename K, typename V> LinkedNode<K, V>::~LinkedNode() {
  prev = nullptr;
  next = nullptr;
  seq_prev = nullptr;
  seq_next = nullptr;
}

template <typename K, typename V> const K &LinkedNode<K, V>::key() const {
  return value.first;
}

// linked list

template <typename K, typename V>
LinkedList<K, V>::LinkedList()
    : prev(CLN(this)), next(CLN(this)), seq_prev(CLN(this)),
      seq_next(CLN(this)) {}

template <typename K, typename V> LinkedList<K, V>::~LinkedList() {
  prev = nullptr;
  next = nullptr;
  seq_prev = nullptr;
  seq_next = nullptr;
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
  e->next = CLN(this);
  prev->next = e;
  prev = e;
}

template <typename K, typename V>
void LinkedList<K, V>::insertHead(LinkedNode<K, V> *e) {
  e->next = next;
  e->prev = CLN(this);
  next->prev = e;
  next = e;
}

template <typename K, typename V>
LinkedNode<K, V> *LinkedList<K, V>::removeTail() {
  if (empty()) {
    return CLN(this);
  }
  LinkedNode<K, V> *e = prev;
  prev->prev->next = CLN(this);
  prev = prev->prev;
  return e;
}

template <typename K, typename V>
LinkedNode<K, V> *LinkedList<K, V>::removeHead() {
  if (empty()) {
    return CLN(this);
  }
  LinkedNode<K, V> *e = next;
  next->next->prev = CLN(this);
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
void LinkedList<K, V>::seq_insert(LinkedNode<K, V> *e) {
  e->seq_prev = seq_prev;
  e->seq_next = CLN(this);
  seq_prev->seq_next = e;
  seq_prev = e;
}

template <typename K, typename V>
void LinkedList<K, V>::seq_remove(LinkedNode<K, V> *e) {
  if (seq_empty()) {
    return;
  }
  e->seq_prev->seq_next = e->seq_next;
  e->seq_next->seq_prev = e->seq_prev;
  e->seq_prev = nullptr;
  e->seq_next = nullptr;
}

template <typename K, typename V> bool LinkedList<K, V>::empty() const {
  X_ASSERT(prev && next, "prev {}, next {} is null", (void *)prev,
           (void *)next);
  X_ASSERT((prev == CLN(this) && next == CLN(this)) ||
               (prev != CLN(this) && next != CLN(this)),
           "this: {}, prev: {}, next: {}", (void *)this, (void *)prev,
           (void *)next);
  return prev == CLN(this) && next == CLN(this);
}

template <typename K, typename V> bool LinkedList<K, V>::seq_empty() const {
  X_ASSERT(seq_prev && seq_next, "seq_prev {}, seq_next {} is null",
           (void *)prev, (void *)next);
  X_ASSERT((seq_prev == CLN(this) && seq_next == CLN(this)) ||
               (seq_prev != CLN(this) && seq_next != CLN(this)),
           "this: {}, seq_prev: {}, seq_next: {}", (void *)this,
           (void *)seq_prev, (void *)seq_next);
  return seq_prev == CLN(this) && seq_next == CLN(this);
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
    : node_(other.node_) {}

template <typename K, typename V, typename T>
LinkedIterator<K, V, T> &
LinkedIterator<K, V, T>::operator=(const LinkedRIterator<K, V, T> &other) {
  if (node_ == other.node_) {
    return *this;
  }
  node_ = other.node_;
  return *this;
}

template <typename K, typename V, typename T>
LinkedIterator<K, V, T> &LinkedIterator<K, V, T>::operator++() {
  node_ = node_->seq_next;
  return *this;
}

template <typename K, typename V, typename T>
LinkedIterator<K, V, T> &LinkedIterator<K, V, T>::operator--() {
  node_ = node_->seq_prev;
  return *this;
}

template <typename K, typename V, typename T>
LinkedIterator<K, V, T> LinkedIterator<K, V, T>::operator++(int) {
  LinkedIterator<K, V, T> save = node_;
  node_ = node_->seq_next;
  return LinkedIterator<K, V, T>(save);
}

template <typename K, typename V, typename T>
LinkedIterator<K, V, T> LinkedIterator<K, V, T>::operator--(int) {
  LinkedIterator<K, V, T> save = node_;
  node_ = node_->seq_prev;
  return LinkedIterator<K, V, T>(save);
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
  return (std::pair<const K, V> &)node_->value;
}

template <typename K, typename V, typename T>
const std::pair<const K, V> &LinkedIterator<K, V, T>::operator*() const {
  return (const std::pair<const K, V> &)node_->value;
}

template <typename K, typename V, typename T>
std::pair<const K, V> *LinkedIterator<K, V, T>::operator->() {
  return (std::pair<const K, V> *)&(node_->value);
}

template <typename K, typename V, typename T>
const std::pair<const K, V> *LinkedIterator<K, V, T>::operator->() const {
  return (const std::pair<const K, V> *)&(node_->value);
}

template <typename K, typename V, typename T>
std::string LinkedIterator<K, V, T>::toString() const {
  return fmt::format("[@LinkedIterator node_:{}]", (void *)node_);
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
    : node_(other.node_) {}

template <typename K, typename V, typename T>
LinkedRIterator<K, V, T> &
LinkedRIterator<K, V, T>::operator=(const LinkedIterator<K, V, T> &other) {
  if (node_ == &other.node_) {
    return *this;
  }
  node_ = other.node_;
  return *this;
}

template <typename K, typename V, typename T>
LinkedRIterator<K, V, T> &LinkedRIterator<K, V, T>::operator++() {
  node_ = node_->seq_prev;
  return *this;
}

template <typename K, typename V, typename T>
LinkedRIterator<K, V, T> &LinkedRIterator<K, V, T>::operator--() {
  node_ = node_->seq_next;
  return *this;
}

template <typename K, typename V, typename T>
LinkedRIterator<K, V, T> LinkedRIterator<K, V, T>::operator++(int) {
  LinkedRIterator<K, V, T> save = node_;
  node_ = node_->seq_prev;
  return LinkedRIterator<K, V, T>(save);
}

template <typename K, typename V, typename T>
LinkedRIterator<K, V, T> LinkedRIterator<K, V, T>::operator--(int) {
  LinkedRIterator<K, V, T> save = node_;
  node_ = node_->seq_next;
  return LinkedRIterator<K, V, T>(save);
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
  return (std::pair<const K, V> &)node_->value;
}

template <typename K, typename V, typename T>
const std::pair<const K, V> &LinkedRIterator<K, V, T>::operator*() const {
  return (const std::pair<const K, V> &)node_->value;
}

template <typename K, typename V, typename T>
std::pair<const K, V> *LinkedRIterator<K, V, T>::operator->() {
  return (std::pair<const K, V> *)&(node_->value);
}

template <typename K, typename V, typename T>
const std::pair<const K, V> *LinkedRIterator<K, V, T>::operator->() const {
  return (const std::pair<const K, V> *)&(node_->value);
}

template <typename K, typename V, typename T>
std::string LinkedRIterator<K, V, T>::toString() const {
  return fmt::format("[@LinkedRIterator node_:{}]", (void *)node_);
}

// linked hashtable

template <typename K, typename V, typename H, typename E>
LinkedHt<K, V, H, E>::LinkedHt()
    : hasher_(), equal_(), head_(), ht_(nullptr), count_(nullptr), bucket_(0),
      size_(0) {}

template <typename K, typename V, typename H, typename E>
LinkedHt<K, V, H, E>::LinkedHt(int bucket) : LinkedHt() {
  X_ASSERT(bucket > 0, "bucket {} > 0", bucket);
  extend(bucket);
}

template <typename K, typename V, typename H, typename E>
LinkedHt<K, V, H, E>::~LinkedHt() {
  release();
}

template <typename K, typename V, typename H, typename E>
bool LinkedHt<K, V, H, E>::empty() const {
  X_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  X_ASSERT((ht_ && count_) || (!ht_ && !count_ && size_ == 0 && bucket_ == 0),
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
    X_ASSERT(head_.seq_empty(), "head_#seq_empty: {}", head_.seq_empty());
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
  X_ASSERT(count_[b] >= 0, "count_[{}] {} >= 0", b, count_[b]);
  X_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
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
    X_ASSERT(count_[b] >= 0, "count_[{}] {} >= 0", b, count_[b]);
    X_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
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
  LinkedNode<K, V> *e = ht_[b].next;
  while (e != CLN(&ht_[b])) {
    if (equal_(e->key(), key)) {
      return LinkedHt<K, V, H, E>::Iterator(e);
    }
    e = e->next;
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
  const LinkedNode<K, V> *e = ht_[b].next;
  while (e != CCLN(&ht_[b])) {
    if (equal_(e->key(), key)) {
      return LinkedHt<K, V, H, E>::CIterator(e);
    }
    e = e->next;
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
  LinkedNode<K, V> *e = ht_[b].next;
  while (e != CLN(&ht_[b])) {
    if (equal_(e->key(), position->first)) {
      ht_[b].remove(e);
      head_.seq_remove(e);
      delete e;
      --count_[b];
      --size_;
      X_ASSERT(count_[b] >= 0, "count_[{}] {} >= 0", b, count_[b]);
      X_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
      return 0;
    }
    e = e->next;
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
  LinkedNode<K, V> *e = ht_[b].next;
  while (e != CLN(&ht_[b])) {
    if (equal_(e->key(), position->first)) {
      ht_[b].remove(e);
      head_.seq_remove(e);
      delete e;
      --count_[b];
      --size_;
      X_ASSERT(count_[b] >= 0, "count_[{}] {} >= 0", b, count_[b]);
      X_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
      return 0;
    }
    e = e->next;
  }
  return -1;
}

template <typename K, typename V, typename H, typename E>
typename LinkedHt<K, V, H, E>::Iterator LinkedHt<K, V, H, E>::begin() {
  return typename LinkedHt<K, V, H, E>::Iterator(CLN(head_.seq_next));
}

template <typename K, typename V, typename H, typename E>
typename LinkedHt<K, V, H, E>::CIterator LinkedHt<K, V, H, E>::begin() const {
  return typename LinkedHt<K, V, H, E>::CIterator(CCLN(head_.seq_next));
}

template <typename K, typename V, typename H, typename E>
typename LinkedHt<K, V, H, E>::Iterator LinkedHt<K, V, H, E>::end() {
  return typename LinkedHt<K, V, H, E>::Iterator(CLN(&head_));
}

template <typename K, typename V, typename H, typename E>
typename LinkedHt<K, V, H, E>::CIterator LinkedHt<K, V, H, E>::end() const {
  return typename LinkedHt<K, V, H, E>::CIterator(CCLN(&head_));
}

template <typename K, typename V, typename H, typename E>
typename LinkedHt<K, V, H, E>::RIterator LinkedHt<K, V, H, E>::rbegin() {
  return typename LinkedHt<K, V, H, E>::RIterator(CLN(head_.seq_prev));
}

template <typename K, typename V, typename H, typename E>
typename LinkedHt<K, V, H, E>::CRIterator LinkedHt<K, V, H, E>::rbegin() const {
  return typename LinkedHt<K, V, H, E>::CRIterator(CCLN(head_.seq_prev));
}

template <typename K, typename V, typename H, typename E>
typename LinkedHt<K, V, H, E>::RIterator LinkedHt<K, V, H, E>::rend() {
  return typename LinkedHt<K, V, H, E>::RIterator(CLN(&head_));
}

template <typename K, typename V, typename H, typename E>
typename LinkedHt<K, V, H, E>::CRIterator LinkedHt<K, V, H, E>::rend() const {
  return typename LinkedHt<K, V, H, E>::CRIterator(CCLN(&head_));
}

template <typename K, typename V, typename H, typename E>
void LinkedHt<K, V, H, E>::extend(int n) {
  X_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  X_ASSERT(bucket_ >= 0, "bucket_ {} >= 0", bucket_);
  if (!empty() && load() < 4.0) {
    return;
  }
  n = alignBucket(n);
  X_ASSERT(n > bucket_, "n {} > bucket_ {}", n, bucket_);
  LinkedList<K, V> *newHt = new LinkedList<K, V>[n];
  int *newCount = new int[n];
  X_ASSERT(newHt, "newHt is null");
  X_ASSERT(newCount, "newCount is null");
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
  while (ht_[i].next != CLN(&ht_[i])) {
    LinkedNode<K, V> *e = ht_[i].removeTail();
    head_.seq_remove(e);
    delete e;
    --count_[i];
    --size_;
    X_ASSERT(count_[i] >= 0, "count_[{}] {} >= 0", i, count_[i]);
    X_ASSERT(size_ >= 0, "size_ {} >= 0", size_);
  }
  X_ASSERT(ht_[i].empty(), "ht_[{}]#empty: {}", i, ht_[i].empty());
  X_ASSERT(count_[i] == 0, "count_[{}] {} == 0", i, count_[i]);
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
#undef CCLN
