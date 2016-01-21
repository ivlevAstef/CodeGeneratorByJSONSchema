//
//  JSCObjcJSONModelLanguage.h
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/18/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#pragma once
#ifndef JSON_SCHEMA_CODE_LANGUAGE_OBJC_JSONMODEL_H__
#define JSON_SCHEMA_CODE_LANGUAGE_OBJC_JSONMODEL_H__

#include "JSCLanguageBase.h"

class JSCObjcJSONModelLanguage : public JSCLanguageBase {
 private:
  std::string className(const JSCObjectPointer& object) const;
  std::string enumName(const JSCEnumPointer& enumObj) const;
  std::string enumClassName(const JSCEnumPointer& enumObj) const;
  std::string propertyName(const JSCObjectPointer& parent, const JSCPropertyPointer& property) const;

  std::vector<JSCOutput> generateOutput(const JSCEnumPointer& enumObj) const override;
  std::vector<JSCOutput> generateOutput(const JSCObjectPointer& object) const override;

  JSCOutput generateOutputHeader(const JSCEnumPointer& enumObj) const;
  JSCOutput generateOutputSource(const JSCEnumPointer& enumObj) const;

  JSCOutput generateOutputHeader(const JSCObjectPointer& object) const;
  JSCOutput generateOutputSource(const JSCObjectPointer& object) const;

  std::string generateImport(const JSCObjectPointer& object) const;
  std::string generateImportFileName(const JSCPropertyPointer& property) const;

  std::string propertyTypeString(const JSCPropertyPointer& property, bool useOptional = true) const;
  std::string propertyModificatorString(const JSCPropertyPointer& property) const;
};

#endif /* JSON_SCHEMA_CODE_LANGUAGE_OBJC_JSONMODEL_H__ */
