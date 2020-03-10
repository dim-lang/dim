// Copyright 2019- <coli-lang>
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

template <typename K, typename V>
LinkedList<K, V>::LinkedList(const LinkedList<K, V> &other)
    : prev_(other.prev_), next_(other.next_), seq_prev_(other.seq_prev_),
      seq_next_(other.seq_next_) {}

template <typename K, typename V>
LinkedList<K, V> &LinkedList<K, V>::operator=(const LinkedList<K, V> &other) {
  if (this == &other) {
    return *this;
  }
  prev_ = other.prev();
  next_ = other.next();
  seq_prev_ = other.seq_prev();
  seq_next_ = other.seq_next();
  return *this;
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
  CASSERT(prev_ && next_, "prev_ {}, next_ {} is null", (void *)prev_,
          (void *)next_);
  CASSERT((prev_ == CLN(this) && next_ == CLN(this)) ||
              (prev_ != CLN(this) && next_ != CLN(this)),
          "this: {}, prev_: {}, next_: {}", (void *)this, (void *)prev_,
          (void *)next_);
  return prev_ == CLN(this) && next_ == CLN(this);
}

template <typename K, typename V> bool LinkedList<K, V>::seq_empty() const {
  CASSERT(seq_prev_ && seq_next_, "seq_prev_ {}, seq_next_ {} is null",
          (void *)prev_, (void *)next_);
  CASSERT((seq_prev_ == CLN(this) && seq_next_ == CLN(this)) ||
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

template <typename K, typename V>
LinkedNode<K, V>::LinkedNode(const LinkedNode<K, V> &other)
    : prev_(other.prev_), next_(other.next_), seq_prev_(other.seq_prev_),
      seq_next_(other.seq_next_), value_(other.value_) {}

template <typename K, typename V>
LinkedNode<K, V> &LinkedNode<K, V>::operator=(const LinkedNode<K, V> &other) {
  if (this == &other) {
    return *this;
  }
  prev_ = other.prev();
  next_ = other.next();
  seq_prev_ = other.seq_prev();
  seq_next_ = other.seq_next();
  value_ = other.value();
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
std::pair<const K, V> &LinkedIterator<K, V>::operator*() {
  return node_->value();
}

template <typename K, typename V>
const std::pair<const K, V> &LinkedIterator<K, V>::operator*() const {
  return node_->value();
}

template <typename K, typename V>
std::pair<const K, V> *LinkedIterator<K, V>::operator->() {
  return &(node_->value());
}

template <typename K, typename V>
const std::pair<const K, V> *LinkedIterator<K, V>::operator->() const {
  return &(node_->value());
}

template <typename K, typename V>
std::string LinkedIterator<K, V>::toString() const {
  return fmt::format("[ @LinkedIterator node_:{} ]", (void *)node_);
}

// linked hashtable

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
  CASSERT((ht_ && count_) || (!ht_ && !count_ && size_ == 0 && bucket_ == 0),
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
    CASSERT(head_.seq_empty(), "head_#seq_empty: {}", head_.seq_empty());
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
  int b = getBucket(value.first);
  ht_[b].insertHead(e);
  head_.seq_insert(e);
  CASSERT(count_[b] >= 0, "count_[{}] {} >= 0", b, count_[b]);
  CASSERT(size_ >= 0, "size_ {} >= 0", size_);
  ++count_[b];
  ++size_;
}

template <typename K, typename V, typename H, typename E>
int LinkedHt<K, V, H, E>::insertOrAssign(const std::pair<const K, V> &value) {
  extend(2 * bucket_);
  LinkedIterator<K, V> position = find(value.first);
  if (position == end()) {
    LinkedNode<K, V> *e = new LinkedNode<K, V>(value);
    int b = getBucket(value.first);
    ht_[b].insertHead(e);
    head_.seq_insert(e);
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
  if (empty()) {
    return end();
  }
  int b = getBucket(key);
  LinkedNode<K, V> *e = ht_[b].next();
  while (e != CLN(&ht_[b])) {
    if (equal_(e->key(), key)) {
      return LinkedIterator<K, V>(e);
    }
    e = e->next();
  }
  return end();
}

template <typename K, typename V, typename H, typename E>
int LinkedHt<K, V, H, E>::remove(LinkedIterator<K, V> position) {
  if (empty()) {
    return -1;
  }
  int b = getBucket(position->first);
  LinkedNode<K, V> *e = ht_[b].next();
  while (e != CLN(&ht_[b])) {
    if (equal_(e->key(), position->first)) {
      ht_[b].remove(e);
      head_.seq_remove(e);
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
  return LinkedIterator<K, V>(CLN(head_.next()));
}

template <typename K, typename V, typename H, typename E>
const LinkedIterator<K, V> LinkedHt<K, V, H, E>::begin() const {
  return LinkedIterator<K, V>(CLN(head_.next()));
}

template <typename K, typename V, typename H, typename E>
LinkedIterator<K, V> LinkedHt<K, V, H, E>::end() {
  return LinkedIterator<K, V>(CLN(&head_));
}

template <typename K, typename V, typename H, typename E>
const LinkedIterator<K, V> LinkedHt<K, V, H, E>::end() const {
  return LinkedIterator<K, V>(CLN(&head_));
}

template <typename K, typename V, typename H, typename E>
void LinkedHt<K, V, H, E>::extend(int n) {
  CASSERT(size_ >= 0, "size_ {} >= 0", size_);
  CASSERT(bucket_ >= 0, "bucket_ {} >= 0", bucket_);
  if (!empty() && load() < 4.0) {
    return;
  }
  n = alignBucket(n);
  CASSERT(n > bucket_, "n {} > bucket_ {}", n, bucket_);
  LinkedList<K, V> *ht = new LinkedList<K, V>[n];
  int *count = new int[n];
  CASSERT(ht, "ht is null");
  CASSERT(count, "count is null");
  std::memset(count, 0, n * sizeof(int));

  // first try rehash old hashtable
  if (!empty()) {
    for (int i = 0; i < bucket_; i++) {
      while (count_[i] > 0) {
        LinkedNode<K, V> *e = ht_[i].removeHead();
        int b = getBucket(e->key());
        ht[b].insertHead(e);
        --count_[i];
        ++count[b];
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
  ht_ = ht;
  count_ = count;
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
    CASSERT(count_[i] >= 0, "count_[{}] {} >= 0", i, count_[i]);
    CASSERT(size_ >= 0, "size_ {} >= 0", size_);
  }
  CASSERT(ht_[i].empty(), "ht_[{}]#empty: {}", i, ht_[i].empty());
  CASSERT(count_[i] == 0, "count_[{}] {} == 0", i, count_[i]);
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
int LinkedHt<K, V, H, E>::getBucket(const K &key) const {
  return int((size_t)hasher_(key) % (size_t)bucket_);
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
bool LinkedHashMap<K, V, H, E>::exist(const K &key) const {
  return find(key) != end();
}

template <typename K, typename V, typename H, typename E>
typename LinkedHashMap<K, V, H, E>::Iterator
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
