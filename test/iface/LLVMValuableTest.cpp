// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "iface/LLVMValuable.h"
#include "catch2/catch.hpp"
#include "infra/Log.h"

TEST_CASE("LLVMValuable", "[LLVMValuable]") {
  SECTION("LLVMValuable") {
    LLVMValuable a;
    LLVMValuable b((llvm::Value *)0x1);
    LLVMValuable c((llvm::Value *)0x2);
    REQUIRE(!a.llvmValue());
    REQUIRE(b.llvmValue());
    REQUIRE(c.llvmValue());
    REQUIRE(b.llvmValue() > a.llvmValue());
    REQUIRE(c.llvmValue() > b.llvmValue());
  }
}
