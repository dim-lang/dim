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
    : fileName(fileName), readBuffer(BUF_SIZE), writeBuffer(BUF_SIZE) {
  log = LogManager::getLogger(fileName);
  fd = std::fopen(fileName.data(), "rw");
  F_DEBUGF(log, "fileName:{}", fileName);
}

File::~File() {
  readBuffer.release();
  writeBuffer.release();
  if (fd) {
    std::fclose(fd);
    fd = nullptr;
  }
  std::for_each(
      buffer.begin(), buffer.end(),
      [](std::vector<std::shared_ptr<Line>>::iterator i) { (*i).reset(); });
  buffer.clear();
}

const std::string &File::getFileName() const { return fileName; }

std::shared_ptr<File> File::open(const std::string &fileName) {
  return std::shared_ptr(new File(fileName));
}

void File::close(std::shared_ptr<File> file) { file.reset(); }

std::shared_ptr<Line> File::firstLine() {
  return std::shared_ptr<Line>(new Line(std::shared_ptr<File>(this), 0));
}

std::shared_ptr<Line> File::lastLine() {
  return std::shared_ptr<Line>(new Line());
}

std::shared_ptr<Line> File::getLine(int32_t lineNumber) {
  loadAll();
  int32_t startBuffer = 0;
  int32_t startByte = 0;
  int32_t endBuffer = 0;
  int32_t endByte = 0;
  return Line(std::shared_ptr<File>(this), startBuffer, startByte, endBuffer,
              endByte);
}

} // namespace fastype
