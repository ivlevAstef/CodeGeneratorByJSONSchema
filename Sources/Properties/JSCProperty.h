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
  JSCProperty_Date,

  JSCProperty_Object,
  JSCProperty_Array,
  JSCProperty_Any,
  JSCProperty_MultyType,

  JSCProperty_Ref,
};

class JSCProperty {
 public:
  typedef std::vector<std::string> Path;

  static std::string propertyTypeToString(JSCPropertyType type);
  static JSCPropertyType propertyStringToType(const std::string& str);

  static Path toValidPath(const Path& path);

 public:
  JSCProperty(JSCPropertyType type);

  void setCodeGenerate(bool codeGenerate);

  void setTitle(const std::string& title);
  void setDescription(const std::string& description);

  void setPath(const Path& path);

  void setHasNull(bool hasNull);
  void setRequired(bool required);

  const std::string& title() const;
  const std::string& description() const;

  bool isCodeGenerate() const;

  const Path& path() const;
  const Path& fullPath() const;

  const std::string pathName() const;
  const std::string fullPathName() const;

  JSCPropertyType type() const;

  bool hasNull() const;
  bool required() const;

 private:
  Path m_path;
  Path m_fullPath;
  JSCPropertyType m_type;
  bool m_hasNull;
  bool m_required;
  bool m_codeGenerate;

  std::string m_title;
  std::string m_description;
};

typedef std::shared_ptr<JSCProperty> JSCPropertyPointer;

#endif /* JSON_SCHEMA_CODE_PROPERTY_H__ */
