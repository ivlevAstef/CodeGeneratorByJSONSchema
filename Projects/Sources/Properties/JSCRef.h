//
//  JSCRef.h
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/14/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#pragma once
#ifndef JSON_SCHEMA_CODE_REF_H__
#define JSON_SCHEMA_CODE_REF_H__

#include "JSCProperty.h"

class JSCRef : public JSCProperty {
 public:
  JSCRef(const std::string& refPath);

  void setRefProperty(JSCPropertyPointer refProperty);

  const std::string& refPath() const;
  std::vector<std::string> separatedRefPath() const;

  JSCPropertyPointer refProperty() const;

 private:
  std::string m_refPath;
  JSCPropertyPointer m_refProperty;
};

#endif /* JSON_SCHEMA_CODE_REF_H__ */
