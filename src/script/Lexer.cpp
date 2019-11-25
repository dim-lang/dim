// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Lexer.h"
#include "exception/ParseException.h"
#include "script/token/BooleanToken.h"
#include "script/token/EofToken.h"
#include "script/token/FloatingToken.h"
#include "script/token/IdentifierToken.h"
#include "script/token/IntegerToken.h"
#include "script/token/OperatorToken.h"
#include "script/token/StringToken.h"
#include <cstdlib>
#include <cstring>
#include <fmt/format.h>
#include <regex>
#include <string>

#define F_SUB_STRING(x, pos, y)                                                \
  ((x).tempSubString(pos,                                                      \
                     std::min<int>(64, (int)(x).tempSubString(pos).length()))  \
       .toUTF8String(y))

namespace fastype {

Lexer::Lexer(const icu::UnicodeString &text) : pos_(0), text_(text) {}

Lexer::~Lexer() {
  while (!queue_.empty()) {
    queue_.pop_front();
  }
  queue_.clear();
  queue_.shrink_to_fit();
}

std::string Lexer::toString() const {
  std::string _1;
  return fmt::format("[ @Lexer pos_:{}, text_:{} ]", pos_,
                     F_SUB_STRING(text_, 0, _1));
}

// parse whitespace
// @return   true if is whitespace and skipped
//           false if not
static bool parseWhitespace(const icu::UnicodeString &text, int &i) {
  bool parsed = false;
  while (u_isspace(text.charAt(i))) {
    i += 1;
    parsed = true;
  }
  return parsed;
}

// parse constant token
static void parseConstToken(int &i, Sptr<Token> t, std::deque<Sptr<Token>> &q,
                            int diff = 1) {
  q.push_back(t);
  i += diff;
}

// parse integer or floating
static void parseNumber(const icu::UnicodeString &text, int &i,
                        std::deque<Sptr<Token>> &q) {
  std::string _1;
  int dotCount = 0;  // .
  int expCount = 0;  // e or E
  int flagCount = 0; // + or -
  int j = i;
  while (j < text.length()) {
    if (u_isdigit(text.charAt(j))) {
      j++;
      continue;
    }
    // floating number: 23.91
    if (text.charAt(j) == (UChar)'.') {
      j++;
      dotCount++;
      continue;
    }
    // exp: 12e8
    if (text.charAt(j) == (UChar)'e' || text.charAt(j) == (UChar)'E') {
      j++;
      expCount++;

      // exp with flag: 3E+5
      if (j + 1 < text.length() && (text.charAt(j + 1) == (UChar)'+' ||
                                    text.charAt(j + 1) == (UChar)'-')) {
        j++;
        flagCount++;
      }
      continue;
    }
    break;
  }

  F_CHECK(dotCount <= 1, "dotCount {} <= 1 failure! text[{}]:{}", dotCount, i,
          F_SUB_STRING(text, i, _1));
  F_CHECK(expCount <= 1, "expCount {} <= 1 failure! text[{}]:{}", expCount, i,
          F_SUB_STRING(text, i, _1));
  F_CHECK(flagCount <= 2, "flagCount {} <= 2 failure! text[{}]:{}", flagCount,
          i, F_SUB_STRING(text, i, _1));

  std::string utf8;
  text.tempSubString(i, j - i).toUTF8String(utf8);

  // floating number
  if (dotCount > 0) {
    double value = std::stod(utf8);
    Sptr<Token> floatingToken = Sptr<Token>(new FloatingToken(value));
    q.push_back(floatingToken);
  } else {
    // integer number
    long long value = std::stoll(utf8);
    Sptr<Token> integerToken = Sptr<Token>(new IntegerToken(value));
    q.push_back(integerToken);
  }

  i = j;
}

// parse comment
static void parseComment(const icu::UnicodeString &text, int &i) {
  icu::UnicodeString unixLineBreak = UNICODE_STRING_SIMPLE("\n");
  icu::UnicodeString blockCommetEnd = UNICODE_STRING_SIMPLE("*/");

  F_CHECK(text.charAt(i) == (UChar)'/', "text[{}] {} == / {}", i,
          (int)text.charAt(i), (int)'/');
  F_CHECK(i + 1 < text.length(), "i+1 {} < text.length {}", i + 1,
          text.length());
  switch (text.charAt(i + 1)) {
  case (UChar)'/': // line comment
  {
    int lineEndPos = text.indexOf(unixLineBreak, i + 1);
    i = lineEndPos + 1;
  } break;
  case (UChar)'*': // block comment
  {
    int lineEndPos = text.indexOf(blockCommetEnd, i + 1);
    i = lineEndPos + 2;
  } break;
  }
}

// parse string
static void parseString(const icu::UnicodeString &text, int &i,
                        std::deque<Sptr<Token>> &q) {
  std::string _1;
  int j = i + 1;
  bool findString = false;
  (void)findString;
  while (j < text.length()) {
    if (text.charAt(j) == (UChar)'\\') {
      j += 2;
      continue;
    }
    if (text.charAt(j) == (UChar)'\"') {
      j += 1;
      findString = true;
      break;
    }
    j += 1;
  }
  F_CHECK(findString, "parse string fail at i:{}, j:{}, text[{}]: {}", i, j, i,
          F_SUB_STRING(text, i, _1));
  Sptr<Token> strToken =
      Sptr<Token>(new StringToken(text.tempSubString(i, j - i)));
  q.push_back(strToken);
  i = j;
}

// parse identifier
static bool parseIdentifier(const icu::UnicodeString &text, int &i,
                            std::deque<Sptr<Token>> &q) {
  if (!u_isalpha(text.charAt(i)) && text.charAt(i) != (UChar)'_') {
    return false;
  }
  int j = i;
  while (j < text.length()) {
    if (u_isalnum(text.charAt(j)) || text.charAt(j) == (UChar)'_') {
      j++;
    } else {
      break;
    }
  }
  std::string _1;
  F_CHECK(j > i, "j {} > i {}, text[{}]:{}", j, i, i,
          F_SUB_STRING(text, i, _1));
  Sptr<Token> idToken =
      Sptr<Token>(new IdentifierToken(text.tempSubString(i, j - i)));
  q.push_back(idToken);
  i = j;
  return true;
}

// parse keyword or identifier
// @return   true if is keyword
//           false if not
static bool parseKeywordOrIdentifier(const icu::UnicodeString &text, int &i,
                                     std::deque<Sptr<Token>> &q) {
  switch (text.charAt(i)) {
  case (UChar)'l':
    if (i + 3 < text.length() &&
        text.tempSubString(i, 3) == Token::T_LET->literal()) {
      q.push_back(Token::T_LET);
      i += 3;
      return true;
    }
    break;
  case (UChar)'n':
    if (i + 4 < text.length() &&
        text.tempSubString(i, 4) == Token::T_NULL->literal()) {
      q.push_back(Token::T_NULL);
      i += 4;
      return true;
    }
    break;
  case (UChar)'i':
    if (i + 2 < text.length() &&
        text.tempSubString(i, 2) == Token::T_IF->literal()) {
      q.push_back(Token::T_IF);
      i += 2;
      return true;
    } else if (i + 10 < text.length() &&
               text.tempSubString(i, 10) == Token::T_ISINSTANCE->literal()) {
      q.push_back(Token::T_ISINSTANCE);
      i += 10;
      return true;
    } else if (i + 6 < text.length() &&
               text.tempSubString(i, 6) == Token::T_IMPORT->literal()) {
      q.push_back(Token::T_IMPORT);
      i += 6;
      return true;
    }
    break;
  case (UChar)'e':
    if (i + 6 < text.length() &&
        text.tempSubString(i, 6) == Token::T_ELSEIF->literal()) {
      q.push_back(Token::T_ELSEIF);
      i += 6;
      return true;
    } else if (i + 4 < text.length() &&
               text.tempSubString(i, 4) == Token::T_ELSE->literal()) {
      q.push_back(Token::T_ELSE);
      i += 4;
      return true;
    }
    break;
  case (UChar)'f':
    if (i + 3 < text.length() &&
        text.tempSubString(i, 3) == Token::T_FOR->literal()) {
      q.push_back(Token::T_FOR);
      i += 3;
      return true;
    } else if (i + 4 < text.length() &&
               text.tempSubString(i, 4) == Token::T_FUNC->literal()) {
      q.push_back(Token::T_FUNC);
      i += 4;
      return true;
    }
    break;
  case (UChar)'w':
    if (i + 5 < text.length() &&
        text.tempSubString(i, 5) == Token::T_WHILE->literal()) {
      q.push_back(Token::T_WHILE);
      i += 5;
      return true;
    }
    break;
  case (UChar)'b':
    if (i + 5 < text.length() &&
        text.tempSubString(i, 5) == Token::T_BREAK->literal()) {
      q.push_back(Token::T_BREAK);
      i += 5;
      return true;
    }
    break;
  case (UChar)'c':
    if (i + 8 < text.length() &&
        text.tempSubString(i, 8) == Token::T_CONTINUE->literal()) {
      q.push_back(Token::T_CONTINUE);
      i += 8;
      return true;
    } else if (i + 5 < text.length() &&
               text.tempSubString(i, 5) == Token::T_CLASS->literal()) {
      q.push_back(Token::T_CLASS);
      i += 5;
      return true;
    }
    break;
  case (UChar)'t':
    if (i + 4 < text.length() &&
        text.tempSubString(i, 4) == Token::T_TYPE->literal()) {
      q.push_back(Token::T_TYPE);
      i += 4;
      return true;
    }
    break;
  case (UChar)'r':
    if (i + 6 < text.length() &&
        text.tempSubString(i, 6) == Token::T_RETURN->literal()) {
      q.push_back(Token::T_RETURN);
      i += 6;
      return true;
    }
    break;
  case (UChar)'F':
    if (i + 5 < text.length() &&
        text.tempSubString(i, 5) == UNICODE_STRING_SIMPLE("False")) {
      q.push_back(Token::T_FALSE);
      i += 5;
      return true;
    }
    break;
  case (UChar)'T':
    if (i + 4 < text.length() &&
        text.tempSubString(i, 4) == UNICODE_STRING_SIMPLE("True")) {
      q.push_back(Token::T_TRUE);
      i += 4;
      return true;
    }
    break;
  }
  return parseIdentifier(text, i, q);
}

Sptr<Token> Lexer::read() {
  readImpl();

  if (!queue_.empty()) {
    Sptr<Token> t = queue_.front();
    queue_.pop_front();
    return t;
  }
  return Token::T_EOF;
}

Sptr<Token> Lexer::peek(int pos) {
  do {
    int n = queue_.size();
    readImpl();
    if (queue_.size() <= n) {
      break;
    }
  } while (queue_.size() <= pos);
  return queue_.size() <= pos ? Token::T_EOF : queue_[pos];
}

void Lexer::readImpl() {
  std::string _1;

  while (pos_ < text_.length()) {
    if (parseWhitespace(text_, pos_)) {
      continue;
    }
    if (parseKeywordOrIdentifier(text_, pos_, queue_)) {
      return;
    }
    switch (text_.charAt(pos_)) {
    case (UChar)'+': {
      if (pos_ + 1 < text_.length() && text_.charAt(pos_ + 1) == (UChar)'+') {
        // ++
        parseConstToken(pos_, Token::T_INC, queue_, 2);
      } else if (pos_ + 1 < text_.length() &&
                 text_.charAt(pos_ + 1) == (UChar)'=') {
        // +=
        parseConstToken(pos_, Token::T_ADDASSIGN, queue_, 2);
      } else {
        // +
        parseConstToken(pos_, Token::T_ADD, queue_);
      }
      return;
    } break;
    case (UChar)'-': {
      if (pos_ + 1 < text_.length() && text_.charAt(pos_ + 1) == (UChar)'-') {
        // --
        parseConstToken(pos_, Token::T_DEC, queue_, 2);
      } else if (pos_ + 1 < text_.length() &&
                 text_.charAt(pos_ + 1) == (UChar)'=') {
        // -=
        parseConstToken(pos_, Token::T_SUBASSIGN, queue_, 2);
      } else {
        // -
        parseConstToken(pos_, Token::T_SUB, queue_);
      }
      return;
    } break;
    case (UChar)'*': {
      if (pos_ + 1 < text_.length() && text_.charAt(pos_ + 1) == (UChar)'=') {
        // *=
        parseConstToken(pos_, Token::T_MULASSIGN, queue_, 2);
      } else {
        // *
        parseConstToken(pos_, Token::T_MUL, queue_);
      }
      return;
    } break;
    case (UChar)'/': {
      if (pos_ + 1 < text_.length() && text_.charAt(pos_ + 1) == (UChar)'=') {
        // /=
        parseConstToken(pos_, Token::T_DIVASSIGN, queue_, 2);
      } else if (pos_ + 1 < text_.length() &&
                 (text_.charAt(pos_ + 1) == (UChar)'/' ||
                  text_.charAt(pos_ + 1) == (UChar)'*')) {
        // block comment start /*
        parseComment(text_, pos_);
      } else {
        // /
        parseConstToken(pos_, Token::T_DIV, queue_);
      }
      return;
    } break;
    case (UChar)'%': {
      if (pos_ + 1 < text_.length() && text_.charAt(pos_ + 1) == (UChar)'=') {
        // %=
        parseConstToken(pos_, Token::T_MODASSIGN, queue_, 2);
      } else {
        // %
        parseConstToken(pos_, Token::T_MOD, queue_);
      }
      return;
    } break;
    case (UChar)'0': // number
    case (UChar)'1':
    case (UChar)'2':
    case (UChar)'3':
    case (UChar)'4':
    case (UChar)'5':
    case (UChar)'6':
    case (UChar)'7':
    case (UChar)'8':
    case (UChar)'9': {
      parseNumber(text_, pos_, queue_);
      return;
    } break;
    case (UChar)'"': // string or char
      parseString(text_, pos_, queue_);
      break;
    case (UChar)'=':
      if (text_.tempSubString(pos_, 2) == UNICODE_STRING_SIMPLE("==")) {
        // ==
        parseConstToken(pos_, Token::T_EQ, queue_, 2);
      } else {
        // =
        parseConstToken(pos_, Token::T_ASSIGNMENT, queue_);
      }
      break;
      /*
    case (UChar)'!': // ! or !=
      if (text_.tempSubString(pos_, 2) == UNICODE_STRING_SIMPLE("!=")) {
        parseConstToken(pos_, Token::T_NEQ, queue_, 2);
      } else {
        parseConstToken(pos_, Token::T_NOT, queue_);
      }
      break;
    case (UChar)'<': // < or <=
      if (text_.tempSubString(pos_, 2) == UNICODE_STRING_SIMPLE("<=")) {
        parseConstToken(pos_, Token::T_LE, queue_, 2);
      } else {
        parseConstToken(pos_, Token::T_LT, queue_);
      }
      break;
    case (UChar)'>': // > or >=
      if (text_.tempSubString(pos_, 2) == UNICODE_STRING_SIMPLE(">=")) {
        parseConstToken(pos_, Token::T_GE, queue_, 2);
      } else {
        parseConstToken(pos_, Token::T_GT, queue_);
      }
      break;
    case (UChar)',':
      parseConstToken(pos_, Token::T_COMMA, queue_);
      break;
      */
    case (UChar)';':
      parseConstToken(pos_, Token::T_SEMI, queue_);
      break;
      /*
    case (UChar)'?':
      parseConstToken(pos_, Token::T_QUESTION, queue_);
      break;
    case (UChar)':':
      parseConstToken(pos_, Token::T_COLON, queue_);
      break;
    */
    case (UChar)'(': {
      parseConstToken(pos_, Token::T_LP, queue_);
      return;
    } break;
    case (UChar)')': {
      parseConstToken(pos_, Token::T_RP, queue_);
      return;
    } break;
    case (UChar)'[':
      parseConstToken(pos_, Token::T_LBRACKET, queue_);
      break;
    case (UChar)']':
      parseConstToken(pos_, Token::T_RBRACKET, queue_);
      break;
    case (UChar)'{':
      parseConstToken(pos_, Token::T_LBRACE, queue_);
      break;
    case (UChar)'}':
      parseConstToken(pos_, Token::T_RBRACE, queue_);
      break;
    default:
      F_CHECK(false, "unknown token at text_[{}]: {}", pos_,
              F_SUB_STRING(text_, pos_, _1));
    }
  }
}

} // namespace fastype

#undef F_SUB_STRING
