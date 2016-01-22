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

#include "JSCArray.h"
#include "JSCObjcBaseLanguage.h"

class JSCObjcJSONModelLanguage : public JSCObjcBaseLanguage {
 private:
  std::vector<JSCOutput> generateOutput(const JSCEnumPointer& enumObj) const override;
  std::vector<JSCOutput> generateOutput(const JSCObjectPointer& object) const override;

  JSCOutput generateOutputHeader(const JSCEnumPointer& enumObj) const;
  JSCOutput generateOutputSource(const JSCEnumPointer& enumObj) const;

  JSCOutput generateOutputHeader(const JSCObjectPointer& object) const;
  JSCOutput generateOutputSource(const JSCObjectPointer& object) const;

  std::string generateImport(const JSCObjectPointer& object) const;

  std::string arrayProtocol(const JSCArrayPointer& array, bool fromArray = false) const;
  std::string propertyTypeString(const JSCPropertyPointer& property, bool fromArray = false) const;
  bool propertyIsOptional(const JSCPropertyPointer& property, bool fromArray = false) const;
  std::string propertyModificatorString(const JSCPropertyPointer& property) const;
};

#endif /* JSON_SCHEMA_CODE_LANGUAGE_OBJC_JSONMODEL_H__ */
