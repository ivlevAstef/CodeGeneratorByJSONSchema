//
//  JSCObject.h
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/13/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#pragma once
#ifndef JSON_SCHEMA_CODE_OBJECT_H__
#define JSON_SCHEMA_CODE_OBJECT_H__

#include "JSCProperty.h"

class JSCObject : public JSCProperty {
 public:
  JSCObject();

  bool addProperty(const JSCPropertyPointer property);

  const std::vector<sJSCPropertyPointer>& properties() const;

 private:
  std::vector<sJSCPropertyPointer> m_properties;
};

#endif /* JSON_SCHEMA_CODE_OBJECT_H__ */
