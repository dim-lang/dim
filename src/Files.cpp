// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Files.h"
#include "Log.h"
#include <cerrno>
#include <cstdio>

#define BUF_SIZE 4096

namespace detail {

FileInfo::FileInfo(const Cowstr &fileName)
    : fileName_(fileName), fp_(nullptr) {}

FileInfo::~FileInfo() {
  if (fp_) {
    std::fclose(fp_);
    fp_ = nullptr;
  }
}

const Cowstr &FileInfo::fileName() const { return fileName_; }

FileWriterImpl::FileWriterImpl(const Cowstr &fileName) : FileInfo(fileName) {}

FileWriterImpl::~FileWriterImpl() { flush(); }

void FileWriterImpl::reset(int offset) { std::fseek(fp_, offset, SEEK_SET); }

int FileWriterImpl::flush() {
  if (buffer_.size() > 0) {
    buffer_.writefile(fp_);
  }
  return std::fflush(fp_) ? errno : 0;
}

int FileWriterImpl::write(const Cowstr &buf) {
  int r = buffer_.read(buf);
  if (buffer_.size() >= BUF_SIZE) {
    buffer_.writefile(fp_, BUF_SIZE);
  }
  return r;
}

int FileWriterImpl::writeln(const Cowstr &buf) {
  return write(buf) + write("\n");
}

} // namespace detail

FileReader::FileReader(const Cowstr &fileName) : detail::FileInfo(fileName) {
  fp_ = std::fopen(fileName.rawstr(), "r");
  LOG_ASSERT(fp_, "fp_ is null, open fileName {} failed", fileName);
}

FileMode FileReader::mode() const { return FileMode::Read; }

void FileReader::reset(int offset) { std::fseek(fp_, offset, SEEK_SET); }

void FileReader::prepareFor(int n) {
  if (n <= 0) {
    return;
  }
  if (buffer_.size() < n) {
    int c = 0;
    do {
      c = buffer_.readfile(fp_, BUF_SIZE);
      if (buffer_.size() >= n) {
        break;
      }
    } while (c > 0);
  }
}

Cowstr FileReader::read(int n) {
  if (n <= 0) {
    return "";
  }
  prepareFor(n);
  return buffer_.write(n);
}

Cowstr FileReader::readall() {
  buffer_.readfile(fp_);
  return buffer_.write(buffer_.size());
}

Cowstr FileReader::readc() {
  prepareFor(1);
  return buffer_.write(1);
}

char *FileReader::prepareUntil(char c) {
  char *linepos = nullptr;
  int n = 0;
  do {
    n = buffer_.readfile(fp_, BUF_SIZE);
    linepos = buffer_.search(c);
    if (linepos) {
      break;
    }
  } while (n > 0);
  return linepos;
}

Cowstr FileReader::readln() {
  char *linepos = prepareUntil('\n');
  if (!linepos) {
    return "";
  }
  int len = linepos - buffer_.begin() + 1;
  return buffer_.write(len);
}

FileWriter::FileWriter(const Cowstr &fileName)
    : detail::FileWriterImpl(fileName) {
  fp_ = std::fopen(fileName.rawstr(), "w");
  LOG_ASSERT(fp_, "fp_ is null, open fileName {} failed", fileName);
}

FileMode FileWriter::mode() const { return FileMode::Write; }

void FileWriter::reset(int offset) { detail::FileWriterImpl::reset(offset); }

FileAppender::FileAppender(const Cowstr &fileName)
    : detail::FileWriterImpl(fileName) {
  fp_ = std::fopen(fileName.rawstr(), "a");
  LOG_ASSERT(fp_, "fp_ is null, open fileName {} failed", fileName);
}

void FileAppender::reset(int offset) { detail::FileWriterImpl::reset(offset); }

FileMode FileAppender::mode() const { return FileMode::Append; }
