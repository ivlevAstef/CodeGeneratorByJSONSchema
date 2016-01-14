//
//  JSCProperty.h
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/13/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#pragma once
#ifndef JSON_SCHEMA_CODE_PROPERTY_H__
#define JSON_SCHEMA_CODE_PROPERTY_H__

#include <memory>
#include <string>

enum JSCPropertyType {
  JSCProperty_Boolean,
  JSCProperty_Integer,
  JSCProperty_Number,
  JSCProperty_String,
  JSCProperty_Enum,

  JSCProperty_Object,
  JSCProperty_Array,
};

class JSCProperty {
 public:
  JSCProperty(JSCPropertyType type);

  void setName(std::string name);
  void setPath(std::string path);
  void setOptional(bool optional);

  const std::string& name() const;
  const std::string& path() const;

  JSCPropertyType type() const;
  bool optional() const;

 private:
  std::string m_name;
  std::string m_path;
  JSCPropertyType m_type;
  bool m_optional;
};

typedef std::shared_ptr<JSCProperty> JSCPropertyPointer;

#endif /* JSON_SCHEMA_CODE_PROPERTY_H__ */
