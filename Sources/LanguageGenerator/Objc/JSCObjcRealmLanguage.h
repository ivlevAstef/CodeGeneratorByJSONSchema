//
//  JSCObjcJSONModelLanguage.h
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/18/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#pragma once
#ifndef JSON_SCHEMA_CODE_LANGUAGE_OBJC_REALM_H__
#define JSON_SCHEMA_CODE_LANGUAGE_OBJC_REALM_H__

#include "JSCObjcBaseLanguage.h"

class JSCObjcRealmLanguage : public JSCObjcBaseLanguage {
 private:
  std::vector<JSCOutput> generateOutput(const JSCEnumPointer& enumObj) const override;
  std::vector<JSCOutput> generateOutput(const JSCObjectPointer& object) const override;

  JSCOutput generateOutputHeader(const JSCObjectPointer& object) const;

  std::string generateImport(const JSCObjectPointer& object) const;

  std::string protocolName(std::string type) const;
  std::string propertyTypeString(const JSCPropertyPointer& property) const;
};

#endif /* JSON_SCHEMA_CODE_LANGUAGE_OBJC_REALM_H__ */
