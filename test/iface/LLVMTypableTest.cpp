// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "iface/LLVMTypable.h"
#include "catch2/catch.hpp"
#include "infra/Log.h"

TEST_CASE("LLVMTypable", "[LLVMTypable]") {
  SECTION("LLVMTypable") {
    LLVMTypable a;
    LLVMTypable b((llvm::Type *)0x1);
    LLVMTypable c((llvm::Type *)0x2);
    REQUIRE(!a.llvmType());
    REQUIRE(b.llvmType());
    REQUIRE(c.llvmType());
    REQUIRE(b.llvmType() > a.llvmType());
    REQUIRE(c.llvmType() > b.llvmType());
  }
}
