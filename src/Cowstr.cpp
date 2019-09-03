// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Cowstr.h"
#include "Logging.h"
#include "Profile.h"
#include <algorithm>
#include <boost/align/align_up.hpp>
#include <fmt/format.h>
#include <utility>

#define F_ALIGN_UP 16

namespace fastype {

Cowstr::Cowstr() : impl_(new CowStrImpl()), ref_(new int()) {
  std::memset(impl_, 0, sizeof(CowStrImpl));
  F_DEBUGF("Constructor:{}", toString());
}

Cowstr::Cowstr(int capacity) : Cowstr() {
  expand(capacity);
  F_DEBUGF("Capacity Constructor:{}", toString());
}

Cowstr::~Cowstr() { release(); }

Cowstr::Cowstr(const Cowstr &other) : impl_(other.impl_), ref_(other.ref_) {
  incref();
  F_DEBUGF("Copy Constructor: {}", toString());
}

Cowstr &Cowstr::operator=(const Cowstr &other) {
  if (this == &other) {
    return *this;
  }
  impl_ = other.impl_;
  ref_ = other.ref_;
  incref();
  F_DEBUGF("Copy Assign: {}", toString());
  return *this;
}

Cowstr::Cowstr(Cowstr &&other) : impl_(other.impl_), ref_(other.ref_) {
  incref();
  F_DEBUGF("Move Constructor: {}", toString());
}

Cowstr &Cowstr::operator=(Cowstr &&other) {
  if (this == &other) {
    return *this;
  }
  impl_ = other.impl_;
  ref_ = other.ref_;
  incref();
  F_DEBUGF("Copy Move : {}", toString());
  return *this;
}

CowStrImpl *Cowstr::create(int capacity) {
  F_CHECKF(capacity > 0, "capacity {} > 0", capacity);

  int newCapacity = std::max<int>(
      F_ALIGN_UP, (int)boost::alignment::align_up(capacity, F_ALIGN_UP));
  F_DEBUGF("capacity:{} newCapacity:{}", capacity, newCapacity);
  F_CHECKF(newCapacity >= 2 * capacityImpl(),
           "newCapacity {} >= 2 * capacityImpl {}", newCapacity,
           capacityImpl());
  char *newData = (char *)realloc(dataImpl(), newCapacity * sizeof(char));
  if (!newData) {
    return *this;
  }
  dataImpl() = newData;
  capacityImpl() = newCapacity;
  F_DEBUGF("after expand:{}", toString());
  return *this;
}

bool Cowstr::empty() const { return size_ <= 0; }

Cowstr &Cowstr::clear() {
  if (data_) {
    std::memset(data_, 0, capacity_ * sizeof(char));
    size_ = 0;
  }
  return *this;
}

void Cowstr::release() {
  decref();
  if (refImpl() <= 0) {
    delete impl_;
  }
  impl_ = nullptr;
  return *this;
}

Cowstr Cowstr::subString(int start, int length) const {
  F_CHECKF(start >= 0, "start {} >= 0", start);
  F_CHECKF(length > 0, "length {} > 0", length);
  if (!data_) {
    return;
  }
  if (start > size_) {
    return;
  }
  length = std::min<int>(length, size_ - start);
  std::memmove(data_ + start, data_ + start + length, length);
  size_ -= length;
  std::memset(data_ + size_, 0, (capacity_ - size_) * sizeof(char));
}

Cowstr Cowstr::trim() const {
  trimLeft();
  trimRight();
}

void Cowstr::trimLeft() {
  F_CHECKF(length >= 0, "length {} >= 0", length);
  if (!data_) {
    return;
  }
  length = std::min<int>(length, size_);
  std::memmove(data_, data_ + length, size_ - length);
  size_ -= length;
  std::memset(data_ + size_, 0, (capacity_ - size_) * sizeof(char));
}

void Cowstr::trimRight() {
  F_CHECKF(length >= 0, "length {} >= 0", length);
  if (!data_) {
    return;
  }
  length = std::min<int>(length, size_);
  size_ -= length;
  std::memset(data_ + size_, 0, (capacity_ - size_) * sizeof(char));
}

char *Cowstr::data() { return data_; }

const char *Cowstr::data() const { return data_; }

char &Cowstr::operator[](int index) {
  F_CHECKF(index >= 0, "index {} >= 0", index);
  F_CHECKF(index < capacity_, "index {} < capacity_ {}", index, capacity_);
  return data_[index];
}

const char &Cowstr::operator[](int index) const {
  F_CHECKF(index >= 0, "index {} >= 0", index);
  F_CHECKF(index < capacity_, "index {} < capacity_ {}", index, capacity_);
  return data_[index];
}

int Cowstr::size() const {
  F_CHECKF(size_ >= 0, "size_ {} >= 0", size_);
  return size_;
}

void Cowstr::setSize(int size) {
  F_CHECKF(size >= 0, "size {} >= 0", size);
  F_CHECKF(size_ >= 0, "size_ {} >= 0", size_);
  size_ = size;
}

void Cowstr::incSize(int update) {
  F_CHECKF(update > 0, "update {} > 0", update);
  F_CHECKF(size_ + update <= capacity_, "size_ {} + update {} <= capacity_ {}",
           size_, update, capacity_);
  size_ += update;
}

void Cowstr::decSize(int update) {
  F_CHECKF(update > 0, "update {} > 0", update);
  F_CHECKF(size_ - update >= 0, "size_ {} - update {} >= 0", size_, update);
  size_ -= update;
}

int Cowstr::capacity() const {
  F_CHECKF(capacity_ >= 0, "capacity_ {} >= 0", capacity_);
  return capacity_;
}

std::string Cowstr::toString() const {
  return fmt::format("[ @Cowstr data_:{} size_:{} capacity_:{} ref_:{} ]",
                     (void *)impl_->data, impl_->size, impl_->capacity_, ref_);
}

const int &Cowstr::refcount() const { return *ref_; }

void Cowstr::incref() const { *ref_ += 1; }

void Cowstr::decref() const { *ref_ -= 1; }

char *&Cowstr::dataImpl() { return impl_->data; }

int &Cowstr::sizeImpl() { return impl_->size; }

int &Cowstr::capacityImpl() { return impl_->capacity; }

int &Cowstr::refImpl() { return *ref_; }

} // namespace fastype

#undef F_ALIGN_UP
