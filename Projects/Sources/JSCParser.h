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

#include "JSCProperty.h"
#include "JSCTokens.h"

class JSCParser {
 public:
  JSCParser(const JSCTokens& tokens);

  const JSCPropertyPointer root() const;

  const std::vector<JSCPropertyPointer> allObjects() const;

 private:
  void parse(const JSCTokens& tokens);

 private:
  JSCPropertyPointer m_root;
};

#endif /* JSON_SCHEMA_CODE_PARSER_H__ */
