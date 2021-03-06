//
//  JSCObjcBaseLanguage.cpp
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/18/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#include "JSCArray.h"
#include "JSCObjcBaseLanguage.h"
#include "JSCRef.h"
#include "SIALogger.h"

std::string JSCObjcBaseLanguage::className(const JSCObjectPointer& object) const {
  return m_prefix + renamed(toCamelCase(renamed(object->path(), object->rootName()), true));
}

std::string JSCObjcBaseLanguage::enumName(const JSCEnumPointer& enumObj) const {
  return "E" + enumClassName(enumObj);
}

std::string JSCObjcBaseLanguage::enumClassName(const JSCEnumPointer& enumObj) const {
  return m_prefix + renamed(toCamelCase(renamed(enumObj->path(), enumObj->enumName()), true));
}

std::string JSCObjcBaseLanguage::propertyName(const JSCObjectPointer& parent, const JSCPropertyPointer& property) const {
  std::string name = relativePath(parent->path(), property->path(), "_", 2);
  return renamed(toCamelCase(renamed(property->path(), name)));
}

std::string JSCObjcBaseLanguage::generateImportFileName(const JSCPropertyPointer& property) const {
  SIAAssert(nullptr != property.get());

  if (JSCProperty_Enum == property->type()) {
    return enumClassName(std::static_pointer_cast<JSCEnum>(property)) + ".h";
  } else if (JSCProperty_Object == property->type()) {
    return className(std::static_pointer_cast<JSCObject>(property)) + ".h";
  } else if (JSCProperty_Array == property->type()) {
    return generateImportFileName(std::static_pointer_cast<JSCArray>(property)->propertyType());
  } else if (JSCProperty_Ref == property->type()) {
    return generateImportFileName(std::static_pointer_cast<JSCRef>(property)->refProperty());
  }
  return "";
}

std::string JSCObjcBaseLanguage::relativePath(const JSCProperty::Path& parentPath, const JSCProperty::Path& childPath, std::string separator, size_t maxDepth) const {
  JSCProperty::Path finalPath = childPath;

  for (const auto& subpath : parentPath) {
    if (finalPath[0] == subpath) {
      finalPath.erase(finalPath.begin());
    } else {
      break;
    }
  }

  if (maxDepth < finalPath.size()) {
    finalPath.erase(finalPath.begin(), finalPath.begin() + (finalPath.size() - maxDepth));
  }

  std::string result;
  for (const auto& subpath : finalPath) {
    result += subpath + separator;
  }

  if (!result.empty()) {
    result.resize(result.size() - separator.size());
  }

  return result;
}

void JSCObjcBaseLanguage::finished() {
  JSCLanguageBase::finished();

  std::set<std::string> allHeaders;

  for (const auto& output : m_outputs) {
    if (std::string::npos != output.fileName().find(".h")) {
      allHeaders.insert(output.fileName());
    }
  }

  std::string fileName = m_prefix + "DtoAllTypes.h";

  std::string text = generateLicenceHeader(fileName);
  text += "\n";
  for (const auto& header : allHeaders) {
    text += "#import \"" + header + "\"\n";
  }
  text += "\n";

  m_outputs.push_back(JSCOutput(fileName, text));
}