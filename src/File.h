// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "boost/filesystem.hpp"
#include "container/CycleBuffer.h"
#include "enum.h"
#include <string>
#include <vector>

/*================ type start from 3000 ================*/
BETTER_ENUM(FileModeType, int,
            // type
            Read = 4000, Write, Append, ReadUpdate, WriteUpdate, AppendUpdate)

class FileReader;
class FileWriter;
class FileAppender;

namespace detail {

// read line by line terminate with '\0'
class FileReaderLineIterator {
public:
  virtual ~FileReaderLineIterator() = default;

  std::string operator*();
  FileReaderLineIterator &operator++();
  FileReaderLineIterator operator++(int);

private:
  FileReaderLineIterator(FILE *fp);
  friend class FileReader;
  FILE *fp_;
};

// read char by char
class FileReaderCharIterator {
public:
  virtual ~FileReaderCharIterator() = default;

  char operator*();
  FileReaderCharIterator &operator++();
  FileReaderCharIterator operator++(int);

private:
  FileReaderCharIterator(FILE *fp);
  friend class FileReader;
  FILE *fp_;
};

// read buffer by buffer
class FileReaderBufferIterator {
public:
  virtual ~FileReaderBufferIterator() = default;

  const char *operator*();
  FileReaderBufferIterator &operator++();
  FileReaderBufferIterator operator++(int);

private:
  FileReaderBufferIterator(FILE *fp);
  friend class FileReader;
  FILE *fp_;
};

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

} // namespace detail

class FileReader : public detail::FileInfo {
public:
  using LineIterator = detail::FileReaderLineIterator;
  using CharIterator = detail::FileReaderCharIterator;
  using BufferIterator = detail::FileReaderBufferIterator;

  FileReader(const std::string &fileName);
  virtual ~FileReader();

  // reset reading status
  void reset();
  LineIterator beginLine();
  CharIterator beginChar();
  BufferIterator beginBuffer();
  LineIterator endLine();
  CharIterator endChar();
  BufferIterator endBuffer();

  friend class detail::FileReaderLineIterator;
  friend class detail::FileReaderCharIterator;
  friend class detail::FileReaderBufferIterator;

private:
  DynamicBuffer buffer_;
};

class FileWriter : public detail::FileInfo {
public:
  FileWriter(const std::string &fileName);
  virtual ~FileWriter();

private:
  DynamicBuffer buffer_;
};

class FileAppender : public detail::FileInfo {
public:
  FileAppender(const std::string &fileName);
  virtual ~FileAppender();

private:
  DynamicBuffer buffer_;
};

class File {
public:
  // read all into one string
  static std::string read(const std::string &fileName);

  // read all to line by line
  static std::vector<std::string> readline(const std::string &fileName);

  // write all into one file
  static int write(const std::string &fileName, const std::string &text);

  // write all lines into one file
  static int writeline(const std::string &fileName,
                       const std::vector<std::string> &lines);

  // write all into one file
  static int append(const std::string &fileName, const std::string &text);

  // write all lines into one file
  static int appendline(const std::string &fileName,
                        const std::vector<std::string> &lines);
};
