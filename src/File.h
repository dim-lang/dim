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

/**
 * read line by line terminate with '\0', usage:
 *
 * <code>
 * while (lineIterator.hasNext()) {
 *   std::string line = lineIterator.next();
 * }
 * </code>
 */
class FileReaderLineIterator {
public:
  FileReaderLineIterator(FileReader *reader);
  virtual ~FileReaderLineIterator() = default;
  std::string next();
  bool hasNext();

private:
  FileReader *reader_;
  char *linePosition_;
};

/**
 * read char by char, usage:
 *
 * <code>
 * while (charIterator.hasNext()) {
 *   char c = charIterator.next();
 * }
 * </code>
 */
class FileReaderCharIterator {
public:
  FileReaderCharIterator(FileReader *reader);
  virtual ~FileReaderCharIterator() = default;
  char next();
  bool hasNext();

private:
  FileReader *reader_;
};

/**
 * read buffer by buffer, usage:
 *
 * <code>
 * while (bufferIterator.hasNext()) {
 *   std::string buffer = bufferIterator.next(n);
 * }
 * </code>
 */
class FileReaderBufferIterator {
public:
  FileReaderBufferIterator(FileReader *reader);
  virtual ~FileReaderBufferIterator() = default;
  std::string next(int n);
  bool hasNext(int n);

private:
  FileReader *reader_;
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

class FileWriterImpl : public FileInfo {
public:
  FileWriterImpl(const std::string &fileName);
  virtual ~FileWriterImpl() = default;

  // reset writing offset
  virtual void reset(int offset);
  // flush buffer
  virtual void flush();
  virtual void write(const char *buf, int n);
  virtual void write(const std::string &buf);

protected:
  DynamicBuffer buffer_;
};

} // namespace detail

class FileReader : public detail::FileInfo {
public:
  using LineIterator = detail::FileReaderLineIterator;
  using CharIterator = detail::FileReaderCharIterator;
  using BufferIterator = detail::FileReaderBufferIterator;

  FileReader(const std::string &fileName);
  virtual ~FileReader() = default;
  virtual FileModeType mode() const;

  // reset reading offset
  virtual void reset(int offset = 0);

  // reader iterator
  virtual LineIterator beginLine();
  virtual CharIterator beginChar();
  virtual BufferIterator beginBuffer();

private:
  friend class detail::FileReaderLineIterator;
  friend class detail::FileReaderCharIterator;
  friend class detail::FileReaderBufferIterator;

  DynamicBuffer buffer_;
};

class FileWriter : public detail::FileWriterImpl {
public:
  FileWriter(const std::string &fileName);
  virtual ~FileWriter() = default;
  virtual FileModeType mode() const;
  virtual void reset(int offset = 0);
};

class FileAppender : public detail::FileWriterImpl {
public:
  FileAppender(const std::string &fileName);
  virtual ~FileAppender() = default;
  virtual FileModeType mode() const;
  virtual void reset(int offset = 0);
};
