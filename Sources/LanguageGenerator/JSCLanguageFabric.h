//
//  JSCLanguageFabric.h
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/22/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#pragma once
#ifndef JSON_SCHEMA_CODE_LANGUAGE_FABRIC_H__
#define JSON_SCHEMA_CODE_LANGUAGE_FABRIC_H__

#include "JSCLanguageBase.h"

class JSCLanguageFabric {
 public:
  static std::unique_ptr<JSCLanguageBase> getLanguage(std::string language, std::string lib);
};

#endif /* JSON_SCHEMA_CODE_LANGUAGE_FABRIC_H__ */
