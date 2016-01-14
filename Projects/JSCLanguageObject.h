//
//  JSCLanguageObject.h
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/13/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#pragma once
#ifndef JSON_SCHEMA_CODE_LANGUAGE_OBJECT_H__
#define JSON_SCHEMA_CODE_LANGUAGE_OBJECT_H__

#include "../Properties/JSCObject.h"

class JSCLanguageObject {
 public:
  JSCLanguageObject(const JSCObject& object);

  void setIgnore(bool ignore);

  bool isIgnore() const;
  const std::string& name() const;

 private:
  bool m_ignore;
  std::string m_name;
};

#endif /* JSON_SCHEMA_CODE_LANGUAGE_OBJECT_H__ */
