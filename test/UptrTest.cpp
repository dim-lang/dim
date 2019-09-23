// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Logging.h"
#include "SmartPointer.h"
#include "catch2/catch.hpp"
#include <fmt/format.h>
#include <string>

#define TEST_MAX 128

class UptrPeople {
public:
  UptrPeople(const std::string &name, int age)
      : name_(new std::string(name)), age_(new int(age)) {}

  virtual ~UptrPeople() {
    if (name_) {
      delete name_;
      name_ = nullptr;
    }
    if (age_) {
      delete age_;
      age_ = nullptr;
    }
  }

  std::string toString() const {
    return fmt::format("[ @UptrPeople name_:{} {}, age_:{} {} ]", (void *)name_,
                       *name_, (void *)age_, *age_);
  }

  int age() const { return age_ ? *age_ : -1; }
  std::string name() const { return name_ ? *name_ : "null"; }

private:
  std::string *name_;
  int *age_;
};

TEST_CASE("Uptr", "[Uptr]") {

  SECTION("constructor and move") {
    for (int i = 0; i < TEST_MAX; i++) {
      int *t = new int(i);
      fastype::Uptr<int> p1(t);
      REQUIRE(p1);
      REQUIRE(p1.get() != nullptr);
      REQUIRE(p1.get() == t);
      REQUIRE(*p1 == i);
      fastype::Uptr<int> p2(std::move(p1));
      REQUIRE(!p1);
      REQUIRE(p1.get() == nullptr);
      REQUIRE(p2.get() == t);
      REQUIRE(*p2 == i);
      fastype::Uptr<int> p3(std::move(p2));
      REQUIRE(!p2);
      REQUIRE(p2.get() == nullptr);
      REQUIRE(p3.get() == t);
      REQUIRE(*p3 == i);
      fastype::Uptr<int> p4(std::move(p3));
      REQUIRE(!p3);
      REQUIRE(p3.get() == nullptr);
      REQUIRE(p4.get() == t);
      REQUIRE(*p4 == i);
    }
  }

  SECTION("data structure") {
    for (int i = 0; i < TEST_MAX; i++) {
      UptrPeople *p =
          new UptrPeople(std::string("people ") + std::to_string(i), i);
      fastype::Uptr<UptrPeople> p1(p);
      REQUIRE(p1);
      REQUIRE(p1.get() == p);
      REQUIRE(p1->age() == i);
      REQUIRE(p1->name() == std::string("people ") + std::to_string(i));
      fastype::Uptr<UptrPeople> p2(std::move(p1));
      REQUIRE(!p1);
      REQUIRE(p1.get() == nullptr);
      REQUIRE(p2);
      REQUIRE(p2.get() == p);
      REQUIRE(p2->age() == i);
      REQUIRE(p2->name() == std::string("people ") + std::to_string(i));
      fastype::Uptr<UptrPeople> p3(std::move(p2));
      REQUIRE(!p2);
      REQUIRE(p2.get() == nullptr);
      REQUIRE(p3);
      REQUIRE(p3.get() == p);
      REQUIRE(p3->age() == i);
      REQUIRE(p3->name() == std::string("people ") + std::to_string(i));
      fastype::Uptr<UptrPeople> p4(std::move(p3));
      REQUIRE(!p3);
      REQUIRE(p3.get() == nullptr);
      REQUIRE(p4);
      REQUIRE(p4.get() == p);
      REQUIRE(p4->age() == i);
      REQUIRE(p4->name() == std::string("people ") + std::to_string(i));

      {
        fastype::Uptr<UptrPeople> p5 = std::move(p4);
        REQUIRE(!p4);
        REQUIRE(p4.get() == nullptr);
        REQUIRE(p5);
        REQUIRE(p5.get() == p);
        REQUIRE(p5->age() == i);
        REQUIRE(p5->name() == std::string("people ") + std::to_string(i));
      }

      fastype::Uptr<UptrPeople> p6(std::move(p4));
      REQUIRE(!p6);
      REQUIRE(p6.get() == nullptr);
    }
  }
}
