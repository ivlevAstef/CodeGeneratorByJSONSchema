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

std::vector<JSCOutput> JSCObjcRealmLanguage::generateOutput(const JSCEnumPointer& enumObj) const {
  return std::vector<JSCOutput>();
}

std::vector<JSCOutput> JSCObjcRealmLanguage::generateOutput(const JSCObjectPointer& object) const {
  return std::vector<JSCOutput>{generateOutputHeader(object)};
}

JSCOutput JSCObjcRealmLanguage::generateOutputHeader(const JSCObjectPointer& object) const {
  std::string name = className(object);
  std::string fileName = name + ".h";

  std::string text = generateLicenceHeader(fileName);
  text += generateImport(object);
  text += "\n";

  text += "@interface " + name + " : RLMObject\n";
  text += "\n";
  for (const auto& property : propertiesForObj(object)) {
    text += "@property " + propertyTypeString(property) + " " + propertyName(object, property) + ";\n";
  }
  text += "\n";
  text += "@end\n\n";

  text += "RLM_ARRAY_TYPE(" + name + ")\n\n";

  text += "@implementation " + name + "\n";

  /*for (const auto& property : propertiesForObj(object)) {
    if (property->pathName() == "id") {
      text += "+ (NSString*)primaryKey {\n";
      text += m_tab + "return @\"" + propertyName(object, property) + "\";\n";
      text += "}\n\n";
      break;
    }
  }*/
  text += "@end\n\n";

  return JSCOutput(fileName, text);
}

std::string JSCObjcRealmLanguage::generateImport(const JSCObjectPointer& object) const {
  std::set<std::string> importFiles{"Realm/Realm.h"};

  for (const auto& property : propertiesForObj(object)) {
    if (JSCProperty_Array == property->type()) {
      std::string protocol = protocolName(propertyTypeString(std::static_pointer_cast<JSCArray>(property)->propertyType()));
      importFiles.insert(protocol + ".h");
    }

    importFiles.insert(generateImportFileName(property));
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

  //if simple type
  if (protocol.size() == type.size()) {
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
    return "NSArray<" + protocol + "*><" + protocol + ">*";
  }
  case JSCProperty_Boolean:
    return "BOOL";
  case JSCProperty_Enum:
    return "NSInteger";
  case JSCProperty_Object:
    return className(std::static_pointer_cast<JSCObject>(property)) + "*";
  case JSCProperty_Integer:
    return "NSInteger";
  case JSCProperty_Number:
    return "NSNumber*";
  case JSCProperty_String:
    return "NSString*";
  case JSCProperty_Date:
    return "NSDate*";

  default:
    SIAError("incorrect property type");
  }

  return "";
}
