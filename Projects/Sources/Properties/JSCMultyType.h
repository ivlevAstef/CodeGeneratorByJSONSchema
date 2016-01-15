//
//  JSCMultyType.h
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/15/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#pragma once
#ifndef JSON_SCHEMA_CODE_MULTY_TYPE_H__
#define JSON_SCHEMA_CODE_MULTY_TYPE_H__

#include <set>
#include "JSCProperty.h"

class JSCMultyType : public JSCProperty {
 public:
  JSCMultyType(const std::set<JSCPropertyType>& validTypes);
  const std::set<JSCPropertyType>& validTypes() const;

 private:
  std::set<JSCPropertyType> m_validTypes;
};

#endif /* JSON_SCHEMA_CODE_MULTY_TYPE_H__ */
