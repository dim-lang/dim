// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "File.h"
#include "Log.h"
#include <cstdlib>

icu::UnicodeString File::read(const icu::UnicodeString &fileName,
                              const char *locale, const char *codepage) {
  UFILE *fp = u_fopen_u(fileName.getBuffer(), "r", locale, codepage);
  FCHECK(fp != nullptr, "fp {} != nullptr", (void *)fp);
  UChar *data = nullptr;
  int l = 1024, n = 0, tot = 0;

  do {
    if (data == nullptr || tot >= l) {
      l *= 2;
      data = (UChar *)realloc(data, l);
      FCHECK(data, "realloc error! data {} != nullptr", (void *)data);
    }

    n = u_file_read(data + tot, l - tot, fp);
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
  FCHECK(fp != nullptr, "fp {} != nullptr", (void *)fp);
  int l = 1024;
  UChar *data = (UChar *)malloc(l);
  FCHECK(data, "realloc error! data {} != nullptr", (void *)data);
  std::vector<icu::UnicodeString> ret;

  while (true) {
    int pos = 0;
    int32_t dataLen;
    do {
      UChar *r = u_fgets(data + pos, l, fp);
      if (!r) {
        goto end_of_lines;
      }
      dataLen = u_strlen(data);
      FCHECK(dataLen > 0, "dataLen {} > 0", dataLen);
      FCHECK(dataLen < l, "dataLen {} < l {}", dataLen, l);
      if (dataLen < l - 1) {
        ret.push_back(icu::UnicodeString(data, dataLen));
      } else {
        l *= 2;
        data = (UChar *)realloc(data, l);
        FCHECK(data, "realloc error! data {} != nullptr", (void *)data);
        pos = dataLen - 1;
      }
    } while (dataLen >= l - 1);
  }

end_of_lines:
  u_fclose(fp);
  free(data);
  return ret;
}

static int writeImpl(const icu::UnicodeString &fileName,
                     const icu::UnicodeString &text, const char *perm,
                     const char *locale, const char *codepage) {
  UFILE *fp = u_fopen_u(fileName.getBuffer(), perm, locale, codepage);
  FCHECK(fp, "open file error! fp {} != nullptr", (void *)fp);
  int n = (int)u_file_write(text.getBuffer(), text.length(), fp);
  u_fclose(fp);
  return n;
}

static int writelineImpl(const icu::UnicodeString &fileName,
                         const std::vector<icu::UnicodeString> &lines,
                         const char *perm, const char *locale,
                         const char *codepage) {
  UFILE *fp = u_fopen_u(fileName.getBuffer(), perm, locale, codepage);
  FCHECK(fp, "open file error! fp {} != nullptr", (void *)fp);
  int n = 0;
  for (int i = 0; i < (int)lines.size(); i++) {
    n += (int)u_file_write(lines[i].getBuffer(), lines[i].length(), fp);
  }
  u_fclose(fp);
  return n;
}

int File::write(const icu::UnicodeString &fileName,
                const icu::UnicodeString &text, const char *locale,
                const char *codepage) {
  return writeImpl(fileName, text, "w", locale, codepage);
}

int File::writeline(const icu::UnicodeString &fileName,
                    const std::vector<icu::UnicodeString> &lines,
                    const char *locale, const char *codepage) {
  return writelineImpl(fileName, lines, "w", locale, codepage);
}

int File::append(const icu::UnicodeString &fileName,
                 const icu::UnicodeString &text, const char *locale,
                 const char *codepage) {
  return writeImpl(fileName, text, "a", locale, codepage);
}

int File::appendline(const icu::UnicodeString &fileName,
                     const std::vector<icu::UnicodeString> &lines,
                     const char *locale, const char *codepage) {
  return writelineImpl(fileName, lines, "a", locale, codepage);
}
