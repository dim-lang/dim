// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "iface/LLVMModular.h"
#include "catch2/catch.hpp"
#include "infra/Log.h"

TEST_CASE("LLVMModular", "[LLVMModular]") {
  SECTION("LLVMModular") {
    LLVMModular a;
    LLVMModular b((llvm::Module *)0x1);
    LLVMModular c((llvm::Module *)0x2);
    REQUIRE(!a.llvmModule());
    REQUIRE(b.llvmModule());
    REQUIRE(c.llvmModule());
    REQUIRE(b.llvmModule() > a.llvmModule());
    REQUIRE(c.llvmModule() > b.llvmModule());
  }
}
