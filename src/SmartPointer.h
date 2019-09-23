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

template <typename T> class Sptr : public Stringify {
public:
  explicit Sptr(T *ptr = nullptr)
      : ptr_(ptr), pc_(new detail::PointerCounter()) {
    (*pc_)++;
    F_INFOF("Constructor:{}", toString());
  }

  // copy
  Sptr(const Sptr<T> &sp) : ptr_(nullptr), pc_(nullptr) {
    ptr_ = sp.ptr_;
    pc_ = sp.pc_;
    (*pc_)++;
    F_INFOF("Copy Constructor:{}", toString());
  }

  Sptr &operator=(const Sptr<T> &sp) {
    if (this == &sp) {
      F_INFOF("Copy Assign self:{}", toString());
      return *this;
    }
    (*pc_)--;
    if (pc_->get() <= 0) {
      F_CHECKF(pc_->get() == 0, "pc_->get {} == 0", pc_->get());
      releasePtr();
      delete pc_;
      pc_ = nullptr;
    }
    ptr_ = sp.ptr_;
    pc_ = sp.pc_;
    F_CHECKF(pc_, "pc_ {} != nullptr", (void *)pc_);
    (*pc_)++;
    F_CHECKF(pc_->get() >= 1, "pc_->get {} >= 1", pc_->get());
    F_INFOF("Copy Assign:{}", toString());
    return *this;
  }

  // move
  Sptr(Sptr<T> &&sp) : Sptr() {
    swap(sp);
    F_INFOF("Move Constructor:{}", toString());
  }

  Sptr &operator=(Sptr<T> &&sp) {
    if (this == &sp) {
      F_INFOF("Move Assign self:{}", toString());
      return *this;
    }
    swap(sp);
    F_INFOF("Move Assign:{}", toString());
    return *this;
  }

  virtual ~Sptr() {
    (*pc_)--;
    if (pc_->get() <= 0) {
      F_CHECKF(pc_->get() == 0, "pc_->get {} == 0", pc_->get());
      releasePtr();
      delete pc_;
      pc_ = nullptr;
    }
    F_INFOF("Destructor:{}", toString());
  }

  int32_t useCount() {
    F_INFOF("{}", toString());
    return pc_->get();
  }

  void reset(T *p = nullptr) {
    (*pc_)--;
    if (pc_->get() <= 0) {
      releasePtr();
    }
    ptr_ = p;
    pc_->reset();
    if (p) {
      (*pc_)++;
    }
    F_INFOF("{}", toString());
  }

  void swap(Sptr<T> &sp) {
    std::swap(ptr_, sp.ptr_);
    std::swap(pc_, sp.pc_);
    F_INFOF("{}", toString());
  }

  explicit operator bool() const { return ptr_; }
  T &operator*() const { return *ptr_; }
  T *operator->() const { return ptr_; }
  T *get() const { return ptr_; }

  virtual std::string toString() const {
    return pc_ ? fmt::format("[ @Sptr ptr_:{} pc_:{} ]", (void *)ptr_,
                             pc_->toString())
               : fmt::format("[ @Sptr ptr_:{} pc_:{} ]", (void *)ptr_,
                             (void *)pc_);
  }

private:
  inline void releasePtr() {
    if (ptr_) {
      delete ptr_;
      ptr_ = nullptr;
    }
  }

  T *ptr_;
  detail::PointerCounter *pc_;
}; // Sptr

template <typename T> class Uptr : public boost::noncopyable, Stringify {
public:
  explicit Uptr(T *ptr = nullptr) : ptr_(ptr) {
    F_INFOF("Constructor:{}", toString());
  }

  // disable copy
  Uptr(const Uptr<T> &) = delete;
  Uptr &operator=(const Uptr<T> &) = delete;

  // move
  Uptr(Uptr<T> &&up) : Uptr() {
    F_INFOF("Move Constructor enter:{}", toString());
    swap(up);
    F_INFOF("Move Constructor:{}", toString());
  }

  Uptr &operator=(Uptr<T> &&up) {
    F_INFOF("Move Assign enter:{}", toString());
    if (this == &up) {
      F_INFOF("Move Assign self:{}", toString());
      return *this;
    }
    swap(up);
    F_INFOF("Move Assign:{}", toString());
    return *this;
  }

  virtual ~Uptr() {
    F_INFOF("Destructor enter:{}", toString());
    release();
    F_INFOF("Destructor:{}", toString());
  }

  void reset(T *p = nullptr) {
    F_INFOF("enter:{}", toString());
    release();
    ptr_ = p;
    F_INFOF("{}", toString());
  }

  void swap(Uptr<T> &up) {
    F_INFOF("enter:{}", toString());
    std::swap(ptr_, up.ptr_);
    F_INFOF("{}", toString());
  }

  explicit operator bool() const { return ptr_; }
  T &operator*() const { return *ptr_; }
  T *operator->() const { return ptr_; }
  T *get() const { return ptr_; }

  virtual std::string toString() const {
    return fmt::format("[ @Uptr ptr_:{} ]", (void *)ptr_);
  }

private:
  void release() {
    F_INFOF("enter:{}", toString());
    if (ptr_) {
      delete ptr_;
      ptr_ = nullptr;
    }
    F_INFOF("{}", toString());
  }

  T *ptr_;
}; // Uptr

} // namespace fastype
