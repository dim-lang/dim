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

namespace fastype {

File::File(const std::string &fileName)
    : fileName(fileName), fd(std::fopen(fileName.data(), "rw")), eof(false),
      readBuffer(BUF_SIZE), bufferList(), lineList(),
      log(LogManager::getLogger(fileName)) {
  F_DEBUGF(log, "fileName: {}", fileName);
}

File::~File() {
  if (fd) {
    std::fclose(fd);
    fd = nullptr;
  }
  readBuffer.release();
  std::for_each(
      bufferList.begin(), bufferList.end(),
      [](std::vector<std::shared_ptr<Buffer>>::iterator i) { (*i).reset(); });
  bufferList.clear();
  lineList.clear();
  F_DEBUGF(log, "fileName: {}", fileName);
}

const std::string &File::getFileName() const { return fileName; }

std::shared_ptr<File> File::open(const std::string &fileName) {
  return std::shared_ptr(new File(fileName));
}

void File::close(std::shared_ptr<File> file) { file.reset(); }

Line File::begin() {}

Line File::end() {}

Line File::getLine(int32_t lineNumber) {}

std::string File::toString() const {
  return "[ @File fileName: " + fileName + ", fd: 0x" + fd +
         ", eof: " + std::to_string(eof) +
         ", bufferList.size: " + bufferList.size() +
         ", lineList.size: " + lineList.size() + " ]";
}

int64_t File::loadOne() {
  // read 1 buffer
  readBuffer.clear();

  // if new buffer has line break character
  // tag them to new line

  if (!readBuffer.empty()) {
    F_DEBUGF(log, "readBuffer not empty: {}", readBuffer.toString());
    char *rend = readBuffer.buffer + readBuffer.seek + 1;
    char *p = std::find_first_of(readBuffer.buffer + readBuffer.seek, rend,
                                 [](char *i) { return *i == '\n' });
    F_DEBUGF(log, "readBuffer not empty, p: {} rend:{}", p, rend);
    if (p != rend) {
      std::shared_ptr<Buffer> b = std::shared_ptr<Buffer>(new Buffer(BUF_SIZE));
      int32_t bRead =
          b->read(readBuffer, p - (readBuffer.buffer + readBuffer.seek));
      F_DEBUGF(log, "b read: {}", bRead);
      std::shared_ptr<detail::LineImpl> lineImpl =
          std::shared_ptr<detail::LineImpl>(new detail::LineImpl(
              std::shared_ptr<File>(this), {b}, lineList.size()));
    }
  }

  // return readed bytes
}

} // namespace fastype

#undef BUF_SIZE
