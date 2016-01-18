//
//  JSCObjcJSONModelLanguage.h
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/18/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#pragma once
#ifndef JSON_SCHEMA_CODE_LANGUAGE_OBJC_JSONMODEL_H__
#define JSON_SCHEMA_CODE_LANGUAGE_OBJC_JSONMODEL_H__

#include "IJSCLanguage.h"

class JSCObjcJSONModelLanguage : public IJSCLanguage {
 public:
  void setPrefix(std::string prefix) override;

  void setIgnoreList(const IgnoreList& ignoreList) override;
  void setRenameMap(const RenameMap& renameMap) override;

  void add(const JSCEnumPointer& enumObj) override;
  void add(const JSCObjectPointer& object) override;

  const std::vector<JSCOutput>& outputs() const override;

 private:
  void addOutput(const JSCOutput& output);

  std::vector<JSCOutput> generateOutput(const JSCEnumPointer& enumObj) const;
  std::vector<JSCOutput> generateOutput(const JSCObjectPointer& object) const;

  JSCOutput generateOutputHeader(const JSCObjectPointer& object) const;
  JSCOutput generateOutputSource(const JSCObjectPointer& object) const;

  std::string generateLicenceHeader(std::string fileName) const;
  std::string generateImport(const JSCObjectPointer& object) const;
  std::string generateImportFileName(const JSCPropertyPointer& property) const;

  std::string propertyTypeString(const JSCPropertyPointer& property) const;
  std::string propertyModificatorString(const JSCPropertyPointer& property) const;

  bool isIgnore(const JSCEnumPointer& enumObj) const;
  bool isIgnore(const JSCObjectPointer& object) const;

  std::string className(const JSCObjectPointer& object) const;
  std::string enumName(const JSCEnumPointer& enumObj) const;
  std::string propertyName(const JSCPropertyPointer& property) const;
  std::string toCamelCase(std::string str, bool firstUpper = false) const;

 private:
  std::string m_prefix;

  IgnoreList m_ignoreList;
  RenameMap m_renameMap;
  std::vector<JSCOutput> m_outputs;
};

#endif /* JSON_SCHEMA_CODE_LANGUAGE_OBJC_JSONMODEL_H__ */
