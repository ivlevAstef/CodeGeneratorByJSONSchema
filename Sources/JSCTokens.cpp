
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
  static const char enter = '\n';
  static const char caret = '\r';
  static const char space = 32;
  static const char comma = ',';

  return tab == symbol || enter == symbol || caret == symbol || space == symbol || comma == symbol;
}

bool JSCTokens::isCharacter(const char& symbol) const {
  return '{' == symbol || '}' == symbol ||
         '[' == symbol || ']' == symbol ||
         ':' == symbol;
}

bool JSCTokens::isQuote(const char& symbol) const {
  return '"' == symbol;
}

bool JSCTokens::isCharacterQuote(const char& lastSymbol, const char& symbol) const {
  return '"' == symbol && '\\' == lastSymbol;
}

bool JSCTokens::isLineComment(const char& symbol, const char& nextSymbol) const {
  return '/' == symbol && '/' == nextSymbol;
}

bool JSCTokens::isMultyLineCommentBegin(const char& symbol, const char& nextSymbol) const {
  return '/' == symbol && '*' == nextSymbol;
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

    if (isLineComment(symbol, stream[i + 1])) {
      i = endLineIndex(i, stream);
      continue;
    }

    if (isMultyLineCommentBegin(symbol, stream[i + 1])) {
      i = multyLineCommentEndIndex(i, stream);
      continue;
    }

    if (isSpace(symbol)) {
      addToken(buffer);
      continue;
    }

    if (isCharacter(symbol)) {
      addToken(buffer);
      std::string symbolStr = std::string(1, symbol);
      addToken(symbolStr);
      continue;
    }

    buffer += symbol;
  }
}

size_t JSCTokens::endLineIndex(const size_t fromIndex, const JSCStream& stream) const {
  for (size_t i = fromIndex + 2; i < stream.length(); i++) {
    if ('\n' == stream[i]) {
      return i;
    }
  }
  return stream.length();
}

size_t JSCTokens::multyLineCommentEndIndex(const size_t fromIndex, const JSCStream& stream) const {
  for (size_t i = fromIndex + 2; i < stream.length(); i++) {
    if ('*' == stream[i] && '/' == stream[i + 1]) {
      return i + 1;
    }
  }
  return stream.length();
}