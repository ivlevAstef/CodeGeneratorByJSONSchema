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

void JSCLanguageBase::setLeafClasses(const LeafClasses& leafClasses, const std::vector<const JSCObjectPointer>& classes) {
  m_leafClasses = leafClasses;

  m_leafIgnoreList.clear();
  for (const auto& object : classes) {
    if (m_leafClasses.count(object->rootName())) {
      fillLeafIgnoreListFor(object);
    }
  }
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

void JSCLanguageBase::add(const AdditionalClass& additionalClass) {
  std::vector<JSCPropertyPointer> properties;

  for (const auto& propertyPair : additionalClass.properties) {
    JSCPropertyType type = JSCProperty::propertyStringToType(std::get<0>(propertyPair));
    if (JSCProperty_Unknown == type) {
      SIAWarning("unsupported property type: %s", std::get<0>(propertyPair).c_str());
      continue;
    }

    JSCPropertyPointer property(new JSCProperty(type));
    property->setPath({std::get<1>(propertyPair)});
    property->setOptional({std::get<2>(propertyPair)});
    properties.push_back(property);
  }

  JSCObjectPointer newClass(new JSCObject(properties));
  newClass->setRootName(additionalClass.className);
  newClass->setPath({additionalClass.name});
  newClass->setOptional(additionalClass.optional);

  m_additionalClasses.push_back(newClass);

  for (const auto& out : generateOutput(newClass)) {
    m_outputs.push_back(out);
  }
}

void JSCLanguageBase::removeEqualsOutput() {
  for (size_t i = 0; i < m_outputs.size() - 1; i++) {
    const auto& out = m_outputs[i];

    const auto& removedList = std::remove_if(m_outputs.begin() + i + 1, m_outputs.end(), [&out](const JSCOutput& check) {
      return out.fileName() == check.fileName();
    });

    size_t size = m_outputs.size();
    m_outputs.erase(removedList, m_outputs.end());

    if (size != m_outputs.size()) {
      SIAWarning("Removed equals output: %s (%d)", out.fileName().c_str(), size - m_outputs.size());
    }
  }
}

bool JSCLanguageBase::isIgnore(const std::string& name, bool isLeaf) const {
  return m_ignoreList.count(name) > 0 || (!isLeaf && m_leafIgnoreList.count(name) > 0);
}

bool JSCLanguageBase::isIgnoreEnum(const JSCEnumPointer& enumObj) const {
  return isIgnore(enumObj->enumName());
}

bool JSCLanguageBase::isIgnoreObj(const JSCObjectPointer& object, bool isLeaf) const {
  return isIgnore(object->rootName(), isLeaf);
}

bool JSCLanguageBase::isIgnore(const JSCPropertyPointer& property, bool isLeaf) const {
  bool ignore = m_ignoreList.count(property->pathName()) > 0;

  if (nullptr != property.get()) {
    ignore |= JSCProperty_Ref == property->type() && isIgnore(std::static_pointer_cast<JSCRef>(property)->refProperty(), isLeaf);
    ignore |= JSCProperty_Array == property->type() && isIgnore(std::static_pointer_cast<JSCArray>(property)->propertyType(), isLeaf);

    ignore |= JSCProperty_Object == property->type() && isIgnoreObj(std::static_pointer_cast<JSCObject>(property), isLeaf);
    ignore |= JSCProperty_Enum == property->type() && isIgnoreEnum(std::static_pointer_cast<JSCEnum>(property));
  }

  return ignore;
}

static bool equalProperty(const JSCPropertyPointer& prop1, const JSCPropertyPointer& prop2) {
  return prop1->type() == prop2->type() && prop1->pathName() == prop2->pathName();
}

static bool containsPropertyInClasses(const JSCPropertyPointer& checkProperty, const std::vector<JSCObjectPointer>& classes) {
  for (const auto& object : classes) {
    for (const auto& property : object->properties()) {
      if (equalProperty(checkProperty, property)) {
        return true;
      }
    }
  }
  return false;
}

static bool containsClassInClass(const JSCObjectPointer& container, const JSCObjectPointer& object) {
  for (const auto& checkProperty : container->properties()) {
    bool found = false;
    for (const auto& property : object->properties()) {
      found |= equalProperty(checkProperty, property);
    }
    if (!found) {
      return false;
    }
  }

  return true;
}

std::vector<JSCPropertyPointer> JSCLanguageBase::propertiesForProperty(const JSCPropertyPointer& property, bool isLeaf) const {
  if (isLeaf) {
    if (JSCProperty_Object == property->type()) {
      return propertiesForObj(std::static_pointer_cast<JSCObject>(property), isLeaf);
    } else if (JSCProperty_Ref == property->type()) {
      return propertiesForProperty(std::static_pointer_cast<JSCRef>(property)->refProperty(), isLeaf);
    } else if (JSCProperty_Array == property->type()) {
      return propertiesForProperty(std::static_pointer_cast<JSCArray>(property)->propertyType(), isLeaf);
    }
  }

  return std::vector<JSCPropertyPointer>{property};
}

std::vector<JSCPropertyPointer> JSCLanguageBase::propertiesForObj(const JSCObjectPointer& object, bool isLeaf) const {
  SIAAssert(nullptr != object.get());

  std::vector<JSCPropertyPointer> result;
  result.reserve(object->properties().size());

  std::vector<JSCObjectPointer> additionalClasses = findAdditionalClasses(object);
  isLeaf |= (m_leafClasses.count(object->rootName()) > 0);

  for (const auto& property : object->properties()) {
    if (!isIgnore(property, isLeaf) && !containsPropertyInClasses(property, additionalClasses)) {
      for (const auto& childProperty : propertiesForProperty(property, isLeaf)) {
        result.push_back(childProperty);
      }
    }
  }

  for (const auto& additionalClass : additionalClasses) {
    result.push_back(additionalClass);
  }

  return result;
}

std::vector<JSCObjectPointer> JSCLanguageBase::findAdditionalClasses(const JSCObjectPointer& object) const {
  std::vector<JSCObjectPointer> result;

  for (const auto& additionalClass : m_additionalClasses) {
    if (additionalClass->rootName() == object->rootName()) {
      continue;
    }

    if (containsClassInClass(additionalClass, object)) {
      result.push_back(additionalClass);
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

  return result + '\n';
}

void JSCLanguageBase::fillLeafIgnoreListFor(const JSCObjectPointer& object) {
  for (const auto& property : object->properties()) {
    JSCPropertyPointer checkProperty = property;

    if (JSCProperty_Ref == property->type()) {
      checkProperty = std::static_pointer_cast<JSCRef>(property)->refProperty();
    } else if (JSCProperty_Array == property->type()) {
      checkProperty = std::static_pointer_cast<JSCArray>(property)->propertyType();
    }

    if (JSCProperty_Object == checkProperty->type()) {
      JSCObjectPointer checkObject = std::static_pointer_cast<JSCObject>(checkProperty);
      m_leafIgnoreList.insert(checkObject->rootName());
      fillLeafIgnoreListFor(checkObject);
    }
  }
}
