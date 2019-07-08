// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Line.h"
#include "Log.h"
#include "boost/core/noncopyable.hpp"
#include "unicode/ustring.h"
#include <cstdio>
#include <cstring>
#include <memory>
#include <vector>

namespace fastype {

class File : private boost::noncopyable {
public:
  virtual ~File();
  File(File &&) = default;
  File &operator=(File &&) = default;

  const std::string &getFileName() const;
  LineIterator begin();
  LineIterator end();
  LineIterator getLine(int32_t lineNumber);

  static std::shared_ptr<File> open(const std::string &fileName);
  static void close(std::shared_ptr<File> file);

  class LineIterator {
  public:
    virtual ~LineIterator() = default;

    bool hasNext() const { return iter != fp->end(); }
    LineIterator next() { return iter + 1; }
    bool hasPrevious() const { return fp->begin() != iter; }
    LineIterator previous() { return iter - 1; }

  private:
    LineIterator(std::shared_ptr<File> fp,
                 std::vector<std::shared_ptr<Line>>::iterator iter)
        : fp(fp), iter(iter) {}

    std::vector<std::shared_ptr<Line>>::iterator iter;
    std::shared_ptr<File> fp;
  };

private:
  File(const std::string &fileName);

  int64_t load(int count);
  int64_t loadOne();
  int64_t loadAll();

  FILE *fd;
  std::string fileName;
  std::shared_ptr<Logger> log;

  Buffer readBuffer;
  Buffer writeBuffer;
  std::vector<std::shared_ptr<Line>> lineList;

  friend class LineIterator;
};

} // namespace fastype
