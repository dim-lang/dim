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
  readBuf = new char[BUF_SIZE];
  fd = std::fopen(fileName.data(), "rw");
  F_DEBUGF(log, "fileName:{}", fileName);
}

File::~File() {
  if (readBuf) {
    delete[] readBuf;
    readBuf = nullptr;
  }
  if (fd) {
    std::fclose(fd);
    fd = nullptr;
  }
  for (int i = 0; i < lineBuf.size(); i++) {
    if (lineBuf[i]) {
      delete[] lineBuf[i];
      lineBuf[i] = nullptr;
    }
  }
}

const std::string &File::getFileName() const { return fileName; }

int64_t File::readToBuffer() {
  int64_t r = 0;
  int32_t n = 0;
  while ((n = fread(fd, readBuf, BUF_SIZE)) > 0) {
    char *buf = new char[BUF_SIZE];
    std::memset(buf, 0, BUF_SIZE);
    std::memcpy(buf, readBuf, n);
    lineBuf.append(buf);
    r += (int64_t)n;
  }
  return r;
}

std::shared_ptr<File> File::open(const std::string &fileName) {
  return std::shared_ptr<File>(new File(fileName));
}

void File::close(const std::shared_ptr<File> file) { file->~File(); }

} // namespace fastype
