//
//  JSCUnknown.h
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/14/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#pragma once
#ifndef JSON_SCHEMA_CODE_UNKNOWN_H__
#define JSON_SCHEMA_CODE_UNKNOWN_H__

#include <vector>
#include "JSCProperty.h"

class JSCUnknown : public JSCProperty {
 public:
  JSCUnknown(const std::vector<JSCPropertyPointer>& properties);

  const std::vector<JSCPropertyPointer>& properties() const;

 private:
  std::vector<JSCPropertyPointer> m_properties;
};

#endif /* JSON_SCHEMA_CODE_UNKNOWN_H__ */
