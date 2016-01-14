//
//  JSCTokens.h
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/13/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#pragma once
#ifndef JSON_SCHEMA_CODE_TOKENS_H__
#define JSON_SCHEMA_CODE_TOKENS_H__

#include <vector>
#include "JSCStream.h"

typedef std::string JSCToken;

class JSCTokens {
 public:
  static JSCToken invalidToken;

 public:
  JSCTokens(const JSCStream& stream);

  size_t length() const;
  const JSCToken& operator[](size_t index) const;

 private:
  void createTokens(const JSCStream& stream);

  bool isSpace(const char& symbol) const;
  bool isCharacter(const char& symbol) const;
  bool isQuote(const char& symbol) const;
  bool isCharacterQuote(const char& lastSymbol, const char& symbol) const;

  bool isLineComment(const char& symbol, const char& nextSymbol) const;
  size_t endLineIndex(const size_t fromIndex, const JSCStream& stream) const;

  bool isMultyLineCommentBegin(const char& symbol, const char& nextSymbol) const;
  size_t multyLineCommentEndIndex(const size_t fromIndex, const JSCStream& stream) const;

  void addToken(JSCToken& buffer, bool canEmpty = false);

 private:
  std::vector<JSCToken> m_tokens;
};

#endif /* JSON_SCHEMA_CODE_TOKENS_H__ */
