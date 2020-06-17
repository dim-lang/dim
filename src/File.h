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

} // namespace detail

class FileReader : public detail::FileInfo {
public:
  using LineIterator = detail::FileReaderLineIterator;
  using CharIterator = detail::FileReaderCharIterator;
  using BufferIterator = detail::FileReaderBufferIterator;

  FileReader(const std::string &fileName);
  virtual ~FileReader() = default;

  // reset reading status
  virtual void reset();

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

class FileWriter : public detail::FileInfo {
public:
  FileWriter(const std::string &fileName);
  virtual ~FileWriter() = default;

  virtual void reset();
  virtual void flush();
  virtual int write(const char *buf, int n);
  virtual int write(const std::string &buf);

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
