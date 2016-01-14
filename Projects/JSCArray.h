//
//  JSCArray.h
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/13/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#pragma once
#ifndef JSON_SCHEMA_CODE_ARRAY_H__
#define JSON_SCHEMA_CODE_ARRAY_H__

#include "JSCProperty.h"

class JSCArray : public JSCProperty {
 public:
  JSCArray();

  bool addPropertyType(const JSCPropertyPointer property);

  const std::vector<JSCPropertyPointer>& propertyTypes() const;

 private:
  std::vector<JSCPropertyPointer> m_propertyTypes;
};

#endif /* JSON_SCHEMA_CODE_ARRAY_H__ */
