// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "File.h"
#include "Log.h"
#include <cstdlib>

#define INIT_LEN 1024

icu::UnicodeString File::read(const icu::UnicodeString &fileName,
                              const char *locale, const char *codepage) {
  UFILE *fp = u_fopen_u(fileName.getBuffer(), "r", locale, codepage);
  CASSERT(fp, "fp is null: {}", (void *)fp);
  UChar *data = nullptr;
  int len = INIT_LEN, n = 0, tot = 0;

  do {
    if (data == nullptr || tot >= len) {
      len *= 2;
      data = (UChar *)realloc(data, len * sizeof(UChar));
      CASSERT(data, "realloc error! data {} != nullptr", (void *)data);
    }

    n = u_file_read(data + tot, len - tot, fp);
    tot += n;
  } while (n > 0);

  u_fclose(fp);

  icu::UnicodeString ret = icu::UnicodeString(data, tot);
  free(data);
  return ret;
}

std::vector<icu::UnicodeString>
File::readline(const icu::UnicodeString &fileName, const char *locale,
               const char *codepage) {
  UFILE *fp = u_fopen_u(fileName.getBuffer(), "r", locale, codepage);
  CASSERT(fp, "fp is null: {}", (void *)fp);
  int len = INIT_LEN;
  UChar *data = (UChar *)malloc(len * sizeof(UChar));
  CASSERT(data, "realloc error! data is null: {}", (void *)data);
  std::vector<icu::UnicodeString> ret;

  while (true) {
    int pos = 0;
    int32_t dataLen;
    while (true) {
      UChar *r = u_fgets(data + pos, len - pos, fp);
      if (!r) {
        goto end_of_readline;
      }
      dataLen = u_strlen(data);
      CASSERT(dataLen > 0, "dataLen {} > 0", dataLen);
      CASSERT(dataLen < len, "dataLen {} < len {}", dataLen, len);
      if (dataLen < len - 1) {
        ret.push_back(icu::UnicodeString(data, dataLen));
        break;
      } else {
        len *= 2;
        data = (UChar *)realloc(data, sizeof(UChar) * len);
        CASSERT(data, "realloc error! data is null: {}", (void *)data);
        pos = dataLen;
      }
    }
  }

end_of_readline:
  u_fclose(fp);
  free(data);
  return ret;
}

static int writeImpl(const icu::UnicodeString &fileName,
                     const std::vector<icu::UnicodeString> &texts,
                     const char *perm, const char *locale,
                     const char *codepage) {
  UFILE *fp = u_fopen_u(fileName.getBuffer(), perm, locale, codepage);
  CASSERT(fp, "open file error! fp is null: {}", (void *)fp);
  int n = 0;
  for (int i = 0; i < (int)texts.size(); i++) {
    n += (int)u_file_write(texts[i].getBuffer(), texts[i].length(), fp);
  }
  u_fclose(fp);
  return n;
}

int File::write(const icu::UnicodeString &fileName,
                const icu::UnicodeString &text, const char *locale,
                const char *codepage) {
  std::vector<icu::UnicodeString> ts = {text};
  return writeImpl(fileName, ts, "w", locale, codepage);
}

int File::writeline(const icu::UnicodeString &fileName,
                    const std::vector<icu::UnicodeString> &lines,
                    const char *locale, const char *codepage) {
  return writeImpl(fileName, lines, "w", locale, codepage);
}

int File::append(const icu::UnicodeString &fileName,
                 const icu::UnicodeString &text, const char *locale,
                 const char *codepage) {
  std::vector<icu::UnicodeString> ts = {text};
  return writeImpl(fileName, ts, "a", locale, codepage);
}

int File::appendline(const icu::UnicodeString &fileName,
                     const std::vector<icu::UnicodeString> &lines,
                     const char *locale, const char *codepage) {
  return writeImpl(fileName, lines, "a", locale, codepage);
}

#undef INIT_LEN
