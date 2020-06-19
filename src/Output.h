// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "Ir.h"
#include <string>
#include <system_error>
#include <utility>

std::error_code output_fd(IrTranslateUnit *tunit, const std::string &fileName);

std::string output_string(IrTranslateUnit *tunit);
