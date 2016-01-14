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

#include <vector>
#include "JSCProperty.h"

class JSCArray : public JSCProperty {
 public:
  JSCArray(JSCPropertyPointer propertyType);

  const JSCPropertyPointer& propertyType() const;

 private:
  JSCPropertyPointer m_propertyType;
};

#endif /* JSON_SCHEMA_CODE_ARRAY_H__ */
