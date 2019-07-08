// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "File.h"
#include "Line.h"
#include "Log.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <string>

#define BUF_SIZE 4096
#define F_LI detail::LineImpl

namespace fastype {

File::File(const std::string &fileName)
    : fileName(fileName), readBuffer(BUF_SIZE), writeBuffer(BUF_SIZE) {
  log = LogManager::getLogger(fileName);
  fd = std::make_shared<File>(std::fopen(fileName.data(), "rw"));
  F_DEBUGF(log, "fileName: {}", fileName);
}

File::~File() {
  readBuffer.release();
  writeBuffer.release();
  if (fd) {
    std::fclose(fd.get());
    fd.reset();
  }
  std::for_each(buffer.begin(), buffer.end(),
                [](std::vector<std::shared_ptr<detail::LineImpl>>::iterator i) {
                  (*i).reset();
                });
  buffer.clear();
  F_DEBUGF(log, "fileName: {}", fileName);
}

const std::string &File::getFileName() const { return fileName; }

std::shared_ptr<File> File::open(const std::string &fileName) {
  return std::shared_ptr(new File(fileName));
}

void File::close(std::shared_ptr<File> file) { file.reset(); }

Line File::begin() {
  return Line(lineList.begin(), std::shared_ptr<File>(this));
}

Line File::end() { return Line(lineList.end(), std::shared_ptr<File>(this)); }

std::shared_ptr<Line> File::getLine(int32_t lineNumber) {
  loadAll();
  int32_t startBuffer = 0;
  int32_t startByte = 0;
  int32_t endBuffer = 0;
  int32_t endByte = 0;
  return Line(std::shared_ptr<File>(this), startBuffer, startByte, endBuffer,
              endByte);
}

int64_t File::loadOne() {
  std::shared_ptr<detail::LineImpl> lineImpl =
      std::shared_ptr<detail::LineImpl>(
          new detail::LineImpl(std::shared_ptr<File>(this), lineList.size()));
  if (!readBuffer.empty()) {
  }
  int64_t n = (int64_t)std::fread(readBuffer.getData(), readBuffer.getMargin(),
                                  sizeof(char), this->fd.get());
}

} // namespace fastype

#undef BUF_SIZE
#undef F_LI
