// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "File.h"
#include "Exception.h"
#include <cstdio>

#define BUF_SIZE 4096

namespace detail {

FileInfo::FileInfo(const std::string &fileName)
    : fileName_(fileName), fp_(nullptr) {}

FileInfo::~FileInfo() {
  if (fp_) {
    std::fclose(fp_);
    fp_ = nullptr;
  }
}

const std::string &FileInfo::fileName() const { return fileName_; }

FileReaderLineIterator::FileReaderLineIterator(FileReader *reader)
    : reader_(reader), linePosition_(nullptr) {}

std::string FileReaderLineIterator::next() {
  EX_ASSERT(linePosition_, "linePosition_ is null");
  int len = linePosition_ - reader_->buffer_.begin() + 1;
  linePosition_ = nullptr;
  return reader_->buffer_.write(len);
}

bool FileReaderLineIterator::hasNext() {
  if (linePosition_) {
    return true;
  }
  int n = 0;
  do {
    n = reader_->buffer_.readfile(reader_->fp_, BUF_SIZE);
    linePosition_ = reader_->buffer_.search('\n');
    if (linePosition_) {
      return true;
    }
  } while (n > 0);
  return false;
}

FileReaderCharIterator::FileReaderCharIterator(FileReader *reader)
    : reader_(reader) {}

char FileReaderCharIterator::next() {
  EX_ASSERT(!reader_->buffer_.empty(), "reader_->buffer_ is empty");
  char c;
  int writen = reader_->buffer_.write(&c, 1);
  EX_ASSERT(writen == 1, "writen {} == 1", writen);
  return c;
}

bool FileReaderCharIterator::hasNext() {
  if (!reader_->buffer_.empty()) {
    return true;
  }
  int n = 0;
  do {
    n = reader_->buffer_.readfile(reader_->fp_, BUF_SIZE);
    if (!reader_->buffer_.empty()) {
      return true;
    }
  } while (n > 0);
  return false;
}

FileReaderBlockIterator::FileReaderBlockIterator(FileReader *reader)
    : reader_(reader) {}

std::string FileReaderBlockIterator::next(int n) {
  EX_ASSERT(reader_->buffer_.size() >= n, "read_->buffer.size {} >= n {}",
            reader_->buffer_.size(), n);
  return reader_->buffer_.write(n);
}

bool FileReaderBlockIterator::hasNext(int n) {
  if (reader_->buffer_.size() >= n) {
    return true;
  }
  int c = 0;
  do {
    c = reader_->buffer_.readfile(reader_->fp_, BUF_SIZE);
    if (reader_->buffer_.size() >= n) {
      return true;
    }
  } while (c > 0);
  return false;
}

FileWriterImpl::FileWriterImpl(const std::string &fileName)
    : FileInfo(fileName) {}

FileWriterImpl::~FileWriterImpl() { flush(); }

void FileWriterImpl::reset(int offset) {
  int r = std::fseek(fp_, offset, SEEK_SET);
  EX_ASSERT(r == 0, "r {} == 0", r);
}

void FileWriterImpl::flush() {
  int sz = buffer_.size();
  int r = buffer_.writefile(fp_);
  EX_ASSERT(r == sz, "r {} == sz {}", r, sz);
  std::fflush(fp_);
}

void FileWriterImpl::write(const char *buf, int n) {
  int r = buffer_.read(buf, n);
  EX_ASSERT(r == n, "r {} == n {}", r, n);
  if (buffer_.size() >= BUF_SIZE) {
    r = buffer_.writefile(fp_, BUF_SIZE);
    EX_ASSERT(r == BUF_SIZE, "r {} == BUF_SIZE {}", r, BUF_SIZE);
  }
}

void FileWriterImpl::write(const std::string &buf) {
  write(buf.data(), buf.length());
}

} // namespace detail

FileReader::FileReader(const std::string &fileName)
    : detail::FileInfo(fileName) {
  fp_ = std::fopen(fileName.c_str(), "r");
  EX_ASSERT(fp_, "fp_ is null, open fileName {} failed", fileName);
}

FileModeType FileReader::mode() const { return FileModeType::Read; }

void FileReader::reset(int offset) {
  int r = std::fseek(fp_, offset, SEEK_SET);
  EX_ASSERT(r == 0, "r {} == 0", r);
}

FileReader::LineIterator FileReader::lineIterator() {
  return FileReader::LineIterator(this);
}

FileReader::CharIterator FileReader::charIterator() {
  return FileReader::CharIterator(this);
}

FileReader::BlockIterator FileReader::blockIterator() {
  return FileReader::BlockIterator(this);
}

std::string FileReader::read() {
  int n = buffer_.readfile(fp_);
  EX_ASSERT(n >= 0, "n {} >= 0", n);
  return buffer_.write(buffer_.size());
}

FileWriter::FileWriter(const std::string &fileName)
    : detail::FileWriterImpl(fileName) {
  fp_ = std::fopen(fileName.c_str(), "w");
  EX_ASSERT(fp_, "fp_ is null, open fileName {} failed", fileName);
}

FileModeType FileWriter::mode() const { return FileModeType::Write; }

void FileWriter::reset(int offset) { detail::FileWriterImpl::reset(offset); }

FileAppender::FileAppender(const std::string &fileName)
    : detail::FileWriterImpl(fileName) {
  fp_ = std::fopen(fileName.c_str(), "a");
  EX_ASSERT(fp_, "fp_ is null, open fileName {} failed", fileName);
}

void FileAppender::reset(int offset) { detail::FileWriterImpl::reset(offset); }

FileModeType FileAppender::mode() const { return FileModeType::Append; }
