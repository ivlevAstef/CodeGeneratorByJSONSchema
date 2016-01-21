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

  JSCPropertyPointer propertyType() const;

 private:
  JSCPropertyPointer m_propertyType;
};

typedef std::shared_ptr<JSCArray> JSCArrayPointer;

#endif /* JSON_SCHEMA_CODE_ARRAY_H__ */
