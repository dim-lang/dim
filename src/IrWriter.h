// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "IrContext.h"
#include <string>
#include <system_error>
#include <utility>

class IrWriter {
public:
  virtual ~IrWriter() = default;
  virtual void toStdout() = 0;
  virtual void toStderr() = 0;
  virtual std::pair<std::error_code, std::string> toFileOstream() = 0;
  virtual std::string toStringOstream() = 0;
};

class IrLLWriter : public IrWriter {
public:
  IrLLWriter(IrContext *context);
  virtual ~IrLLWriter() = default;
  virtual void toStdout();
  virtual void toStderr();
  virtual std::pair<std::error_code, std::string> toFileOstream();
  virtual std::string toStringOstream();

private:
  IrContext *context_;
};

class IrBitCodeWriter : public IrWriter {
public:
  IrBitCodeWriter(IrContext *context);
  virtual ~IrBitCodeWriter() = default;
  virtual void toStdout();
  virtual void toStderr();
  virtual std::pair<std::error_code, std::string> toFileOstream();
  virtual std::string toStringOstream();

private:
  IrContext *context_;
};
