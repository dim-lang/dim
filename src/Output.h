// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "Ir.h"
#include "IrContext.h"
#include "llvm/Support/raw_ostream.h"
#include <string>
#include <system_error>
#include <utility>

#if 0
std::error_code output_fd(IrTranslateUnit *tunit, const std::string &fileName);

std::string output_string(IrTranslateUnit *tunit);
#endif

llvm::raw_ostream &output_os(IrContext *context, const llvm::raw_ostream &os);

std::error_code output_file(IrContext *context, const std::string &fileName);

std::string output_string(IrContext *context);
