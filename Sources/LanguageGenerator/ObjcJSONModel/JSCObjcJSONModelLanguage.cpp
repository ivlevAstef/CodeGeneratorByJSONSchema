//
//  JSCObjcJSONModelLanguage.cpp
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/18/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#include "JSCArray.h"
#include "JSCObjcJSONModelLanguage.h"
#include "JSCRef.h"
#include "SIALogger.h"

std::string JSCObjcJSONModelLanguage::className(const JSCObjectPointer& object) const {
  return m_prefix + renamed(toCamelCase(renamed(object->rootName()), true));
}

std::string JSCObjcJSONModelLanguage::enumName(const JSCEnumPointer& enumObj) const {
  return m_prefix + renamed(toCamelCase(renamed(enumObj->enumName()), true));
}

std::string JSCObjcJSONModelLanguage::propertyName(const JSCPropertyPointer& property) const {
  return renamed(toCamelCase(renamed(property->pathName())));
}

std::vector<JSCOutput> JSCObjcJSONModelLanguage::generateOutput(const JSCEnumPointer& enumObj) const {
  std::string name = enumName(enumObj);
  std::string fileName = name + ".h";

  std::string text = generateLicenceHeader(fileName);

  text += "typedef NS_ENUM(NSUInteger, " + name + ") {\n";
  for (auto& identifier : enumObj->identifiers()) {
    text += m_tab + name + "_" + toCamelCase(identifier, true) + ",\n";
  }
  text += "};\n\n";

  return std::vector<JSCOutput>{JSCOutput(fileName, text)};
}

std::vector<JSCOutput> JSCObjcJSONModelLanguage::generateOutput(const JSCObjectPointer& object) const {
  return std::vector<JSCOutput>{generateOutputHeader(object), generateOutputSource(object)};
}

JSCOutput JSCObjcJSONModelLanguage::generateOutputHeader(const JSCObjectPointer& object) const {
  std::string name = className(object);
  std::string fileName = name + ".h";

  std::string text = generateLicenceHeader(fileName);
  text += generateImport(object);
  text += "\n";

  text += "@interface " + name + " : JSONModel\n";
  text += "\n";
  for (const auto& property : propertiesForObj(object)) {
    text += "@property (" + propertyModificatorString(property) + ", nonatomic) " + propertyTypeString(property) + " " + propertyName(property) + ";\n";
  }
  text += "\n";
  text += "@end\n\n";

  return JSCOutput(fileName, text);
}

JSCOutput JSCObjcJSONModelLanguage::generateOutputSource(const JSCObjectPointer& object) const {
  std::string name = className(object);
  std::string fileName = name + ".m";

  std::string text = generateLicenceHeader(fileName);

  text += "#import \"" + name + ".h" + "\"\n\n";

  text += "@implementation " + name + "\n\n";

  ///Key mapper
  text += "+ (JSONKeyMapper*)keyMapper {\n";
  text += m_tab + "return [[JSONKeyMapper alloc] initWithDictionary:@{\n";
  for (const auto& property : propertiesForObj(object)) {
    text += m_tab + m_tab + "@\"" + property->pathName() + "\" : @\"" + propertyName(property) + "\",\n";
  }

  text += m_tab + "}];\n";
  text += "}\n\n";

  ///Optional
  text += "+ (BOOL)propertyIsOptional:(NSString*)propertyName {\n";
  for (const auto& property : propertiesForObj(object)) {
    if (property->optional()) {
      text += m_tab + "if ([propertyName isEqualToString: @\"" + propertyName(property) + "\"]) return YES;\n";
    }
  }

  text += "\n";
  text += m_tab + "return NO;\n";
  text += "}\n\n";

  text += "@end\n\n";

  return JSCOutput(fileName, text);
}

std::string JSCObjcJSONModelLanguage::generateImport(const JSCObjectPointer& object) const {
  std::string result = "#import \"JSONModel.h\"\n";

  for (const auto& property : propertiesForObj(object)) {
    std::string importFileName = generateImportFileName(property);
    if (!importFileName.empty()) {
      result += "#import \"" + importFileName + "\"\n";
    }
  }
  return result;
}

std::string JSCObjcJSONModelLanguage::generateImportFileName(const JSCPropertyPointer& property) const {
  SIAAssert(nullptr != property.get());

  if (JSCProperty_Enum == property->type()) {
    return enumName(std::static_pointer_cast<JSCEnum>(property)) + ".h";
  } else if (JSCProperty_Object == property->type()) {
    return className(std::static_pointer_cast<JSCObject>(property)) + ".h";
  } else if (JSCProperty_Array == property->type()) {
    return generateImportFileName(std::static_pointer_cast<JSCArray>(property)->propertyType());
  } else if (JSCProperty_Ref == property->type()) {
    return generateImportFileName(std::static_pointer_cast<JSCRef>(property)->refProperty());
  }
  return "";
}

std::string JSCObjcJSONModelLanguage::propertyTypeString(const JSCPropertyPointer& property) const {
  SIAAssert(nullptr != property.get());
  std::string optionalText = property->optional() ? "<Optional>" : "";

  switch (property->type()) {
  case JSCProperty_Ref:
    return propertyTypeString(std::static_pointer_cast<JSCRef>(property)->refProperty());
  case JSCProperty_Any:
  case JSCProperty_MultyType:
    return "id" + optionalText;
  case JSCProperty_Array:
    return "NSArray<" + propertyTypeString(std::static_pointer_cast<JSCArray>(property)->propertyType()) + ">" + optionalText + "*";
  case JSCProperty_Boolean:
    return "BOOL";
  case JSCProperty_Enum:
    return enumName(std::static_pointer_cast<JSCEnum>(property));
  case JSCProperty_Object:
    return className(std::static_pointer_cast<JSCObject>(property)) + optionalText + "*";
  case JSCProperty_Integer:
    return "NSInteger";
  case JSCProperty_Number:
    return "NSNumber" + optionalText + "*";
  case JSCProperty_String:
    return "NSString" + optionalText + "*";

  default:
    SIAError("incorrect property type");
  }

  return "";
}

std::string JSCObjcJSONModelLanguage::propertyModificatorString(const JSCPropertyPointer& property) const {
  SIAAssert(nullptr != property.get());

  switch (property->type()) {
  case JSCProperty_Ref:
    return propertyModificatorString(std::static_pointer_cast<JSCRef>(property)->refProperty());
  case JSCProperty_Any:
  case JSCProperty_Array:
  case JSCProperty_MultyType:
  case JSCProperty_Number:
  case JSCProperty_String:
  case JSCProperty_Object:
    return "strong";
  case JSCProperty_Boolean:
  case JSCProperty_Enum:
  case JSCProperty_Integer:
    return "assign";

  default:
    SIAError("incorrect property type");
  }

  return "strong";
}
