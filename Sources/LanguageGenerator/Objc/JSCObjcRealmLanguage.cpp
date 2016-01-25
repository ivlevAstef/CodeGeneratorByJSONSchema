//
//  JSCObjcJSONModelLanguage.cpp
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/18/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#include <algorithm>
#include "JSCArray.h"
#include "JSCObjcRealmLanguage.h"
#include "JSCRef.h"
#include "SIALogger.h"

static const std::string sRealmUniqueId = "realmUniqueId";

std::vector<JSCOutput> JSCObjcRealmLanguage::generateOutput(const JSCEnumPointer& enumObj) const {
  return std::vector<JSCOutput>();
}

std::vector<JSCOutput> JSCObjcRealmLanguage::generateOutput(const JSCObjectPointer& object) const {
  return std::vector<JSCOutput>{generateOutputHeader(object)};
}

std::string JSCObjcRealmLanguage::primaryKeyName(const JSCObjectPointer& object) const {
  std::vector<std::string> primaryKeyNames;
  for (const auto& property : propertiesForObj(object)) {
    if (property->pathName() == "id") {
      primaryKeyNames.push_back(propertyName(object, property));
    }
  }

  if (!primaryKeyNames.empty()) {
    //incorrect check (primaryKeyNames[0] == "objId"), but needed for DtoCatalogJob where jobId, not primary
    if (1 == primaryKeyNames.size() && primaryKeyNames[0] == "objId") {
      return primaryKeyNames[0];
    } else {
      return sRealmUniqueId;
    }
  }

  return "";
}

std::string JSCObjcRealmLanguage::primaryKeyType(const JSCObjectPointer& object) const {
  for (const auto& property : propertiesForObj(object)) {
    if (property->pathName() == "id") {
      return propertyTypeString(property);
    }
  }

  return "";
}

JSCOutput JSCObjcRealmLanguage::generateOutputHeader(const JSCObjectPointer& object) const {
  std::string name = className(object);
  std::string fileName = name + ".h";

  std::string text = generateLicenceHeader(fileName);
  std::string primaryKey = primaryKeyName(object);

  text += generateImport(object);
  text += "\n";

  text += "@interface " + name + " : RLMObject\n";
  text += "\n";
  for (const auto& property : propertiesForObj(object)) {
    text += "@property " + propertyTypeString(property) + " " + propertyName(object, property) + ";\n";
  }
  if (sRealmUniqueId == primaryKey) {
    text += "@property " + primaryKeyType(object) + " " + primaryKey + ";\n";
  }

  text += "\n";
  text += "@end\n\n";

  text += "RLM_ARRAY_TYPE(" + name + ")\n\n";

  text += "@implementation " + name + "\n";

  if (!primaryKey.empty()) {
    text += "+ (NSString*)primaryKey {\n";
    text += m_tab + "return @\"" + primaryKey + "\";\n";
    text += "}\n";
  }

  text += "@end\n\n";

  return JSCOutput(fileName, text);
}

std::string JSCObjcRealmLanguage::generateImport(const JSCObjectPointer& object) const {
  std::set<std::string> importFiles{"Realm/Realm.h"};

  for (const auto& propertyIter : propertiesForObj(object)) {
    JSCPropertyPointer property = recursiveProperty(propertyIter);

    if (JSCProperty_Array == property->type()) {
      property = recursiveProperty(propertyIter, true);

      std::string protocol = protocolName(propertyTypeString(property));
      importFiles.insert(protocol + ".h");
    } else if (JSCProperty_Object == property->type()) {
      importFiles.insert(generateImportFileName(property));
    }
  }

  std::string result;
  for (const auto& importFileName : importFiles) {
    if (!importFileName.empty()) {
      result += "#import \"" + importFileName + "\"\n";
    }
  }

  return result;
}

std::string JSCObjcRealmLanguage::protocolName(std::string type) const {
  std::string protocol = type;

  protocol.erase(std::remove_if(protocol.begin(), protocol.end(), [](char symbol) {
    return '*' == symbol;
  }));

  auto beginBrace = protocol.find('<');
  auto endBrace = protocol.find('>');
  if (std::string::npos != beginBrace && std::string::npos != endBrace) {
    protocol.replace(beginBrace, endBrace - beginBrace + 1, "");
    return m_prefix + toCamelCase(protocol, true);
  }

  return protocol;
}

std::string JSCObjcRealmLanguage::propertyTypeString(const JSCPropertyPointer& property) const {
  SIAAssert(nullptr != property.get());

  switch (property->type()) {
  case JSCProperty_Ref:
    return propertyTypeString(std::static_pointer_cast<JSCRef>(property)->refProperty());
  case JSCProperty_Any:
  case JSCProperty_MultyType:
    return "id";
  case JSCProperty_Array: {
    std::string protocol = protocolName(propertyTypeString(std::static_pointer_cast<JSCArray>(property)->propertyType()));
    return "RLMArray<" + protocol + "*><" + protocol + ">*";
  }
  case JSCProperty_Boolean:
    return "NSNumber<RLMBool>*";
  case JSCProperty_Enum:
    return "NSNumber<RLMInt>*";
  case JSCProperty_Object:
    return className(std::static_pointer_cast<JSCObject>(property)) + "*";
  case JSCProperty_Integer:
    return "NSNumber<RLMInt>*";
  case JSCProperty_Number:
    return "NSNumber<RLMDouble>*";
  case JSCProperty_String:
    return "NSString*";
  case JSCProperty_Date:
    return "NSDate*";

  default:
    SIAError("incorrect property type");
  }

  return "";
}
