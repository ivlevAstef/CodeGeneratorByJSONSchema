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

void JSCObjcJSONModelLanguage::setPrefix(std::string prefix) {
  m_prefix = prefix;
}

void JSCObjcJSONModelLanguage::setIgnoreList(const IgnoreList& ignoreList) {
  m_ignoreList = ignoreList;
}

void JSCObjcJSONModelLanguage::setRenameMap(const RenameMap& renameMap) {
  m_renameMap = renameMap;
}

void JSCObjcJSONModelLanguage::add(const JSCEnumPointer& enumObj) {
  if (isIgnore(enumObj)) {
    SIADebug("Ignore enum with name: %s", enumObj->enumName().c_str());
    return;
  }

  for (const auto& out : generateOutput(enumObj)) {
    addOutput(out);
  }
}

void JSCObjcJSONModelLanguage::add(const JSCObjectPointer& object) {
  if (isIgnore(object)) {
    SIADebug("Ignore class with name: %s", object->rootName().c_str());
    return;
  }

  for (const auto& out : generateOutput(object)) {
    addOutput(out);
  }
}

bool JSCObjcJSONModelLanguage::isIgnore(const JSCEnumPointer& enumObj) const {
  return m_ignoreList.count(enumObj->enumName()) > 0;
}
bool JSCObjcJSONModelLanguage::isIgnore(const JSCObjectPointer& object) const {
  return m_ignoreList.count(object->rootName()) > 0;
}

std::string JSCObjcJSONModelLanguage::className(const JSCObjectPointer& object) const {
  return m_prefix + toCamelCase(object->rootName(), true);
}

std::string JSCObjcJSONModelLanguage::enumName(const JSCEnumPointer& enumObj) const {
  if (enumObj->path().size() > 3) {
    return m_prefix + toCamelCase(enumObj->path()[2], true) + toCamelCase(enumObj->pathName(), true);
  }

  return m_prefix + toCamelCase(enumObj->pathName(), true);
}

std::string JSCObjcJSONModelLanguage::propertyName(const JSCPropertyPointer& property) const {
  std::string name = toCamelCase(property->pathName());
  if (m_renameMap.count(name) > 0) {
    return m_renameMap.at(name);
  }
  return name;
}

std::string JSCObjcJSONModelLanguage::toCamelCase(std::string str, bool firstUpper) const {
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

void JSCObjcJSONModelLanguage::addOutput(const JSCOutput& output) {
  m_outputs.push_back(output);
}

const std::vector<JSCOutput>& JSCObjcJSONModelLanguage::outputs() const {
  return m_outputs;
}

std::vector<JSCOutput> JSCObjcJSONModelLanguage::generateOutput(const JSCEnumPointer& enumObj) const {
  std::string name = enumName(enumObj);
  std::string fileName = name + ".h";

  std::string text = generateLicenceHeader(fileName);

  text += "typedef NS_ENUM(NSUInteger, " + name + ") {\n";
  for (auto& identifier : enumObj->identifiers()) {
    text += "  " + name + "_" + toCamelCase(identifier, true) + ",\n";
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
  for (const auto& property : object->properties()) {
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
  text += "  return [[JSONKeyMapper alloc] initWithDictionary:@{\n";
  for (const auto& property : object->properties()) {
    text += "    @\"" + property->pathName() + "\" : @\"" + propertyName(property) + "\",\n";
  }

  text += "  }];\n";
  text += "}\n\n";

  ///Optional
  text += "+(BOOL)propertyIsOptional:(NSString*)propertyName {\n";
  for (const auto& property : object->properties()) {
    if (property->optional()) {
      text += "  if ([propertyName isEqualToString: @\"" + propertyName(property) + "\"]) return YES;\n";
    }
  }

  text += "  return NO;\n";
  text += "}\n\n";

  text += "@end\n\n";

  return JSCOutput(fileName, text);
}

std::string JSCObjcJSONModelLanguage::generateImport(const JSCObjectPointer& object) const {
  std::string result = "#import \"JSONModel.h\"\n";

  for (const auto& property : object->properties()) {
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

std::string JSCObjcJSONModelLanguage::generateLicenceHeader(std::string fileName) const {
  std::string result;
  result += "//\n";
  result += "//  " + fileName + "\n";
  result += "//  PROJECT_NAME\n";
  result += "//\n";
  result += "//  Created by Alexander.Ivlev on 1/18/16.\n";
  result += "//  Copyright © 2016 themsteam. All rights reserved.\n";
  result += "//\n\n";

  return result;
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
