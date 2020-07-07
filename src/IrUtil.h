// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include <string>

class IrUtil {
public:
  // "shp.ir."
  static std::string prefix();

  // source code function name translation rule, such as:
  // `format_print` to `shp.ir.format.print`
  // `FormatPrint` to `shp.ir.FormatPrint`
  static std::string toLLVMName(const std::string &name);

  // source code function name reverse translation rule, such as:
  // `shp.ir.format.print` to `format_print`
  // `shp.ir.FormatPrint` to `FormatPrint`
  static std::string fromLLVMName(const std::string &name);
};
