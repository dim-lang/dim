// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "boost/filesystem.hpp"
#include "container/CycleBuffer.h"
#include "enum.h"
#include <string>
#include <vector>

/*================ type start from 3000 ================*/
BETTER_ENUM(FileMode, int,
            // type
            Read = 4000, Write, Append, ReadUpdate, WriteUpdate, AppendUpdate)

class FileReader;
class FileWriter;
class FileAppender;

namespace detail {

class FileInfo {
public:
  FileInfo(const std::string &fileName);
  // close fp_
  virtual ~FileInfo();
  const std::string &fileName() const;

protected:
  std::string fileName_;
  FILE *fp_;
};

class FileWriterImpl : public FileInfo {
public:
  FileWriterImpl(const std::string &fileName);
  virtual ~FileWriterImpl();

  // reset writing offset
  virtual void reset(int offset);
  // flush buffer
  virtual int flush();
  virtual int write(const char *buf, int n);
  virtual int write(const std::string &buf);
  virtual int writeln(const char *buf, int n);
  virtual int writeln(const std::string &buf = "");

protected:
  DynamicBuffer buffer_;
};

} // namespace detail

class FileReader : public detail::FileInfo {
public:
  FileReader(const std::string &fileName);
  virtual ~FileReader() = default;
  virtual FileMode mode() const;

  // reset reading offset
  virtual void reset(int offset = 0);

  // read block
  virtual std::string read(int n);
  virtual int read(char *buf, int n);
  // read all
  virtual std::string readall();
  virtual int readall(char *buf, int n);
  // read char
  virtual std::string readc();
  virtual int readc(char &c);
  // read line
  virtual std::string readln();
  virtual int readln(char *buf, int n);

private:
  virtual void prepareFor(int n);
  virtual char *prepareUntil(char c);

  DynamicBuffer buffer_;
};

class FileWriter : public detail::FileWriterImpl {
public:
  FileWriter(const std::string &fileName);
  virtual ~FileWriter() = default;
  virtual FileMode mode() const;
  virtual void reset(int offset = 0);
};

class FileAppender : public detail::FileWriterImpl {
public:
  FileAppender(const std::string &fileName);
  virtual ~FileAppender() = default;
  virtual FileMode mode() const;
  virtual void reset(int offset = 0);
};
