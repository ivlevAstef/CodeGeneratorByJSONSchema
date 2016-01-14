
#include "JSCParser.h"

JSCParser::JSCParser(const JSCTokens& tokens) {
}

const JSCPropertyPointer JSCParser::root() const {
  return m_root;
}

const std::vector<JSCPropertyPointer> JSCParser::allObjects() const {
  return std::vector<JSCPropertyPointer>();
}