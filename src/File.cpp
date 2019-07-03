// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "File.h"
#include "Log.h"
#include <cstdio>
#include <string>

#define READ_BUF 4096

namespace fastype {

File::File(const std::string &fileName) : fileName(fileName) {
  log = LogManager::getLogger(fileName);
  readBuf = new char[READ_BUF];
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
}

const std::string &File::getFileName() const { return fileName; }

int File::read(int line) {
  (void)line;
  return 0;
}

std::shared_ptr<File> File::open(const std::string &fileName) {
  return std::shared_ptr<File>(new File(fileName));
}

void File::close(const std::shared_ptr<File> file) { file->~File(); }

} // namespace fastype
