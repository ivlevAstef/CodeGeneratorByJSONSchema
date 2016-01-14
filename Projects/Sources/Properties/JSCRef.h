//
//  JSCRef.h
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/14/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#pragma once
#ifndef JSON_SCHEMA_CODE_OBJECT_H__
#define JSON_SCHEMA_CODE_OBJECT_H__

#include "JSCProperty.h"

class JSCRef : public JSCProperty {
 public:
  JSCRef(const std::string& refPath);

  const std::string& refPath() const;

 private:
  std::string m_refPath;
};

#endif /* JSON_SCHEMA_CODE_OBJECT_H__ */
