
#include "JSCArray.h"
#include "JSCEnum.h"
#include "JSCObject.h"
#include "JSCParser.h"
#include "JSCProperty.h"

JSCParser::JSCParser(const JSCTokens& tokens) {
  parse(tokens);
}

const JSCPropertyPointer JSCParser::root() const {
  return m_root;
}

const std::vector<JSCPropertyPointer> JSCParser::allObjects() const {
  return std::vector<JSCPropertyPointer>();
}

void JSCParser::parse(const JSCTokens& tokens) {
}