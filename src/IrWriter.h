// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#pragma once
#include "IrContext.h"
#include "llvm/Target/TargetMachine.h"
#include <string>

class IrWriter {
public:
  virtual ~IrWriter() = default;
  virtual void toStdout() = 0;
  virtual void toStderr() = 0;
  virtual std::string toFileOstream() = 0;
  virtual std::string toStringOstream() = 0;
};

class IrLLWriter : public IrWriter {
public:
  IrLLWriter(IrContext *context);
  virtual ~IrLLWriter() = default;
  virtual void toStdout();
  virtual void toStderr();
  virtual std::string toFileOstream();
  virtual std::string toStringOstream();

private:
  IrContext *context_;
};

class IrObjectWriter : public IrWriter {
public:
  IrObjectWriter(IrContext *context);
  virtual ~IrObjectWriter() = default;
  virtual void toStdout();
  virtual void toStderr();
  virtual std::string toFileOstream();
  virtual std::string toStringOstream();

private:
  IrContext *context_;
  llvm::TargetMachine *targetMachine_;
};
