// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "dsl/Lexer.h"
#include "Exception.h"
#include "Logging.h"
#include <cstdlib>
#include <cstring>
#include <string>

#define FSUBSTR(x, pos, y)                                                     \
  ((x).tempSubString(pos,                                                      \
                     std::min<int>(64, (int)(x).tempSubString(pos).length()))  \
       .toUTF8String(y))

namespace dsl {

Lexer::Lexer()
    : pos_(0), text_(UNICODE_STRING_SIMPLE("")), textList_(), queue_() {}

Lexer::Lexer(const icu::UnicodeString &text)
    : pos_(0), text_(text), textList_(), queue_() {}

Lexer::Lexer(const std::vector<icu::UnicodeString> &textList)
    : pos_(0), text_(UNICODE_STRING_SIMPLE("")), textList_(), queue_() {
  for (int i = 0; i < (int)textList.size(); i++) {
    textList_.push_back(textList[0]);
  }
}

Lexer::~Lexer() {
  while (!queue_.empty()) {
    queue_.pop_front();
  }
  queue_.clear();
}

void Lexer::append(const icu::UnicodeString &text) {
  textList_.push_back(text);
}

void Lexer::append(const std::vector<icu::UnicodeString> &textList) {
  for (int i = 0; i < (int)textList.size(); i++) {
    textList_.push_back(textList[0]);
  }
}

std::string Lexer::toString() const {
  std::string _1;
  return fmt::format("[ @Lexer pos_:{}, text_:{} ]", pos_,
                     FSUBSTR(text_, 0, _1));
}

std::shared_ptr<Token> Lexer::read() {
  lex();

  if (!queue_.empty()) {
    std::shared_ptr<Token> t = queue_.front();
    queue_.pop_front();
    return t;
  }
  return Token::TEOF;
}

std::shared_ptr<Token> Lexer::peek(int pos) {
  do {
    int n = (int)queue_.size();
    lex();
    if ((int)queue_.size() <= n) {
      break;
    }
  } while (queue_.size() <= pos);
  return queue_.size() <= pos ? Token::TEOF : queue_[pos];
}

// parse whitespace
void Lexer::parseWhitespace() {
  while (u_isspace(text_.charAt(pos_))) {
    pos_++;
  }
}

// parse constant token
void Lexer::parseConstToken(std::shared_ptr<Token> t, int count) {
  queue_.push_back(t);
  pos_ += count;
}

// parse integer or real
void Lexer::parseNumber() {
  std::string _1;
  int dotCount = 0;  // .
  int expCount = 0;  // e or E
  int flagCount = 0; // + or -
  int j = pos_;
  while (j < text_.length()) {
    if (u_isdigit(text_.charAt(j))) {
      j++;
      continue;
    }
    // real number: 23.91
    if (text_.charAt(j) == (UChar)'.') {
      j++;
      dotCount++;
      continue;
    }
    // exp: 12e8
    if (text_.charAt(j) == (UChar)'e' || text_.charAt(j) == (UChar)'E') {
      j++;
      expCount++;

      // exp with flag: 3E+5
      if (j + 1 < text_.length() && (text_.charAt(j + 1) == (UChar)'+' ||
                                     text_.charAt(j + 1) == (UChar)'-')) {
        j++;
        flagCount++;
      }
      continue;
    }
    break;
  }

  FCHECK(dotCount <= 1, "dotCount {} <= 1 error! text_[{}]:{}", dotCount, pos_,
         FSUBSTR(text_, pos_, _1));
  FCHECK(expCount <= 1, "expCount {} <= 1 error! text_[{}]:{}", expCount, pos_,
         FSUBSTR(text_, pos_, _1));
  FCHECK(flagCount <= 2, "flagCount {} <= 2 error! text_[{}]:{}", flagCount,
         pos_, FSUBSTR(text_, pos_, _1));

  // real number
  if (dotCount > 0) {
    queue_.push_back(std::shared_ptr<Token>(
        new Token(FDSL_REAL, text_.tempSubString(pos_, j - pos_))));
  } else {
    // integer number
    queue_.push_back(std::shared_ptr<Token>(
        new Token(FDSL_INTEGER, text_.tempSubString(pos_, j - pos_))));
  }

  pos_ = j;
}

// parse comment
void Lexer::parseComment() {
  icu::UnicodeString unixLineBreak = UNICODE_STRING_SIMPLE("\n");
  icu::UnicodeString blockCommetEnd = UNICODE_STRING_SIMPLE("*/");

  FCHECK(text_.charAt(pos_) == (UChar)'/', "text_[{}] {} == / {}", pos_,
         (int)text_.charAt(pos_), (int)'/');
  FCHECK(pos_ + 1 < text_.length(), "pos_+1 {} < text_.length {}", pos_ + 1,
         text_.length());
  switch (text_.charAt(pos_ + 1)) {
  case (UChar)'/': // line comment
  {
    int lineEndPos = text_.indexOf(unixLineBreak, pos_ + 1);
    pos_ = lineEndPos + 1;
  } break;
  case (UChar)'*': // block comment
  {
    int lineEndPos = text_.indexOf(blockCommetEnd, pos_ + 1);
    pos_ = lineEndPos + 2;
  } break;
  }
}

// parse string
void Lexer::parseString() {
  std::string _1;
  int j = pos_ + 1;
  bool findString = false;
  (void)findString;
  while (j < text_.length()) {
    if (text_.charAt(j) == (UChar)'\\') {
      j += 2;
      continue;
    }
    if (text_.charAt(j) == (UChar)'\"') {
      j += 1;
      findString = true;
      break;
    }
    j += 1;
  }
  FCHECK(findString, "parse string fail at pos_:{}, j:{}, text_[{}]: {}", pos_,
         j, pos_, FSUBSTR(text_, pos_, _1));
  std::shared_ptr<Token> strToken = std::shared_ptr<Token>(
      new Token(FDSL_STRING, text_.tempSubString(pos_, j - pos_)));
  queue_.push_back(strToken);
  pos_ = j;
}

// parse identifier
bool Lexer::parseIdentifier() {
  if (!u_isalpha(text_.charAt(pos_)) && text_.charAt(pos_) != (UChar)'_') {
    return false;
  }
  int j = pos_;
  while (j < text_.length()) {
    if (u_isalnum(text_.charAt(j)) || text_.charAt(j) == (UChar)'_') {
      j++;
    } else {
      break;
    }
  }
  std::string _1;
  FCHECK(j > pos_, "j {} > pos_ {}, text_[{}]:{}", j, pos_, pos_,
         FSUBSTR(text_, pos_, _1));
  std::shared_ptr<Token> idToken = std::shared_ptr<Token>(
      new Token(FDSL_IDENTIFIER, text_.tempSubString(pos_, j - pos_)));
  queue_.push_back(idToken);
  pos_ = j;
  return true;
}

// parse keyword or identifier
// @return   true if is keyword
//           false if not
bool Lexer::parseKeyword() {
  switch (text_.charAt(pos_)) {
  case (UChar)'l':
    if (pos_ + 3 < text_.length() &&
        text_.tempSubString(pos_, 3) == Token::TLET->literal) {
      queue_.push_back(Token::TLET);
      pos_ += 3;
      return true;
    }
    break;
  case (UChar)'n':
    if (pos_ + 4 < text_.length() &&
        text_.tempSubString(pos_, 4) == Token::TNULL->literal) {
      queue_.push_back(Token::TNULL);
      pos_ += 4;
      return true;
    }
    break;
  case (UChar)'i':
    if (pos_ + 2 < text_.length() &&
        text_.tempSubString(pos_, 2) == Token::TIF->literal) {
      queue_.push_back(Token::TIF);
      pos_ += 2;
      return true;
    } else if (pos_ + 10 < text_.length() &&
               text_.tempSubString(pos_, 10) == Token::TISINSTANCE->literal) {
      queue_.push_back(Token::TISINSTANCE);
      pos_ += 10;
      return true;
    } else if (pos_ + 6 < text_.length() &&
               text_.tempSubString(pos_, 6) == Token::TIMPORT->literal) {
      queue_.push_back(Token::TIMPORT);
      pos_ += 6;
      return true;
    }
    break;
  case (UChar)'e':
    if (pos_ + 6 < text_.length() &&
        text_.tempSubString(pos_, 6) == Token::TELSEIF->literal) {
      queue_.push_back(Token::TELSEIF);
      pos_ += 6;
      return true;
    } else if (pos_ + 4 < text_.length() &&
               text_.tempSubString(pos_, 4) == Token::TELSE->literal) {
      queue_.push_back(Token::TELSE);
      pos_ += 4;
      return true;
    }
    break;
  case (UChar)'f':
    if (pos_ + 3 < text_.length() &&
        text_.tempSubString(pos_, 3) == Token::TFOR->literal) {
      queue_.push_back(Token::TFOR);
      pos_ += 3;
      return true;
    } else if (pos_ + 4 < text_.length() &&
               text_.tempSubString(pos_, 4) == Token::TFUNC->literal) {
      queue_.push_back(Token::TFUNC);
      pos_ += 4;
      return true;
    }
    break;
  case (UChar)'w':
    if (pos_ + 5 < text_.length() &&
        text_.tempSubString(pos_, 5) == Token::TWHILE->literal) {
      queue_.push_back(Token::TWHILE);
      pos_ += 5;
      return true;
    }
    break;
  case (UChar)'b':
    if (pos_ + 5 < text_.length() &&
        text_.tempSubString(pos_, 5) == Token::TBREAK->literal) {
      queue_.push_back(Token::TBREAK);
      pos_ += 5;
      return true;
    }
    break;
  case (UChar)'c':
    if (pos_ + 8 < text_.length() &&
        text_.tempSubString(pos_, 8) == Token::TCONTINUE->literal) {
      queue_.push_back(Token::TCONTINUE);
      pos_ += 8;
      return true;
    } else if (pos_ + 5 < text_.length() &&
               text_.tempSubString(pos_, 5) == Token::TCLASS->literal) {
      queue_.push_back(Token::TCLASS);
      pos_ += 5;
      return true;
    }
    break;
  case (UChar)'t':
    if (pos_ + 4 < text_.length() &&
        text_.tempSubString(pos_, 4) == Token::TTYPE->literal) {
      queue_.push_back(Token::TTYPE);
      pos_ += 4;
      return true;
    }
    break;
  case (UChar)'r':
    if (pos_ + 6 < text_.length() &&
        text_.tempSubString(pos_, 6) == Token::TRETURN->literal) {
      queue_.push_back(Token::TRETURN);
      pos_ += 6;
      return true;
    }
    break;
  case (UChar)'F':
    if (pos_ + 5 < text_.length() &&
        text_.tempSubString(pos_, 5) == Token::TFALSE->literal) {
      queue_.push_back(Token::TFALSE);
      pos_ += 5;
      return true;
    }
    break;
  case (UChar)'T':
    if (pos_ + 4 < text_.length() &&
        text_.tempSubString(pos_, 4) == Token::TTRUE->literal) {
      queue_.push_back(Token::TTRUE);
      pos_ += 4;
      return true;
    }
    break;
  }
  return parseIdentifier();
}

void Lexer::lex() {
  if (!hasMore()) {
    return;
  }

  std::string _1;
  parseWhitespace();
  if (parseKeyword()) {
    return;
  }
  switch (text_.charAt(pos_)) {
  case (UChar)'+': {
    if (pos_ + 1 < text_.length() && text_.charAt(pos_ + 1) == (UChar)'+') {
      // ++
      parseConstToken(Token::TINC, 2);
    } else if (pos_ + 1 < text_.length() &&
               text_.charAt(pos_ + 1) == (UChar)'=') {
      // +=
      parseConstToken(Token::TADDASSIGN, 2);
    } else {
      // +
      parseConstToken(Token::TADD);
    }
    return;
  } break;
  case (UChar)'-': {
    if (pos_ + 1 < text_.length() && text_.charAt(pos_ + 1) == (UChar)'-') {
      // --
      parseConstToken(Token::TDEC, 2);
    } else if (pos_ + 1 < text_.length() &&
               text_.charAt(pos_ + 1) == (UChar)'=') {
      // -=
      parseConstToken(Token::TSUBASSIGN, 2);
    } else {
      // -
      parseConstToken(Token::TSUB);
    }
    return;
  } break;
  case (UChar)'*': {
    if (pos_ + 1 < text_.length() && text_.charAt(pos_ + 1) == (UChar)'=') {
      // *=
      parseConstToken(Token::TMULASSIGN, 2);
    } else {
      // *
      parseConstToken(Token::TMUL);
    }
    return;
  } break;
  case (UChar)'/': {
    if (pos_ + 1 < text_.length() && text_.charAt(pos_ + 1) == (UChar)'=') {
      // /=
      parseConstToken(Token::TDIVASSIGN, 2);
    } else if (pos_ + 1 < text_.length() &&
               (text_.charAt(pos_ + 1) == (UChar)'/' ||
                text_.charAt(pos_ + 1) == (UChar)'*')) {
      // comment start /* or //
      parseComment();
    } else {
      // /
      parseConstToken(Token::TDIV);
    }
    return;
  } break;
  case (UChar)'%': {
    if (pos_ + 1 < text_.length() && text_.charAt(pos_ + 1) == (UChar)'=') {
      // %=
      parseConstToken(Token::TMODASSIGN, 2);
    } else {
      // %
      parseConstToken(Token::TMOD);
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
    parseNumber();
    return;
  } break;
  case (UChar)'"': // string or char
    parseString();
    break;
  case (UChar)'=':
    if (text_.tempSubString(pos_, 2) == UNICODE_STRING_SIMPLE("==")) {
      // ==
      parseConstToken(Token::TEQ, 2);
    } else {
      // =
      parseConstToken(Token::TASSIGN);
    }
    break;
  case (UChar)'!':
    if (text_.tempSubString(pos_, 2) == UNICODE_STRING_SIMPLE("!=")) {
      // !
      parseConstToken(Token::TNEQ, 2);
    } else {
      // !=
      parseConstToken(Token::TNOT);
    }
    break;
  case (UChar)'<':
    if (text_.tempSubString(pos_, 2) == UNICODE_STRING_SIMPLE("<=")) {
      // <
      parseConstToken(Token::TLE, 2);
    } else {
      // <=
      parseConstToken(Token::TLT);
    }
    break;
  case (UChar)'>':
    if (text_.tempSubString(pos_, 2) == UNICODE_STRING_SIMPLE(">=")) {
      // >
      parseConstToken(Token::TGE, 2);
    } else {
      // >=
      parseConstToken(Token::TGT);
    }
    break;
  case (UChar)',':
    parseConstToken(Token::TCOMMA);
    break;
  case (UChar)';':
    parseConstToken(Token::TSEMI);
    break;
  case (UChar)'?':
    parseConstToken(Token::TQUESTION);
    break;
  case (UChar)':':
    parseConstToken(Token::TCOLON);
    break;
  case (UChar)'(': {
    parseConstToken(Token::TLPAREN);
    return;
  } break;
  case (UChar)')': {
    parseConstToken(Token::TRPAREN);
    return;
  } break;
  case (UChar)'[':
    parseConstToken(Token::TLBRACKET);
    break;
  case (UChar)']':
    parseConstToken(Token::TRBRACKET);
    break;
  case (UChar)'{':
    parseConstToken(Token::TLBRACE);
    break;
  case (UChar)'}':
    parseConstToken(Token::TRBRACE);
    break;
  default:
    FCHECK(false, "unknown token at text_[{}]: {}", pos_,
           FSUBSTR(text_, pos_, _1));
    FTHROW("invalid token at text_[{}]: {}", pos_, FSUBSTR(text_, pos_, _1));
  }
}

bool Lexer::hasMore() {
  if (pos_ >= text_.length() && textList_.size() > 0) {
    text_ = textList_.front();
    textList_.pop_front();
  }
  return text_.length() > 0 && pos_ < text_.length();
}

} // namespace dsl

#undef FSUBSTR
