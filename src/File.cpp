// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "File.h"
#include "Log.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define F_BUF_SIZE 16384

std::string File::read(const std::string &fileName) {
  FILE *fp = std::fopen(fileName.c_str(), "r");
  CASSERT(fp, "fp is null: {}", (void *)fp);
  int len = F_BUF_SIZE, n = 0, tot = 0;
  char *data = (char *)std::malloc(len * sizeof(char));
  CASSERT(data, "malloc error! data is null: {}", (void *)data);

  do {
    if (data == nullptr || tot >= len) {
      len *= 2;
      data = (char *)std::realloc(data, len * sizeof(char));
      CASSERT(data, "realloc error! data {} is null", (void *)data);
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
  CASSERT(fp, "fp is null: {}", (void *)fp);
  int len = F_BUF_SIZE;
  char *data = (char *)malloc(len * sizeof(char));
  CASSERT(data, "realloc error! data is null: {}", (void *)data);
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
      CASSERT(dataLen > 0, "dataLen {} > 0", dataLen);
      CASSERT(dataLen < len, "dataLen {} < len {}", dataLen, len);
      if (dataLen < len - 1) {
        ret.push_back(std::string(data, dataLen));
        break;
      } else {
        len *= 2;
        data = (char *)realloc(data, sizeof(char) * len);
        CASSERT(data, "realloc error! data is null: {}", (void *)data);
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
  CASSERT(fp, "open file error! fp is null: {}", (void *)fp);
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

#undef F_BUF_SIZE
