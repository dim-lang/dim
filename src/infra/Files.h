// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#pragma once
#include "boost/filesystem.hpp"
#include "enum.h"
#include "infra/Cowstr.h"
#include "infra/CycleBuffer.h"
#include <vector>

BETTER_ENUM(FileMode, int,
            // type
            Read = 10000, Write, Append, ReadUpdate, WriteUpdate, AppendUpdate)

class FileReader;
class FileWriter;
class FileAppender;

namespace detail {

class FileInfo {
public:
  FileInfo(const Cowstr &fileName);
  // close fp_
  virtual ~FileInfo();
  virtual const Cowstr &fileName() const;

protected:
  Cowstr fileName_;
  FILE *fp_;
};

class FileWriterImpl : public FileInfo {
public:
  FileWriterImpl(const Cowstr &fileName);
  virtual ~FileWriterImpl();

  // reset writing offset
  virtual void reset(int offset);
  // flush buffer
  virtual int flush();
  virtual int write(const Cowstr &buf);
  virtual int writeln(const Cowstr &buf = "");

protected:
  DynamicCycleBuffer buffer_;
};

} // namespace detail

class FileReader : public detail::FileInfo {
public:
  FileReader(const Cowstr &fileName);
  virtual ~FileReader() = default;
  virtual FileMode mode() const;

  // reset reading offset
  virtual void reset(int offset = 0);

  // read block
  virtual Cowstr read(int n);
  // read all
  virtual Cowstr readall();
  // read char
  virtual Cowstr readc();
  // read line
  virtual Cowstr readln();

private:
  virtual void prepareFor(int n);
  virtual char *prepareUntil(char c);

  DynamicCycleBuffer buffer_;
};

class FileWriter : public detail::FileWriterImpl {
public:
  FileWriter(const Cowstr &fileName);
  virtual ~FileWriter() = default;
  virtual FileMode mode() const;
  virtual void reset(int offset = 0);
};

class FileAppender : public detail::FileWriterImpl {
public:
  FileAppender(const Cowstr &fileName);
  virtual ~FileAppender() = default;
  virtual FileMode mode() const;
  virtual void reset(int offset = 0);
};
