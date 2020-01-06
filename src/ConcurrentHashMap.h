// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "config/Header.h"
#include <mutex>
#include <unordered_map>
#include <utility>

template <typename K, typename T> class ConcurrentHashMap : boost::noncopyable {
public:
  typedef typename std::unordered_map<
      K, T, std::hash<K>, std::equal_to<K>,
      std::allocator<std::pair<const K, T>>>::iterator MapIterator;
  typedef typename std::unordered_map<
      K, T, std::hash<K>, std::equal_to<K>,
      std::allocator<std::pair<const K, T>>>::const_iterator MapConstIterator;

  ConcurrentHashMap() : map_(), lock_() {}
  ~ConcurrentHashMap() {}

  // move
  ConcurrentHashMap(ConcurrentHashMap<K, T> &&other) : map_() { swap(other); }

  ConcurrentHashMap &operator=(ConcurrentHashMap<K, T> &&other) {
    if (this == &other) {
      return *this;
    }
    swap(other);
    return *this;
  }

  const T &operator[](const K &key) const {
    std::lock_guard<std::recursive_mutex> guard(lock_);
    return map_[key];
  }

  T &operator[](const K &key) {
    std::lock_guard<std::recursive_mutex> guard(lock_);
    return map_[key];
  }

  std::pair<MapIterator, bool> insert(const std::pair<K, T> &p) {
    std::lock_guard<std::recursive_mutex> guard(lock_);
    return map_.insert(p);
  }

  std::pair<MapIterator, bool> insert(std::pair<K, T> &&p) {
    std::lock_guard<std::recursive_mutex> guard(lock_);
    return map_.insert(std::move(p));
  }

  MapIterator remove(MapIterator pos) {
    std::lock_guard<std::recursive_mutex> guard(lock_);
    return map_.erase(pos);
  }

  MapIterator remove(MapConstIterator pos) {
    std::lock_guard<std::recursive_mutex> guard(lock_);
    return map_.erase(pos);
  }

  MapIterator find(const K &key) {
    std::lock_guard<std::recursive_mutex> guard(lock_);
    return map_.find(key);
  }

  MapConstIterator find(const K &key) const {
    std::lock_guard<std::recursive_mutex> guard(lock_);
    return map_.find(key);
  }

  MapIterator begin() {
    std::lock_guard<std::recursive_mutex> guard(lock_);
    return map_.begin();
  }

  MapIterator end() {
    std::lock_guard<std::recursive_mutex> guard(lock_);
    return map_.end();
  }

  MapConstIterator begin() const {
    std::lock_guard<std::recursive_mutex> guard(lock_);
    return map_.cbegin();
  }

  MapConstIterator end() const {
    std::lock_guard<std::recursive_mutex> guard(lock_);
    return map_.cend();
  }

  void clear() {
    std::lock_guard<std::recursive_mutex> guard(lock_);
    map_.clear();
    map_.shrink_to_fit();
  }

  int size() const {
    std::lock_guard<std::recursive_mutex> guard(lock_);
    return map_.size();
  }

  void swap(ConcurrentHashMap<K, T> &other) {
    std::lock_guard<std::recursive_mutex> guard(lock_);
    std::swap(map_, other.map_);
  }

  void lock() { lock_.lock(); }

  bool trylock() { return lock_.try_lock(); }

  void unlock() { lock_.unlock(); }

private:
  std::unordered_map<K, T> map_;
  std::recursive_mutex lock_;
};
