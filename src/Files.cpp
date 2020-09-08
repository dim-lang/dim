// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Files.h"
#include "Log.h"
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
  LOG_ASSERT(linePosition_, "linePosition_ is null");
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
  LOG_ASSERT(!reader_->buffer_.empty(), "reader_->buffer_ is empty");
  char c;
  int writen = reader_->buffer_.write(&c, 1);
  LOG_ASSERT(writen == 1, "writen {} == 1", writen);
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
  LOG_ASSERT(n >= 0, "n {} >= 0", n);
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

void FileWriterImpl::reset(int offset) { std::fseek(fp_, offset, SEEK_SET); }

void FileWriterImpl::flush() {
  if (buffer_.size() > 0) {
    buffer_.writefile(fp_);
  }
  std::fflush(fp_);
}

void FileWriterImpl::write(const char *buf, int n) {
  buffer_.read(buf, n);
  if (buffer_.size() >= BUF_SIZE) {
    buffer_.writefile(fp_, BUF_SIZE);
  }
}

void FileWriterImpl::write(const std::string &buf) {
  write(buf.data(), buf.length());
}

} // namespace detail

FileReader::FileReader(const std::string &fileName)
    : detail::FileInfo(fileName) {
  fp_ = std::fopen(fileName.c_str(), "r");
  LOG_ASSERT(fp_, "fp_ is null, open fileName {} failed", fileName);
}

FileModeType FileReader::mode() const { return FileModeType::Read; }

void FileReader::reset(int offset) { std::fseek(fp_, offset, SEEK_SET); }

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
  buffer_.readfile(fp_);
  return buffer_.write(buffer_.size());
}

FileWriter::FileWriter(const std::string &fileName)
    : detail::FileWriterImpl(fileName) {
  fp_ = std::fopen(fileName.c_str(), "w");
  LOG_ASSERT(fp_, "fp_ is null, open fileName {} failed", fileName);
}

FileModeType FileWriter::mode() const { return FileModeType::Write; }

void FileWriter::reset(int offset) { detail::FileWriterImpl::reset(offset); }

FileAppender::FileAppender(const std::string &fileName)
    : detail::FileWriterImpl(fileName) {
  fp_ = std::fopen(fileName.c_str(), "a");
  LOG_ASSERT(fp_, "fp_ is null, open fileName {} failed", fileName);
}

void FileAppender::reset(int offset) { detail::FileWriterImpl::reset(offset); }

FileModeType FileAppender::mode() const { return FileModeType::Append; }
