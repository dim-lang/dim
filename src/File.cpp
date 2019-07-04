// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "File.h"
#include "Log.h"
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
  for (int i = 0; i < buffer.size(); i++) {
    if (buffer[i]) {
      delete[] buffer[i];
      buffer[i] = nullptr;
    }
  }
}

const std::string &File::getFileName() const { return fileName; }

int64_t File::readBufferImpl(int count) {
  int64_t r = 0;
  size_t n = 0;
  while ((n = fread(readBuffer, sizeof(char), BUF_SIZE, fd)) > 0) {
    char *block = new char[BUF_SIZE];
    std::memcpy(block, readBuffer, n);
    block[n + 1] = (char)0;
    buffer.push_back(block);
    r += (int64_t)n;
  }
  return r;
}

std::shared_ptr<File> File::open(const std::string &fileName) {
  return std::shared_ptr<File>(new File(fileName));
}

void File::close(const std::shared_ptr<File> file) { file->~File(); }

} // namespace fastype
