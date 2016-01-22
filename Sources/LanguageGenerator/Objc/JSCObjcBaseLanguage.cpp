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
  return m_prefix + renamed(toCamelCase(renamed(object->rootName()), true));
}

std::string JSCObjcBaseLanguage::enumName(const JSCEnumPointer& enumObj) const {
  return "E" + enumClassName(enumObj);
}

std::string JSCObjcBaseLanguage::enumClassName(const JSCEnumPointer& enumObj) const {
  return m_prefix + renamed(toCamelCase(renamed(enumObj->enumName()), true));
}

std::string JSCObjcBaseLanguage::propertyName(const JSCObjectPointer& parent, const JSCPropertyPointer& property) const {
  std::string name = relativePath(parent->path(), property->path(), "_", 2);
  return renamed(toCamelCase(renamed(name)));
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