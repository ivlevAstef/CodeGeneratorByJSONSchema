//
//  JSCParser.h
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/13/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#pragma once
#ifndef JSON_SCHEMA_CODE_PARSER_H__
#define JSON_SCHEMA_CODE_PARSER_H__

#include <set>
#include "JSCProperty.h"
#include "JSCTokens.h"

class JSCParser {
 private:
  typedef JSCProperty::Path Path;

 public:
  JSCParser(const JSCTokens& tokens);

  const JSCPropertyPointer root() const;

  const std::vector<JSCPropertyPointer> allObjects() const;

 private:
  void parse(const JSCTokens& tokens);
  JSCPropertyPointer parse(const JSCTokens& tokens, const Path& path, size_t& tokenIndex);
  JSCPropertyPointer createProperty(const std::vector<JSCToken>& tokens, const Path& path, const std::vector<JSCPropertyPointer>& children) const;

  JSCPropertyPointer createPropertyByType(JSCPropertyType type, const std::vector<JSCToken>& tokens, const Path& path, const std::vector<JSCPropertyPointer>& children) const;

  JSCPropertyPointer createBooleanProperty(const std::vector<JSCToken>& tokens, const Path& path, const std::vector<JSCPropertyPointer>& children) const;
  JSCPropertyPointer createIntegerProperty(const std::vector<JSCToken>& tokens, const Path& path, const std::vector<JSCPropertyPointer>& children) const;
  JSCPropertyPointer createNumberProperty(const std::vector<JSCToken>& tokens, const Path& path, const std::vector<JSCPropertyPointer>& children) const;
  JSCPropertyPointer createStringProperty(const std::vector<JSCToken>& tokens, const Path& path, const std::vector<JSCPropertyPointer>& children) const;
  JSCPropertyPointer createObjectProperty(const std::vector<JSCToken>& tokens, const Path& path, const std::vector<JSCPropertyPointer>& children) const;
  JSCPropertyPointer createArrayProperty(const std::vector<JSCToken>& tokens, const Path& path, const std::vector<JSCPropertyPointer>& children) const;
  JSCPropertyPointer createAnyProperty(const std::vector<JSCToken>& tokens, const Path& path, const std::vector<JSCPropertyPointer>& children) const;
  JSCPropertyPointer analyzeUnknownProperty(const std::vector<JSCToken>& tokens, const Path& path, const std::vector<JSCPropertyPointer>& children) const;

  bool isBeginBrace(const JSCToken& token) const;
  bool isEndBrace(const JSCToken& token) const;

  JSCPropertyType typeByToken(const JSCToken& token) const;
  std::set<JSCPropertyType> findAndParseTypes(const std::vector<JSCToken>& tokens) const;

  std::vector<JSCToken> findAndParseSimpleOrArray(const std::string foundToken, const std::vector<JSCToken>& tokens) const;
  JSCToken getPropertyByName(const std::string name, const std::vector<JSCToken>& tokens) const;

 private:
  JSCPropertyPointer m_root;
};

#endif /* JSON_SCHEMA_CODE_PARSER_H__ */
