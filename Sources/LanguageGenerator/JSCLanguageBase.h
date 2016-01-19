//
//  JSCLanguageBase.h
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/18/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#pragma once
#ifndef JSON_SCHEMA_CODE_LANGUAGE_BASE_H__
#define JSON_SCHEMA_CODE_LANGUAGE_BASE_H__

#include "JSCEnum.h"
#include "JSCObject.h"
#include "JSCOutput.h"
#include "JSCTypes.h"

class JSCLanguageBase {
 public:
  void setPrefix(std::string prefix);
  void setTab(std::string tab);
  void setLicenceHeader(std::string licenceHeader);

  void setIgnoreList(const IgnoreList& ignoreList);
  void setLeafClasses(const LeafClasses& leafClasses, const std::vector<const JSCObjectPointer>& classes);
  void setRenameMap(const RenameMap& renameMap);

  void add(const AdditionalClass& additionalClass);
  void add(const JSCEnumPointer& enumObj);
  void add(const JSCObjectPointer& object);

  void removeEqualsOutput();

  const std::vector<JSCOutput>& outputs() const;

 protected:
  virtual std::vector<JSCOutput> generateOutput(const JSCEnumPointer& enumObj) const = 0;
  virtual std::vector<JSCOutput> generateOutput(const JSCObjectPointer& object) const = 0;

  std::string generateLicenceHeader(std::string filename) const;

  bool isIgnore(const std::string& name, bool isLeaf = false) const;
  bool isIgnore(const JSCPropertyPointer& property, bool isLeaf = false) const;
  bool isIgnoreEnum(const JSCEnumPointer& enumObj) const;
  bool isIgnoreObj(const JSCObjectPointer& object, bool isLeaf = false) const;

  std::string toCamelCase(std::string str, bool firstUpper = false) const;
  std::string renamed(const std::string& str) const;

  std::vector<JSCPropertyPointer> propertiesForProperty(const JSCPropertyPointer& property, bool isLeaf) const;
  std::vector<JSCPropertyPointer> propertiesForObj(const JSCObjectPointer& object, bool isLeaf = false) const;

  std::vector<JSCObjectPointer> findAdditionalClasses(const JSCObjectPointer& object) const;

  void fillLeafIgnoreListFor(const JSCObjectPointer& object);

 protected:
  std::string m_prefix;
  std::string m_tab;
  std::string m_licenceHeader;

  IgnoreList m_ignoreList;
  IgnoreList m_leafIgnoreList;
  LeafClasses m_leafClasses;
  RenameMap m_renameMap;
  std::vector<JSCOutput> m_outputs;

  std::vector<JSCObjectPointer> m_additionalClasses;
};

#endif /* JSON_SCHEMA_CODE_LANGUAGE_BASE_H__ */
