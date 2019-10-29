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

#define F_SUB_STRING(x, pos, y)                                                \
  ((x).tempSubString(pos,                                                      \
                     std::min<int>(64, (int)(x).tempSubString(pos).length()))  \
       .toUTF8String(y))

namespace fastype {

Lexer::Lexer(const icu::UnicodeString &text)
    : queue_(), more_(true), pos_(0), text_(text) {
  F_INFO("Constructor:{}", toString());
}

std::string Lexer::toString() const {
  std::string _1;
  return fmt::format("[ @Lexer pos_:{}, more_:{}, text_:{} ]", pos_, more_,
                     F_SUB_STRING(text_, 0, _1));
}

Lexer::~Lexer() {
  while (!queue_.empty()) {
    Sptr<Token> t = queue_.front();
    t.reset();
    queue_.pop_front();
  }
  queue_.clear();
}

Sptr<Token> Lexer::read() {
  if (fillQueue(0)) {
    Sptr<Token> t = queue_.front();
    queue_.pop_front();
    return t;
  } else {
    return Token::T_EOF;
  }
}

Sptr<Token> Lexer::peek(int i) {
  if (fillQueue(i)) {
    return queue_[i];
  } else {
    return Token::T_EOF;
  }
}

bool Lexer::fillQueue(int i) {
  while (i >= (int)queue_.size()) {
    if (!more_) {
      return false;
    }
    parse();
  }
  return true;
}

// parse whitespace
// @return   true if is whitespace and skipped
//           false if not
static bool parseWhitespace(const icu::UnicodeString &text, int &i) {
  if (u_isspace(text.charAt(i))) {
    i += 1;
    return true;
  }
  return false;
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
  UErrorCode err;
  icu::NumberFormat *numberFormatter = icu::NumberFormat::createInstance(err);
  F_CHECK(U_SUCCESS(err),
          "createInstance failure! error:{}, errorName:{}, text[{}]:{}",
          (int)err, u_errorName(err), i, F_SUB_STRING(text, i, _1));

  int dotCount = 0;  // .
  int expCount = 0;  // e or E
  int flagCount = 0; // + or -
  int j = i;
  while (j < text.length()) {
    if (u_isdigit(text.charAt(j))) {
      j++;
      continue;
    }
    if (text.charAt(j) == (UChar)'.') {
      j++;
      dotCount++;
      continue;
    }
    if (text.charAt(j) == (UChar)'e' || text.charAt(j) == (UChar)'E') {
      j++;
      expCount++;
      continue;
    }
    if (text.charAt(j) == (UChar)'+' || text.charAt(j) == (UChar)'-') {
      j++;
      flagCount++;
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

  icu::Formattable formattable;
  numberFormatter->parse(text.tempSubString(i, j - i), formattable, err);

  F_CHECK(U_SUCCESS(err), "parse failure! error:{}, errorName:{}, text[{}]:{}",
          (int)err, u_errorName(err), i, F_SUB_STRING(text, i, _1));

  if (formattable.getType() == icu::Formattable::Type::kLong) {
    long long value = (long long)formattable.getLong(err);
    F_CHECK(U_SUCCESS(err),
            "getLong failure! error:{}, errorName:{}, text[{}]:{}", (int)err,
            u_errorName(err), i, F_SUB_STRING(text, i, _1));
    Sptr<Token> integerToken = Sptr<Token>(new IntegerToken(value));
    q.push_back(integerToken);
  } else if (formattable.getType() == icu::Formattable::Type::kInt64) {
    long long value = (long long)formattable.getInt64(err);
    F_CHECK(U_SUCCESS(err),
            "getInt64 failure! error:{}, errorName:{}, text[{}]:{}", (int)err,
            u_errorName(err), i, F_SUB_STRING(text, i, _1));
    Sptr<Token> integerToken = Sptr<Token>(new IntegerToken(value));
    q.push_back(integerToken);
  } else if (formattable.getType() == icu::Formattable::Type::kDouble) {
    double value = (double)formattable.getDouble(err);
    F_CHECK(U_SUCCESS(err),
            "getDouble failure! error:{}, errorName:{}, text[{}]:{}", (int)err,
            u_errorName(err), i, F_SUB_STRING(text, i, _1));
    Sptr<Token> floatingToken = Sptr<Token>(new FloatingToken(value));
    q.push_back(floatingToken);
  } else {
    F_CHECK(false, "formattable type unknown! type:{}, text[{}]:{}",
            (int)formattable.getType(), i, F_SUB_STRING(text, i, _1));
  }
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
  int j = i;
  bool findString = false;
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
  i = j + 1;
}

// parse identifier
static bool parseIdentifier(const icu::UnicodeString &text, int &i,
                            std::deque<Sptr<Token>> &q) {
  int j = i;
  while (j < text.length()) {
    if (u_isalnum(text.charAt(j)) || text.charAt(j) == (UChar)'_') {
      j++;
    } else {
      break;
    }
  }
  Sptr<Token> idToken =
      Sptr<Token>(new IdentifierToken(text.tempSubString(i, j - i)));
  q.push_back(idToken);
}

void Lexer::parse() {
  if (pos_ >= text_.length()) {
    more_ = false;
    return;
  }

  std::string _1;

  int i = 0;
  while (i < text_.length()) {
    if (parseWhitespace(text_, i)) {
      continue;
    }
    switch (text_.charAt(i)) {
    case (UChar)'+': // + or number
    {
      if (i + 1 < text_.length() && u_isdigit(text_.charAt(i + 1))) {
        parseNumber(text_, i, queue_);
      } else {
        parseConstToken(i, Token::T_ADD, queue_);
      }
    } break;
    case (UChar)'-': // - or number
    {
      if (i + 1 < text_.length() && u_isdigit(text_.charAt(i + 1))) {
        parseNumber(text_, i, queue_);
      } else {
        parseConstToken(i, Token::T_SUB, queue_);
      }
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
    case (UChar)'9':
      parseNumber(text_, i, queue_);
      break;
    case (UChar)'*': // *
      parseConstToken(i, Token::T_MUL, queue_);
      break;
    case (UChar)'/': // / or // or /*
    {
      if (i + 1 < text_.length() && (text_.charAt(i + 1) == (UChar)'/' ||
                                     text_.charAt(i + 1) == (UChar)'*')) {
        parseComment(text_, i);
      } else {
        parseConstToken(i, Token::T_DIV, queue_);
      }
    } break;
    case (UChar)'%':
      parseConstToken(i, Token::T_MOD, queue_);
      break;
    case (UChar)'"': // string or char
      parseString(text_, i, queue_);
      break;
    case (UChar)'=': // = or ==
      if (text_.tempSubString(i, 2) == UNICODE_STRING_SIMPLE("==")) {
        parseConstToken(i, Token::T_EQ, queue_, 2);
      } else {
        parseConstToken(i, Token::T_ASSIGNMENT, queue_);
      }
      break;
    case (UChar)'!': // ! or !=
      if (text_.tempSubString(i, 2) == UNICODE_STRING_SIMPLE("!=")) {
        parseConstToken(i, Token::T_NEQ, queue_, 2);
      } else {
        parseConstToken(i, Token::T_NOT, queue_);
      }
      break;
    case (UChar)'<': // < or <=
      if (text_.tempSubString(i, 2) == UNICODE_STRING_SIMPLE("<=")) {
        parseConstToken(i, Token::T_LE, queue_, 2);
      } else {
        parseConstToken(i, Token::T_LT, queue_);
      }
      break;
    case (UChar)'>': // > or >=
      if (text_.tempSubString(i, 2) == UNICODE_STRING_SIMPLE(">=")) {
        parseConstToken(i, Token::T_GE, queue_, 2);
      } else {
        parseConstToken(i, Token::T_GT, queue_);
      }
      break;
    case (UChar)'T': // True or identifier
      if (text_.tempSubString(i, 4) == UNICODE_STRING_SIMPLE("True")) {
        parseConstToken(i, Token::T_TRUE, queue_, 4);
      } else {
        parseIdentifier(text_, i, queue_);
      }
      break;
    case (UChar)'F': // False or identifier
      if (text_.tempSubString(i, 5) == UNICODE_STRING_SIMPLE("False")) {
        parseConstToken(i, Token::T_FALSE, queue_, 5);
      } else {
        parseIdentifier(text_, i, queue_);
      }
      break;
    case (UChar)'A':
    case (UChar)'B':
    case (UChar)'C':
    case (UChar)'D':
    case (UChar)'E':
    // case (UChar)'F':
    case (UChar)'G':
    case (UChar)'H':
    case (UChar)'I':
    case (UChar)'J':
    case (UChar)'K':
    case (UChar)'L':
    case (UChar)'M':
    case (UChar)'N':
    case (UChar)'O':
    case (UChar)'P':
    case (UChar)'Q':
    case (UChar)'R':
    case (UChar)'S':
    // case (UChar)'T':
    case (UChar)'U':
    case (UChar)'V':
    case (UChar)'W':
    case (UChar)'X':
    case (UChar)'Y':
    case (UChar)'Z':
    case (UChar)'a':
    case (UChar)'b':
    case (UChar)'c':
    case (UChar)'d':
    case (UChar)'e':
    case (UChar)'f':
    case (UChar)'g':
    case (UChar)'h':
    case (UChar)'i':
    case (UChar)'j':
    case (UChar)'k':
    case (UChar)'l':
    case (UChar)'m':
    case (UChar)'n':
    case (UChar)'o':
    case (UChar)'p':
    case (UChar)'q':
    case (UChar)'r':
    case (UChar)'s':
    case (UChar)'t':
    case (UChar)'u':
    case (UChar)'v':
    case (UChar)'w':
    case (UChar)'x':
    case (UChar)'y':
    case (UChar)'z':
    case (UChar)'_':
      parseIdentifier(text_, i, queue_);
      break;
    case (UChar)',':
      parseConstToken(i, Token::T_COMMA, queue_);
    case (UChar)';':
      parseConstToken(i, Token::T_SEMI, queue_);
    case (UChar)'?':
      parseConstToken(i, Token::T_QUESTION, queue_);
    case (UChar)':':
      parseConstToken(i, Token::T_COLON, queue_);
    case (UChar)'(':
      parseConstToken(i, Token::T_LP, queue_);
    case (UChar)')':
      parseConstToken(i, Token::T_RP, queue_);
    case (UChar)'[':
      parseConstToken(i, Token::T_LBRACKET, queue_);
    case (UChar)']':
      parseConstToken(i, Token::T_RBRACKET, queue_);
    case (UChar)'{':
      parseConstToken(i, Token::T_LBRACE, queue_);
    case (UChar)'}':
      parseConstToken(i, Token::T_RBRACE, queue_);
    default:
      F_CHECK(false, "unknown token at text_[{}]: {}", i,
              F_SUB_STRING(text_, i, _1));
    }
  }
}

} // namespace fastype

#undef F_SUB_STRING
