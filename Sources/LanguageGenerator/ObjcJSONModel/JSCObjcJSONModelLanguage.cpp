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

static std::string relativePath(const JSCProperty::Path& parentPath, const JSCProperty::Path& childPath, std::string separator, size_t maxDepth = 16) {
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

std::string JSCObjcJSONModelLanguage::className(const JSCObjectPointer& object) const {
  return m_prefix + renamed(toCamelCase(renamed(object->rootName()), true));
}

std::string JSCObjcJSONModelLanguage::enumName(const JSCEnumPointer& enumObj) const {
  return m_prefix + renamed(toCamelCase(renamed(enumObj->enumName()), true));
}

std::string JSCObjcJSONModelLanguage::propertyName(const JSCObjectPointer& parent, const JSCPropertyPointer& property) const {
  std::string name = relativePath(parent->path(), property->path(), "_", 2);
  return renamed(toCamelCase(renamed(name)));
}

std::vector<JSCOutput> JSCObjcJSONModelLanguage::generateOutput(const JSCEnumPointer& enumObj) const {
  return std::vector<JSCOutput>{generateOutputHeader(enumObj), generateOutputSource(enumObj)};
}

JSCOutput JSCObjcJSONModelLanguage::generateOutputHeader(const JSCEnumPointer& enumObj) const {
  std::string name = enumName(enumObj);
  std::string nameEnum = "E" + name;
  std::string fileName = name + ".h";

  std::string text = generateLicenceHeader(fileName);

  text += "typedef enum {\n";
  text += m_tab + name + "_Undefined,\n";
  for (auto& identifier : enumObj->identifiers()) {
    text += m_tab + name + "_" + toCamelCase(identifier, true) + ",\n";
  }
  text += "} " + nameEnum + ";\n\n";

  text += "@interface " + name + "\n";
  text += "\n";
  text += "+ (" + nameEnum + ")toEnum:(NSString*)str;\n";
  text += "+ (NSString*)toString:(" + nameEnum + ")value;\n";
  text += "\n";
  text += "+ (NSArray<NSString*>*)data;\n\n";
  text += "@end\n\n";

  return JSCOutput(fileName, text);
}
JSCOutput JSCObjcJSONModelLanguage::generateOutputSource(const JSCEnumPointer& enumObj) const {
  std::string name = enumName(enumObj);
  std::string nameEnum = "E" + name;
  std::string headerFileName = name + ".h";
  std::string fileName = name + ".m";

  std::string text = generateLicenceHeader(fileName);
  text += "#import \"" + headerFileName + "\"\n";
  text += "\n";

  text += "@implementation " + name + "\n\n";

  //toEnum
  text += "+ (" + nameEnum + ")toEnum:(NSString*)str {\n";
  for (auto& identifier : enumObj->identifiers()) {
    std::string valueName = name + "_" + toCamelCase(identifier, true);
    text += m_tab + "if ([str isEqualToString: @\"" + identifier + "\"]) return " + valueName + ";\n";
  }
  text += m_tab + "return " + name + "_Undefined;\n";
  text += "}\n\n";

  //toString
  text += "+ (NSString*)toString:(" + nameEnum + ")value {\n";
  text += m_tab + "switch(value) {\n";
  for (auto& identifier : enumObj->identifiers()) {
    std::string valueName = name + "_" + toCamelCase(identifier, true);
    text += m_tab + "case " + valueName + ":\n" + m_tab + m_tab + "return @\"" + identifier + "\";\n";
  }
  text += m_tab + "default:\n" + m_tab + m_tab + "return @\"\";\n";
  text += m_tab + "}\n";
  text += "}\n\n";

  //data
  text += "+ (NSArray<NSString*>*)data {\n";
  text += m_tab + "return @[\n";
  for (auto& identifier : enumObj->identifiers()) {
    text += m_tab + m_tab + "@\"" + identifier + "\",\n";
  }
  text += m_tab + "];\n";
  text += "}\n\n";

  text += "@end\n\n";

  return JSCOutput(fileName, text);
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
    text += "@property (" + propertyModificatorString(property) + ", nonatomic) " + propertyTypeString(property) + " " + propertyName(object, property) + ";\n";
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
    text += m_tab + m_tab + "@\"" + relativePath(object->path(), property->path(), ".") + "\" : @\"" + propertyName(object, property) + "\",\n";
  }

  text += m_tab + "}];\n";
  text += "}\n\n";

  ///Optional
  text += "+ (BOOL)propertyIsOptional:(NSString*)propertyName {\n";
  for (const auto& property : propertiesForObj(object)) {
    if (property->optional()) {
      text += m_tab + "if ([propertyName isEqualToString: @\"" + propertyName(object, property) + "\"]) return YES;\n";
    }
  }

  text += "\n";
  text += m_tab + "return NO;\n";
  text += "}\n\n";

  //Method for convert string to enum, and enum to string
  for (const auto& property : propertiesForObj(object)) {
    if (JSCProperty_Enum == property->type()) {
      std::string name = enumName(std::static_pointer_cast<JSCEnum>(property));
      std::string nameProperty = propertyName(object, property);
      std::string namePropertyUpper = nameProperty;
      namePropertyUpper[0] = toupper(namePropertyUpper[0]);

      text += "- (void)set" + namePropertyUpper + "WithNSString:(NSString*)str {\n";
      text += m_tab + "_" + nameProperty + " = [" + name + " toEnum:str];\n";
      text += "}\n\n";

      text += "- (id)JSONObjectFor" + namePropertyUpper + " {\n";
      text += m_tab + "return [" + name + " toString: self." + nameProperty + "];\n";
      text += "}\n\n";
    }
  }

  text += "@end\n\n";

  return JSCOutput(fileName, text);
}

std::string JSCObjcJSONModelLanguage::generateImport(const JSCObjectPointer& object) const {
  std::set<std::string> importFiles{"JSONModel.h"};

  for (const auto& property : propertiesForObj(object)) {
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

std::string JSCObjcJSONModelLanguage::propertyTypeString(const JSCPropertyPointer& property, bool useOptional) const {
  SIAAssert(nullptr != property.get());
  std::string optionalText = (useOptional && property->optional()) ? "<Optional>" : "";

  switch (property->type()) {
  case JSCProperty_Ref:
    return propertyTypeString(std::static_pointer_cast<JSCRef>(property)->refProperty(), useOptional);
  case JSCProperty_Any:
  case JSCProperty_MultyType:
    return "id" + optionalText;
  case JSCProperty_Array:
    return "NSArray<" + propertyTypeString(std::static_pointer_cast<JSCArray>(property)->propertyType(), false) + ">" + optionalText + "*";
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
