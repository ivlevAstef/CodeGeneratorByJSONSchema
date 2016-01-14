
#include "JSCTokens.h"

JSCToken JSCTokens::invalidToken = "++==!@#$%^&*()\"InvalidToken&^%$#@!++==";

JSCTokens::JSCTokens(const JSCStream& stream) {
  createTokens(stream);
}

size_t JSCTokens::length() const {
  return m_tokens.size();
}

const JSCToken& JSCTokens::operator[](size_t index) const {
  if (index < m_tokens.size()) {
    return m_tokens[index];
  }

  //TODO: add error
  return invalidToken;
}

bool JSCTokens::isSpace(const char& symbol) const {
  static const char tab = 9;
  static const char enter = 10;
  static const char caret = 11;
  static const char space = 32;

  return tab == symbol || enter == symbol || caret == symbol || space == symbol;
}

bool JSCTokens::isCharacter(const char& symbol) const {
  return '{' == symbol || '}' == symbol ||
         '[' == symbol || ']' == symbol ||
         ':' == symbol || ',' == symbol;
}

bool JSCTokens::isQuote(const char& symbol) const {
  return '"' == symbol;
}

bool JSCTokens::isCharacterQuote(const char& lastSymbol, const char& symbol) const {
  return '"' == symbol && '\\' == lastSymbol;
}

void JSCTokens::addToken(JSCToken& buffer, bool canEmpty) {
  if (canEmpty || !buffer.empty()) {
    m_tokens.push_back(buffer);
    buffer.clear();
  }
}

void JSCTokens::createTokens(const JSCStream& stream) {
  bool quoteText = false;

  JSCToken buffer;
  for (size_t i = 0; i < stream.length(); i++) {
    const char& symbol = stream[i];

    if (isQuote(symbol)) {
      if (!quoteText) {
        quoteText = true;
      } else if (isCharacterQuote(stream[i - 1], symbol)) {
        buffer += symbol;
      } else {
        addToken(buffer, true);
        quoteText = false;
      }
      continue;
    }

    if (quoteText) {
      buffer += symbol;
      continue;
    }

    if (isSpace(symbol)) {
      addToken(buffer);
      continue;
    }

    if (isCharacter(symbol)) {
      addToken(buffer);
      addToken(std::string(symbol));
      continue;
    }

    buffer += symbol;
  }
}