//
//  JSCLanguageBase.cpp
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/18/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#include "JSCArray.h"
#include "JSCLanguageBase.h"
#include "JSCRef.h"
#include "SIALogger.h"

#include <chrono>   // chrono::system_clock
#include <ctime>    // localtime
#include <iomanip>  // put_time
#include <sstream>  // stringstream

static std::string currentDate() {
  auto now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);

  std::stringstream ss;
  ss << std::put_time(std::localtime(&in_time_t), "%d.%m.%Y");
  return ss.str();
}

void JSCLanguageBase::setPrefix(std::string prefix) {
  m_prefix = prefix;
}

void JSCLanguageBase::setTab(std::string tab) {
  m_tab = tab;
}
void JSCLanguageBase::setLicenceHeader(std::string licenceHeader) {
  m_licenceHeader = licenceHeader;
}

void JSCLanguageBase::setIgnoreList(const IgnoreList& ignoreList) {
  m_ignoreList = ignoreList;
}

void JSCLanguageBase::setRenameMap(const RenameMap& renameMap) {
  m_renameMap = renameMap;
}

void JSCLanguageBase::add(const JSCEnumPointer& enumObj) {
  if (isIgnore(enumObj)) {
    SIADebug("Ignore enum with name: %s", enumObj->enumName().c_str());
    return;
  }

  for (const auto& out : generateOutput(enumObj)) {
    m_outputs.push_back(out);
  }
}

void JSCLanguageBase::add(const JSCObjectPointer& object) {
  if (isIgnore(object)) {
    SIADebug("Ignore class with name: %s", object->rootName().c_str());
    return;
  }

  for (const auto& out : generateOutput(object)) {
    m_outputs.push_back(out);
  }
}

bool JSCLanguageBase::isIgnoreEnum(const JSCEnumPointer& enumObj) const {
  return m_ignoreList.count(enumObj->enumName()) > 0;
}

bool JSCLanguageBase::isIgnoreObj(const JSCObjectPointer& object) const {
  return m_ignoreList.count(object->rootName()) > 0;
}

bool JSCLanguageBase::isIgnore(const JSCPropertyPointer& property) const {
  bool ignore = m_ignoreList.count(property->pathName()) > 0;

  if (nullptr != property.get()) {
    ignore |= JSCProperty_Ref == property->type() && isIgnore(std::static_pointer_cast<JSCRef>(property)->refProperty());
    ignore |= JSCProperty_Array == property->type() && isIgnore(std::static_pointer_cast<JSCArray>(property)->propertyType());

    ignore |= JSCProperty_Object == property->type() && isIgnoreObj(std::static_pointer_cast<JSCObject>(property));
    ignore |= JSCProperty_Enum == property->type() && isIgnoreEnum(std::static_pointer_cast<JSCEnum>(property));
  }

  return ignore;
}

std::vector<JSCPropertyPointer> JSCLanguageBase::propertiesForObjWithoutIgnore(const JSCObjectPointer& object) const {
  SIAAssert(nullptr != object.get());

  std::vector<JSCPropertyPointer> result;
  result.reserve(object->properties().size());

  for (const auto& property : object->properties()) {
    if (!isIgnore(property)) {
      result.push_back(property);
    }
  }
  return result;
}

std::string JSCLanguageBase::toCamelCase(std::string str, bool firstUpper) const {
  if (firstUpper && !str.empty()) {
    str[0] = toupper(str[0]);
  } else {
    str[0] = tolower(str[0]);
  }

  auto findIndex = std::string::npos;
  while (std::string::npos != (findIndex = str.find('_'))) {
    if (findIndex + 1 < str.size()) {
      str[findIndex + 1] = toupper(str[findIndex + 1]);
      str.replace(findIndex, 1, "");
    }
  }

  return str;
}

std::string JSCLanguageBase::renamed(const std::string& str) const {
  return (m_renameMap.count(str) > 0) ? m_renameMap.at(str) : str;
}

const std::vector<JSCOutput>& JSCLanguageBase::outputs() const {
  return m_outputs;
}

std::string JSCLanguageBase::generateLicenceHeader(std::string filename) const {
  static const std::string sFileNameKey = "{FILE_NAME}";
  static const std::string sDateKey = "{DATE}";
  std::string result = m_licenceHeader;

  auto foundIndex = std::string::npos;

  while (std::string::npos != (foundIndex = result.find(sFileNameKey))) {
    result.replace(foundIndex, sFileNameKey.size(), filename);
  }

  while (std::string::npos != (foundIndex = result.find(sDateKey))) {
    result.replace(foundIndex, sDateKey.size(), currentDate());
  }

  return result;
}