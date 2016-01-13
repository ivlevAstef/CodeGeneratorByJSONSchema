//
//  IJSCLanguage.h
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/13/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#pragma once
#ifndef JSON_SCHEMA_CODE_LANGUAGE_INTERFACE_H__
#define JSON_SCHEMA_CODE_LANGUAGE_INTERFACE_H__

#include "/JSCLanguageObject.h"
#include "JSCOutput.h"

class IJSCLanguage {
 public:
  virtual std::vector<JSCLanguageObject> convert(const std::vector<JSCPropertyPointer> objects) const;

  virtual std::vector<JSCOutput> objectOutputs(const JSCLanguageObject& object) const;
};

#endif /* JSON_SCHEMA_CODE_LANGUAGE_INTERFACE_H__ */
