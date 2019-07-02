// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "File.h"
#include "Log.h"
#include <string>

#define READ_BUF 4096

namespace fastype {

File::File(icu::UnicodeString &fileName) : fileName(fileName) {
  std::string tmpName;
  fileName.toUTF8String(tmpName);
  log = LogManager::getLogger(tmpName);
  readBuf_ = new char[READ_BUF];
  fd = u_fopen(tmpName.data(), "rw", nullptr, nullptr);
  F_DEBUGF(log, "fileName:{} codePage: {} locale: {}", tmpName.data(),
           u_fgetcodepage(fd), u_fgetlocale(fd));
}

File::~File() {
  if (readBuf_) {
    delete[] readBuf_;
    readBuf_ = nullptr;
  }
  if (fd) {
    u_fclose(fd);
    fd = nullptr;
  }
}

const icu::UnicodeString &File::getFileName() const { return fileName_; }

int File::read(int line) {
  (void)line;
  return 0;
}

std::shared_ptr<File> File::open(const icu::UnicodeString &fileName) {
  return std::shared_ptr<File>(new File(fileName));
}

void File::close(const std::shared_ptr<File> file) { file->~File(); }

} // namespace fastype
