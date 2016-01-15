//
//  JSCMultyType.cpp
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/15/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#include "JSCMultyType.h"

JSCMultyType::JSCMultyType(const std::set<JSCPropertyType>& validTypes) : JSCProperty(JSCProperty_MultyType) {
  m_validTypes = validTypes;
}

const std::set<JSCPropertyType>& JSCMultyType::validTypes() const {
  return m_validTypes;
}