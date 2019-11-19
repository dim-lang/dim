// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Logging.h"
#include "Stringify.h"
#include <boost/core/noncopyable.hpp>
#include <cstdint>
#include <fmt/format.h>
#include <utility>

namespace fastype {

namespace detail {

class PointerCounter : public boost::noncopyable, Stringify {
public:
  PointerCounter() : counter_(0) {}
  virtual ~PointerCounter() {}
  void reset() { counter_ = 0; }
  void swap(PointerCounter &pc) { std::swap(counter_, pc.counter_); }
  int32_t get() const { return counter_; }
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
  }

  // copy
  Sptr(const Sptr<T> &sp) : ptr_(nullptr), pc_(nullptr) {
    ptr_ = sp.ptr_;
    pc_ = sp.pc_;
    (*pc_)++;
  }

  Sptr &operator=(const Sptr<T> &sp) {
    if (this == &sp) {
      return *this;
    }
    (*pc_)--;
    if (pc_->get() <= 0) {
      F_CHECK(pc_->get() == 0, "pc_->get {} == 0", pc_->get());
      releasePtr();
      delete pc_;
      pc_ = nullptr;
    }
    ptr_ = sp.ptr_;
    pc_ = sp.pc_;
    F_CHECK(pc_, "pc_ {} != nullptr", (void *)pc_);
    (*pc_)++;
    F_CHECK(pc_->get() >= 1, "pc_->get {} >= 1", pc_->get());
    return *this;
  }

  bool operator==(const Sptr<T> &sp) const { return ptr_ == sp.ptr_; }
  bool operator!=(const Sptr<T> &sp) const { return ptr_ != sp.ptr_; }
  bool operator<(const Sptr<T> &sp) const {
    return (char *)ptr_ < (char *)sp.ptr_;
  }
  bool operator<=(const Sptr<T> &sp) const {
    return (char *)ptr_ <= (char *)sp.ptr_;
  }
  bool operator>(const Sptr<T> &sp) const {
    return (char *)ptr_ > (char *)sp.ptr_;
  }
  bool operator>=(const Sptr<T> &sp) const {
    return (char *)ptr_ >= (char *)sp.ptr_;
  }

  // move
  Sptr(Sptr<T> &&sp) : Sptr() { swap(sp); }

  Sptr &operator=(Sptr<T> &&sp) {
    if (this == &sp) {
      return *this;
    }
    swap(sp);
    return *this;
  }

  virtual ~Sptr() {
    (*pc_)--;
    if (pc_->get() <= 0) {
      F_CHECK(pc_->get() == 0, "pc_#get {} == 0", pc_->get());
      releasePtr();
      delete pc_;
      pc_ = nullptr;
    }
  }

  int32_t useCount() const { return pc_->get(); }

  // release the old resource if reference count = 0
  // bind with the new resource and set reference count = 1
  void reset(T *p = nullptr) {
    (*pc_)--;
    if (pc_->get() <= 0) {
      releasePtr();
      pc_->reset();
    } else {
      pc_ = new detail::PointerCounter();
    }
    ptr_ = p;
    // if (p) {
    (*pc_)++;
    //}
  }

  void swap(Sptr<T> &sp) {
    std::swap(ptr_, sp.ptr_);
    std::swap(pc_, sp.pc_);
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
  explicit Uptr(T *ptr = nullptr) : ptr_(ptr) {}

  // disable copy
  Uptr(const Uptr<T> &) = delete;
  Uptr &operator=(const Uptr<T> &) = delete;

  bool operator==(const Uptr<T> &sp) const { return ptr_ == sp.ptr_; }
  bool operator!=(const Uptr<T> &sp) const { return ptr_ != sp.ptr_; }
  bool operator<(const Uptr<T> &sp) const {
    return (char *)ptr_ < (char *)sp.ptr_;
  }
  bool operator<=(const Uptr<T> &sp) const {
    return (char *)ptr_ <= (char *)sp.ptr_;
  }
  bool operator>(const Uptr<T> &sp) const {
    return (char *)ptr_ > (char *)sp.ptr_;
  }
  bool operator>=(const Uptr<T> &sp) const {
    return (char *)ptr_ >= (char *)sp.ptr_;
  }

  // move
  Uptr(Uptr<T> &&up) : Uptr() { swap(up); }

  Uptr &operator=(Uptr<T> &&up) {
    if (this == &up) {
      return *this;
    }
    swap(up);
    return *this;
  }

  virtual ~Uptr() { release(); }

  void reset(T *p = nullptr) {
    release();
    ptr_ = p;
  }

  void swap(Uptr<T> &up) { std::swap(ptr_, up.ptr_); }

  explicit operator bool() const { return ptr_; }
  T &operator*() const { return *ptr_; }
  T *operator->() const { return ptr_; }
  T *get() const { return ptr_; }

  virtual std::string toString() const {
    return fmt::format("[ @Uptr ptr_:{} ]", (void *)ptr_);
  }

private:
  void release() {
    if (ptr_) {
      delete ptr_;
      ptr_ = nullptr;
    }
  }

  T *ptr_;
}; // Uptr

} // namespace fastype
