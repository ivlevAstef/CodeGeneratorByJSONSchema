//
//  JSCEnum.h
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/13/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#pragma once
#ifndef JSON_SCHEMA_CODE_ENUM_H__
#define JSON_SCHEMA_CODE_ENUM_H__

#include <vector>
#include "JSCProperty.h"

class JSCEnum : public JSCProperty {
 public:
  JSCEnum(std::vector<std::string> identifiers);

  const std::vector<std::string>& identifiers() const;

 private:
  std::vector<std::string> m_identifiers;
};

#endif /* JSON_SCHEMA_CODE_ENUM_H__ */
