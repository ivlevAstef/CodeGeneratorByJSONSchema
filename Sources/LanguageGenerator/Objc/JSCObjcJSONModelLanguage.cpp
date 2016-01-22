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

std::vector<JSCOutput> JSCObjcJSONModelLanguage::generateOutput(const JSCEnumPointer& enumObj) const {
  return std::vector<JSCOutput>{generateOutputHeader(enumObj), generateOutputSource(enumObj)};
}

JSCOutput JSCObjcJSONModelLanguage::generateOutputHeader(const JSCEnumPointer& enumObj) const {
  std::string name = enumName(enumObj);
  std::string className = enumClassName(enumObj);
  std::string fileName = className + ".h";

  std::string text = generateLicenceHeader(fileName);

  text += "typedef enum {\n";
  text += m_tab + className + "_Undefined,\n";
  for (auto& identifier : enumObj->identifiers()) {
    text += m_tab + className + "_" + toCamelCase(identifier, true) + ",\n";
  }
  text += "} " + name + ";\n\n";

  text += "@interface " + className + " : NSObject\n";
  text += "\n";
  text += "+ (" + name + ")toEnum:(NSString*)str;\n";
  text += "+ (NSString*)toString:(" + name + ")value;\n";
  text += "\n";
  text += "+ (NSArray<NSString*>*)data;\n\n";
  text += "@end\n\n";

  return JSCOutput(fileName, text);
}
JSCOutput JSCObjcJSONModelLanguage::generateOutputSource(const JSCEnumPointer& enumObj) const {
  std::string name = enumName(enumObj);
  std::string className = enumClassName(enumObj);
  std::string headerFileName = className + ".h";
  std::string fileName = className + ".m";

  std::string text = generateLicenceHeader(fileName);
  text += "#import \"" + headerFileName + "\"\n";
  text += "\n";

  text += "@implementation " + className + "\n\n";

  //toEnum
  text += "+ (" + name + ")toEnum:(NSString*)str {\n";
  for (auto& identifier : enumObj->identifiers()) {
    std::string valueName = className + "_" + toCamelCase(identifier, true);
    text += m_tab + "if ([str isEqualToString: @\"" + identifier + "\"]) return " + valueName + ";\n";
  }
  text += m_tab + "return " + className + "_Undefined;\n";
  text += "}\n\n";

  //toString
  text += "+ (NSString*)toString:(" + name + ")value {\n";
  text += m_tab + "switch(value) {\n";
  for (auto& identifier : enumObj->identifiers()) {
    std::string valueName = className + "_" + toCamelCase(identifier, true);
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

  text += "@protocol " + name + "\n";
  text += "@end\n\n";

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
    std::string propertyPath = relativePath(object->path(), property->path(), ".");
    if (property->isCodeGenerate()) {
      auto lastPoint = propertyPath.find_last_of(".");
      if (std::string::npos == lastPoint) {
        propertyPath = "@self";
      } else {
        propertyPath = propertyPath.substr(0, lastPoint);
      }
    }
    text += m_tab + m_tab + "@\"" + propertyPath + "\" : @\"" + propertyName(object, property) + "\",\n";
  }

  text += m_tab + "}];\n";
  text += "}\n\n";

  ///Optional
  text += "+ (BOOL)propertyIsOptional:(NSString*)propertyName {\n";
  for (const auto& property : propertiesForObj(object)) {
    if (propertyIsOptional(property)) {
      text += m_tab + "if ([propertyName isEqualToString: @\"" + propertyName(object, property) + "\"]) return YES;\n";
    }
  }

  text += "\n";
  text += m_tab + "return NO;\n";
  text += "}\n\n";

  //Method for convert string to enum, and enum to string
  for (const auto& property : propertiesForObj(object)) {
    if (JSCProperty_Enum == property->type()) {
      std::string name = enumClassName(std::static_pointer_cast<JSCEnum>(property));
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

std::string JSCObjcJSONModelLanguage::arrayProtocol(const JSCArrayPointer& property, bool fromArray) const {
  std::vector<std::string> protocols;
  if (!fromArray && (!property->required() || property->hasNull())) {
    protocols.push_back("Optional");
  }

  JSCPropertyPointer object = recursiveProperty(property->propertyType());
  if (nullptr != object.get() && JSCProperty_Object == object->type()) {
    protocols.push_back(className(std::static_pointer_cast<JSCObject>(object)));
  }

  if (0 == protocols.size()) {
    return "";
  } else if (1 == protocols.size()) {
    return "<" + protocols[0] + ">";
  } else if (2 == protocols.size()) {
    return "<" + protocols[0] + "," + protocols[1] + ">";
  }

  SIAAssert(false);
  return "";
}

std::string JSCObjcJSONModelLanguage::propertyTypeString(const JSCPropertyPointer& property, bool fromArray) const {
  SIAAssert(nullptr != property.get());
  std::string optionalText = (!fromArray && (!property->required() || property->hasNull())) ? "<Optional>" : "";

  switch (property->type()) {
  case JSCProperty_Ref:
    return propertyTypeString(std::static_pointer_cast<JSCRef>(property)->refProperty(), fromArray);
  case JSCProperty_Any:
  case JSCProperty_MultyType:
    return "id" + optionalText;
  case JSCProperty_Array: {
    const JSCArrayPointer& array = std::static_pointer_cast<JSCArray>(property);
    return "NSArray<" + propertyTypeString(array->propertyType(), true) + ">" + arrayProtocol(array, fromArray) + "*";
  }
  case JSCProperty_Boolean:
    return "BOOL";
  case JSCProperty_Enum:
    return fromArray ? "NSString*" : enumName(std::static_pointer_cast<JSCEnum>(property));
  case JSCProperty_Object:
    return className(std::static_pointer_cast<JSCObject>(property)) + optionalText + "*";
  case JSCProperty_Integer:
    return "NSInteger";
  case JSCProperty_Number:
    return "NSNumber" + optionalText + "*";
  case JSCProperty_String:
    return "NSString" + optionalText + "*";
  case JSCProperty_Date:
    return "NSDate" + optionalText + "*";

  default:
    SIAError("incorrect property type");
  }

  return "";
}

bool JSCObjcJSONModelLanguage::propertyIsOptional(const JSCPropertyPointer& property, bool fromArray) const {
  std::string type = propertyTypeString(property, fromArray);
  auto optionalPos = type.find_first_of("Optional");

  return (std::string::npos != optionalPos);
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
  case JSCProperty_Date:
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
