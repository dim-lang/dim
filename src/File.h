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
  Line begin();
  Line end();
  Line getLine(int32_t lineNumber);

  static std::shared_ptr<File> open(const std::string &fileName);
  static void close(std::shared_ptr<File> file);

private:
  File(const std::string &fileName);

  // load n lines
  int64_t load(int n);
  // load 1 line
  int64_t loadOne();
  // load all lines
  int64_t loadAll();

  std::shared_ptr<FILE> fd;
  std::string fileName;
  std::shared_ptr<Logger> log;

  Buffer readBuffer;
  Buffer writeBuffer;
  std::vector<std::shared_ptr<detail::LineImpl>> lineList;

  friend class detail::LineImpl;
};

} // namespace fastype
