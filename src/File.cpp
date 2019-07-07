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

File::File(const std::string &fileName) : fileName(fileName) {
  log = LogManager::getLogger(fileName);
  readBuffer = new char[BUF_SIZE];
  writeBuffer = new char[BUF_SIZE];
  fd = std::fopen(fileName.data(), "rw");
  F_DEBUGF(log, "fileName:{}", fileName);
}

File::~File() {
  if (readBuffer) {
    delete[] readBuffer;
    readBuffer = nullptr;
  }
  if (writeBuffer) {
    delete[] writeBuffer;
    writeBuffer = nullptr;
  }
  if (fd) {
    std::fclose(fd);
    fd = nullptr;
  }
  std::for_each(
      buffer.begin(), buffer.end(),
      [](std::vector<std::shared_ptr<Line>>::iterator i) { delete i; });
  buffer.clear();
}

const std::string &File::getFileName() const { return fileName; }

std::shared_ptr<File> File::open(const std::string &fileName) {
  return std::shared_ptr(new File(fileName));
}

void File::close(std::shared_ptr<File> file) { delete file; }

Line File::beginLine() {
  int32_t startBuffer = 0;
  int32_t startByte = 0;
  int32_t endBuffer = 0;
  int32_t endBuffer = 0;
  return Line(std::shared_ptr<File>(this), startBuffer, startByte, endBuffer,
              endByte);
}

Line File::endLine() {
  loadAll();
  int32_t startBuffer = 0;
  int32_t startByte = 0;
  int32_t endBuffer = 0;
  int32_t endByte = 0;
  return Line(std::shared_ptr<File>(this), startBuffer, startByte, endBuffer,
              endByte);
}

Line File::getLine(int32_t line) {
  loadAll();
  int32_t startBuffer = 0;
  int32_t startByte = 0;
  int32_t endBuffer = 0;
  int32_t endByte = 0;
  return Line(std::shared_ptr<File>(this), startBuffer, startByte, endBuffer,
              endByte);
}

} // namespace fastype
