// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Logging.h"
#include "SmartPointer.h"
#include "catch2/catch.hpp"
#include <fmt/format.h>
#include <string>

#define TEST_N 128

class SptrPeople {
public:
  SptrPeople(const std::string &name, int age)
      : name_(new std::string(name)), age_(new int(age)) {}

  virtual ~SptrPeople() {
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
    return fmt::format("[ @SptrPeople name_:{} {}, age_:{} {} ]", (void *)name_,
                       *name_, (void *)age_, *age_);
  }

  int age() const { return age_ ? *age_ : -1; }
  std::string name() const { return name_ ? *name_ : "null"; }

private:
  std::string *name_;
  int *age_;
};

TEST_CASE("Sptr", "[Sptr]") {

  SECTION("constructor and copy") {
    for (int i = 0; i < TEST_N; i++) {
      int *t = new int(i);
      fastype::Sptr<int> p1(t);
      REQUIRE(p1);
      REQUIRE(p1.useCount() == 1);
      fastype::Sptr<int> p2(p1);
      fastype::Sptr<int> p3(p1);
      fastype::Sptr<int> p4(p3);
      REQUIRE(p1.useCount() == 4);
      REQUIRE(p2.useCount() == 4);
      REQUIRE(p3.useCount() == 4);
      REQUIRE(p4.useCount() == 4);
      REQUIRE(p1.get() == p2.get());
      REQUIRE(p2.get() == p3.get());
      REQUIRE(p3.get() == p4.get());
      REQUIRE(*p1 == i);
      REQUIRE(*p2 == i);
      REQUIRE(*p3 == i);
      REQUIRE(*p4 == i);
    }
  }

  SECTION("move") {
    for (int i = 0; i < TEST_N; i++) {
      int *t = new int(i);
      fastype::Sptr<int> p1(t);
      REQUIRE(p1);
      REQUIRE(p1.useCount() == 1);
      fastype::Sptr<int> p2(std::move(p1));
      fastype::Sptr<int> p3(std::move(p2));
      fastype::Sptr<int> p4(std::move(p3));
      REQUIRE(p1.useCount() == 1);
      REQUIRE(p2.useCount() == 1);
      REQUIRE(p3.useCount() == 1);
      REQUIRE(p4.useCount() == 1);
      REQUIRE(p1.get() == nullptr);
      REQUIRE(p2.get() == nullptr);
      REQUIRE(p3.get() == nullptr);
      REQUIRE(p4.get() != nullptr);
      REQUIRE(*p4 == i);

      fastype::Sptr<int> p5 = std::move(p4);
      REQUIRE(p4.useCount() == 1);
      REQUIRE(p5.useCount() == 1);
      REQUIRE(p4.get() == nullptr);
      REQUIRE(p5.get() != nullptr);
    }
  }

  SECTION("data structure") {
    for (int i = 0; i < TEST_N; i++) {
      SptrPeople *p = new SptrPeople(std::to_string(i), i);
      fastype::Sptr<SptrPeople> p1(p);
      REQUIRE(p1);
      REQUIRE(p1.useCount() == 1);
      REQUIRE(p1->age() == i);
      REQUIRE(p1->name() == std::to_string(i));
      fastype::Sptr<SptrPeople> p2(p1);
      fastype::Sptr<SptrPeople> p3(p2);
      fastype::Sptr<SptrPeople> p4(p3);
      REQUIRE(p1.useCount() == 4);
      REQUIRE(p2.useCount() == 4);
      REQUIRE(p3.useCount() == 4);
      REQUIRE(p4.useCount() == 4);
      REQUIRE(p1.get() != nullptr);
      REQUIRE(p1.get() == p2.get());
      REQUIRE(p2.get() == p3.get());
      REQUIRE(p3.get() == p4.get());
      REQUIRE((*p1).age() == i);
      REQUIRE((*p1).name() == std::to_string(i));
      REQUIRE((*p2).age() == i);
      REQUIRE((*p2).name() == std::to_string(i));
      REQUIRE((*p3).age() == i);
      REQUIRE((*p3).name() == std::to_string(i));
      REQUIRE((*p4).age() == i);
      REQUIRE((*p4).name() == std::to_string(i));

      {
        fastype::Sptr<SptrPeople> p5 = p4;
        REQUIRE(p1.useCount() == 5);
        REQUIRE(p2.useCount() == 5);
        REQUIRE(p3.useCount() == 5);
        REQUIRE(p4.useCount() == 5);
        REQUIRE(p5.useCount() == 5);
        REQUIRE(p4.get() != nullptr);
        REQUIRE(p4.get() == p5.get());
        REQUIRE((*p4).age() == i);
        REQUIRE((*p4).name() == std::to_string(i));
        REQUIRE((*p5).age() == i);
        REQUIRE((*p5).name() == std::to_string(i));
      }

      REQUIRE(p1.useCount() == 4);
      REQUIRE(p2.useCount() == 4);
      REQUIRE(p3.useCount() == 4);
      REQUIRE(p4.useCount() == 4);
    }
  }

  SECTION("reset") {
    for (int i = 0; i < TEST_N; i++) {
      fastype::Sptr<SptrPeople> p1(
          new SptrPeople(std::string("hello ") + std::to_string(i), i / 2 - i));
      SptrPeople *p2 = new SptrPeople(
          std::string("goodbye ") + std::to_string(i), i * 2 + i);
      REQUIRE(p1.useCount() == 1);
      REQUIRE(p1->age() == i / 2 - i);
      REQUIRE(p1->name() == std::string("hello ") + std::to_string(i));
      p1.reset();
      REQUIRE(p1.useCount() == 0);
      REQUIRE(p1.get() == nullptr);
      p1.reset(p2);
      REQUIRE(p1.useCount() == 1);
      REQUIRE(p1->age() == i * 2 + i);
      REQUIRE(p1->name() == std::string("goodbye ") + std::to_string(i));
    }
  }

  SECTION("swap") {
    for (int i = 0; i < TEST_N; i++) {
      fastype::Sptr<SptrPeople> p1(
          new SptrPeople(std::string("hello ") + std::to_string(i), i / 2 - i));
      fastype::Sptr<SptrPeople> p2(new SptrPeople(
          std::string("goodbye ") + std::to_string(i), i * 2 + i));
      REQUIRE(p1.useCount() == 1);
      REQUIRE(p1->age() == i / 2 - i);
      REQUIRE(p1->name() == std::string("hello ") + std::to_string(i));
      REQUIRE(p2.useCount() == 1);
      REQUIRE(p2->age() == i * 2 + i);
      REQUIRE(p2->name() == std::string("goodbye ") + std::to_string(i));
      p1.swap(p2);
      REQUIRE(p2.useCount() == 1);
      REQUIRE(p2->age() == i / 2 - i);
      REQUIRE(p2->name() == std::string("hello ") + std::to_string(i));
      REQUIRE(p1.useCount() == 1);
      REQUIRE(p1->age() == i * 2 + i);
      REQUIRE(p1->name() == std::string("goodbye ") + std::to_string(i));
    }
  }
}
