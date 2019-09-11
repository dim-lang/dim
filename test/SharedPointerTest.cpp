// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "SmartPointer.h"
#include "catch2/catch.hpp"
#include <fmt/format.h>
#include <string>

TEST_CASE("SmartPointer", "[SmartPointer]") {

  SECTION("constructor and copy") {
    for (int i = 0; i < 1024; i++) {
      int *t = new int(i);
      fastype::sptr<int> p1(t);
      REQUIRE(p1);
      REQUIRE(p1.useCount() == 1);
      fastype::sptr<int> p2(p1);
      fastype::sptr<int> p3(p1);
      fastype::sptr<int> p4(p3);
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
    for (int i = 0; i < 1024; i++) {
      int *t = new int(i);
      fastype::sptr<int> p1(t);
      REQUIRE(p1);
      REQUIRE(p1.useCount() == 1);
      fastype::sptr<int> p2(std::move(p1));
      fastype::sptr<int> p3(std::move(p2));
      fastype::sptr<int> p4(std::move(p3));
      REQUIRE(p1.useCount() == 0);
      REQUIRE(p2.useCount() == 0);
      REQUIRE(p3.useCount() == 0);
      REQUIRE(p4.useCount() == 1);
      REQUIRE(p1.get() == nullptr);
      REQUIRE(p1.get() == p2.get());
      REQUIRE(p2.get() == p3.get());
      REQUIRE(p4.get() != nullptr);
      REQUIRE(*p4 == i);

      fastype::sptr<int> p5 = std::move(p4);
      REQUIRE(p4.useCount() == 0);
      REQUIRE(p5.useCount() == 1);
      REQUIRE(p4.get() == nullptr);
      REQUIRE(p5.get() != nullptr);
    }
  }

  SECTION("data structure") {
    class People {
    public:
      People(const std::string &name, int age)
          : name_(new std::string(name)), age_(new int(age)) {}

      virtual ~People() {
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
        return fmt::format("[ @People name_:{} {}, age_:{} {} ]", (void *)name_,
                           *name_, (void *)age_, *age_);
      }

      int age() const { return age_ ? *age_ : -1; }
      std::string name() const { return name_ ? *name_ : "null"; }

    private:
      std::string *name_;
      int *age_;
    };

    for (int i = 0; i < 1024; i++) {
      People *p = new People(std::to_string(i), i);
      fastype::sptr<People> p1(p);
      REQUIRE(p1);
      REQUIRE(p1.useCount() == 1);
      REQUIRE(p1->age() == i);
      REQUIRE(p1->name() == std::to_string(i));
      fastype::sptr<People> p2(p1);
      fastype::sptr<People> p3(p2);
      fastype::sptr<People> p4(p3);
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
        fastype::sptr<People> p5 = p4;
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
}
