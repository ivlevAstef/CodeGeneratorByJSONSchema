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
#include <vector>

enum JSCPropertyType {
  JSCProperty_Unknown,
  JSCProperty_Null,

  JSCProperty_Boolean,
  JSCProperty_Integer,
  JSCProperty_Number,
  JSCProperty_String,
  JSCProperty_Enum,

  JSCProperty_Object,
  JSCProperty_Array,

  JSCProperty_Ref,
};

class JSCProperty {
 public:
  typedef std::vector<std::string> Path;

 public:
  JSCProperty(JSCPropertyType type);

  void setPath(const Path& path);
  void setOptional(bool optional);

  const Path& path() const;

  JSCPropertyType type() const;
  bool optional() const;

 private:
  Path m_path;
  JSCPropertyType m_type;
  bool m_optional;
};

typedef std::shared_ptr<JSCProperty> JSCPropertyPointer;

#endif /* JSON_SCHEMA_CODE_PROPERTY_H__ */
