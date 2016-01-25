//
//  JSCObjcBaseLanguage.h
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/18/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#pragma once
#ifndef JSON_SCHEMA_CODE_LANGUAGE_OBJC_BASE_H__
#define JSON_SCHEMA_CODE_LANGUAGE_OBJC_BASE_H__

#include "JSCLanguageBase.h"

class JSCObjcBaseLanguage : public JSCLanguageBase {
 public:
  void finished() override;

 protected:
  std::string className(const JSCObjectPointer& object) const;
  std::string enumName(const JSCEnumPointer& enumObj) const;
  std::string enumClassName(const JSCEnumPointer& enumObj) const;
  std::string propertyName(const JSCObjectPointer& parent, const JSCPropertyPointer& property) const;

  std::string generateImportFileName(const JSCPropertyPointer& property) const;

  std::string relativePath(const JSCProperty::Path& parentPath, const JSCProperty::Path& childPath, std::string separator, size_t maxDepth = 16) const;
};

#endif /* JSON_SCHEMA_CODE_LANGUAGE_OBJC_BASE_H__ */
