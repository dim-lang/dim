// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "File.h"
#include "Exception.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define F_BUF_SIZE 1024

std::string File::read(const std::string &fileName) {
  FILE *fp = std::fopen(fileName.c_str(), "r");
  EX_ASSERT(fp, "fp is null: {}", (void *)fp);
  int len = F_BUF_SIZE, n = 0, tot = 0;
  char *data = (char *)std::malloc(len * sizeof(char));
  EX_ASSERT(data, "malloc error! data is null: {}", (void *)data);

  do {
    if (data == nullptr || tot >= len) {
      len *= 2;
      data = (char *)std::realloc(data, len * sizeof(char));
      EX_ASSERT(data, "realloc error! data {} is null", (void *)data);
    }

    n = std::fread(data + tot, 1, len - tot, fp);
    tot += n;
  } while (n > 0);

  std::fclose(fp);
  std::string ret = std::string(data, tot);
  std::free(data);
  return ret;
}

std::vector<std::string> File::readline(const std::string &fileName) {
  FILE *fp = std::fopen(fileName.c_str(), "r");
  EX_ASSERT(fp, "fp is null: {}", (void *)fp);
  int len = F_BUF_SIZE;
  char *data = (char *)malloc(len * sizeof(char));
  EX_ASSERT(data, "realloc error! data is null: {}", (void *)data);
  std::vector<std::string> ret;

  while (true) {
    int pos = 0;
    int32_t dataLen;
    while (true) {
      char *r = std::fgets(data + pos, len - pos, fp);
      if (!r) {
        goto end_of_readline;
      }
      dataLen = std::strlen(data);
      EX_ASSERT(dataLen > 0, "dataLen {} > 0", dataLen);
      EX_ASSERT(dataLen < len, "dataLen {} < len {}", dataLen, len);
      if (dataLen < len - 1) {
        ret.push_back(std::string(data, dataLen));
        break;
      } else {
        len *= 2;
        data = (char *)realloc(data, sizeof(char) * len);
        EX_ASSERT(data, "realloc error! data is null: {}", (void *)data);
        pos = dataLen;
      }
    }
  }

end_of_readline:
  std::fclose(fp);
  std::free(data);
  return ret;
}

static int writeImpl(const std::string &fileName,
                     const std::vector<std::string> &texts, const char *perm) {
  FILE *fp = std::fopen(fileName.c_str(), perm);
  EX_ASSERT(fp, "open file error! fp is null: {}", (void *)fp);
  int n = 0;
  for (int i = 0; i < (int)texts.size(); i++) {
    n += (int)std::fwrite(texts[i].c_str(), 1, texts[i].length(), fp);
  }
  std::fclose(fp);
  return n;
}

int File::write(const std::string &fileName, const std::string &text) {
  std::vector<std::string> ts = {text};
  return writeImpl(fileName, ts, "w");
}

int File::writeline(const std::string &fileName,
                    const std::vector<std::string> &lines) {
  return writeImpl(fileName, lines, "w");
}

int File::append(const std::string &fileName, const std::string &text) {
  std::vector<std::string> ts = {text};
  return writeImpl(fileName, ts, "a");
}

int File::appendline(const std::string &fileName,
                     const std::vector<std::string> &lines) {
  return writeImpl(fileName, lines, "a");
}

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
  int len = linePosition_ - reader_->buffer_.begin();
  return reader_->buffer_.write(len);
}

bool FileReaderLineIterator::hasNext() {
  if (linePosition_) {
    return true;
  }
  int n = 0;
  do {
    n = reader_->buffer_.readfile(reader_->fp_, F_BUF_SIZE);
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
    n = reader_->buffer_.readfile(reader_->fp_, F_BUF_SIZE);
    if (!reader_->buffer_.empty()) {
      return true;
    }
  } while (n > 0);
  return false;
}

FileReaderBufferIterator::FileReaderBufferIterator(FileReader *reader)
    : reader_(reader) {}

std::string FileReaderBufferIterator::next(int n) {
  EX_ASSERT(reader_->buffer_.size() >= n, "read_->buffer.size {} >= n {}",
            reader_->buffer_.size(), n);
  return reader_->buffer_.write(n);
}

bool FileReaderBufferIterator::hasNext(int n) {
  if (reader_->buffer_.size() >= n) {
    return true;
  }
  int c = 0;
  do {
    c = reader_->buffer_.readfile(reader_->fp_, F_BUF_SIZE);
    if (reader_->buffer_.size() >= n) {
      return true;
    }
  } while (c > 0);
  return false;
}

} // namespace detail

FileReader::FileReader(const std::string &fileName)
    : detail::FileInfo(fileName) {
  fp_ = std::fopen(fileName.c_str(), "r");
  EX_ASSERT(fp_, "fp_ is null, open fileName {} failed", fileName);
}

void FileReader::reset() {
  int r = std::fseek(fp_, 0, SEEK_SET);
  EX_ASSERT(r == 0, "r {} == 0", r);
}

FileReader::LineIterator FileReader::beginLine() {
  return FileReader::LineIterator(this);
}

FileReader::CharIterator FileReader::beginChar() {
  return FileReader::CharIterator(this);
}

FileReader::BufferIterator FileReader::beginBuffer() {
  return FileReader::BufferIterator(this);
}
