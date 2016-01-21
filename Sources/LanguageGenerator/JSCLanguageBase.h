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
  void setRenameMap(const RenameMap& renameMap);

  void addLeafClass(const LeafClass& leafClass);

  void add(const AdditionalClass& additionalClass);
  void add(const JSCEnumPointer& enumObj);
  void add(const JSCObjectPointer& object);

  void removeEqualsOutput();

  const std::vector<JSCOutput>& outputs() const;

 protected:
  virtual std::vector<JSCOutput> generateOutput(const JSCEnumPointer& enumObj) const = 0;
  virtual std::vector<JSCOutput> generateOutput(const JSCObjectPointer& object) const = 0;

  std::string generateLicenceHeader(std::string filename) const;

  bool isIgnore(const std::string& name) const;
  bool isIgnore(const JSCPropertyPointer& property) const;
  bool isIgnoreEnum(const JSCEnumPointer& enumObj) const;
  bool isIgnoreObj(const JSCObjectPointer& object) const;

  std::string toCamelCase(std::string str, bool firstUpper = false) const;
  std::string renamed(const std::string& str) const;

  std::vector<JSCPropertyPointer> propertiesForProperty(const JSCPropertyPointer& property) const;
  std::vector<JSCPropertyPointer> propertiesForObj(const JSCObjectPointer& object) const;
  bool checkIsLeaf(const JSCPropertyPointer& property) const;

  std::vector<JSCObjectPointer> findAdditionalClasses(const JSCObjectPointer& object) const;

  JSCPropertyPointer recursiveProperty(const JSCPropertyPointer& property) const;

 protected:
  std::string m_prefix;
  std::string m_tab;
  std::string m_licenceHeader;

  IgnoreList m_ignoreList;
  std::set<std::string> m_leafClasses;
  RenameMap m_renameMap;
  std::vector<JSCOutput> m_outputs;

  std::vector<JSCObjectPointer> m_additionalClasses;
};

#endif /* JSON_SCHEMA_CODE_LANGUAGE_BASE_H__ */
