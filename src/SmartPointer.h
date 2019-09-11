// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Logging.h"
#include "Stringify.h"
#include <boost/core/noncopyable.hpp>
#include <cstdint>
#include <fmt/format.h>
#include <memory>
#include <utility>

namespace fastype {

namespace detail {

class PointerCounter : public boost::noncopyable, Stringify {
public:
  PointerCounter() : counter_(0) {}
  PointerCounter(PointerCounter &&) = default;
  PointerCounter &operator=(PointerCounter &&) = default;
  virtual ~PointerCounter() {}
  void reset() { counter_ = 0; }
  void swap(PointerCounter &pc) { std::swap(counter_, pc.counter_); }
  int32_t get() { return counter_; }
  void operator++() { counter_++; }
  void operator++(int) { counter_++; }
  void operator--() { counter_--; }
  void operator--(int) { counter_--; }
  virtual std::string toString() const {
    return fmt::format("[ @PointerCounter counter_:{} ]", counter_);
  }

private:
  int32_t counter_;
};

} // namespace detail

template <typename T> class sptr : public Stringify {
public:
  explicit sptr(T *ptr = nullptr) : ptr_(ptr) {
    pc_ = new detail::PointerCounter();
    if (ptr_) {
      (*pc_)++;
    }
    F_DEBUGF("Constructor:{}", toString());
  }

  // copy
  sptr(const sptr<T> &sp) {
    ptr_ = sp.ptr_;
    pc_ = sp.pc_;
    (*pc_)++;
    F_DEBUGF("Copy Constructor:{}", toString());
  }

  sptr &operator=(const sptr<T> &sp) {
    if (this == &sp) {
      F_DEBUGF("Copy Assign self:{}", toString());
      return *this;
    }
    ptr_ = sp.ptr_;
    pc_ = sp.pc_;
    (*pc_)++;
    F_DEBUGF("Copy Assign:{}", toString());
    return *this;
  }

  // move
  sptr(sptr<T> &&sp) : sptr() {
    swap(sp);
    F_DEBUGF("Move Constructor:{}", toString());
  }

  sptr &operator=(sptr<T> &&sp) {
    if (this == &sp) {
      F_DEBUGF("Move Assign self:{}", toString());
      return *this;
    }
    swap(sp);
    F_DEBUGF("Move Assign:{}", toString());
    return *this;
  }

  virtual ~sptr() {
    release();
    if (pc_) {
      delete pc_;
      pc_ = nullptr;
    }
    F_DEBUGF("Destructor:{}", toString());
  }

  int32_t useCount() {
    F_DEBUGF("{}", toString());
    return pc_->get();
  }

  void reset(T *p) {
    release();
    ptr_ = p;
    (*pc_)++;
  }

  void swap(sptr<T> &sp) {
    std::swap(ptr_, sp.ptr_);
    std::swap(pc_, sp.pc_);
  }

  explicit operator bool() const { return ptr_; }
  T &operator*() const { return *ptr_; }
  T *operator->() const { return ptr_; }
  T *get() const { return ptr_; }

  virtual std::string toString() const {
    return fmt::format("[ @sptr ptr_:{} pc_:{} ]", (void *)ptr_,
                       pc_->toString());
  }

private:
  void release() {
    (*pc_)--;
    if (pc_->get() <= 0) {
      delete ptr_;
      ptr_ = nullptr;
    }
    F_DEBUGF("{}", toString());
  }

  detail::PointerCounter *pc_;
  T *ptr_;
};

template <typename T> class uptr : public boost::noncopyable, Stringify {
public:
  explicit uptr(T *ptr = nullptr) : ptr_(ptr) {
    F_DEBUGF("Constructor:{}", toString());
  }

  // move
  uptr(uptr<T> &&up) : uptr() {
    swap(up);
    F_DEBUGF("Move Constructor:{}", toString());
  }

  uptr &operator=(uptr<T> &&up) {
    if (this == &up) {
      F_DEBUGF("Move Assign self:{}", toString());
      return *this;
    }
    swap(up);
    F_DEBUGF("Move Assign:{}", toString());
    return *this;
  }

  virtual ~uptr() {
    release();
    F_DEBUGF("Destructor:{}", toString());
  }

  void reset(T *p) {
    release();
    ptr_ = p;
  }

  void swap(uptr<T> &up) { std::swap(ptr_, up.ptr_); }

  explicit operator bool() const { return ptr_; }
  T &operator*() const { return *ptr_; }
  T *operator->() const { return ptr_; }
  T *get() const { return ptr_; }

  virtual std::string toString() const {
    return fmt::format("[ @uptr ptr_:{} ]", (void *)ptr_);
  }

private:
  void release() {
    if (ptr_) {
      delete ptr_;
      ptr_ = nullptr;
    }
    F_DEBUGF("{}", toString());
  }

  T *ptr_;
};

} // namespace fastype
