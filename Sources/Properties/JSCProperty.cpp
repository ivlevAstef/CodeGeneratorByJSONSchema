
#include "JSCProperty.h"

std::string JSCProperty::propertyTypeToString(JSCPropertyType type) {
  switch (type) {
  case JSCProperty_Any:
    return "any";
  case JSCProperty_Array:
    return "array";
  case JSCProperty_Boolean:
    return "boolean";
  case JSCProperty_Enum:
    return "enum";
  case JSCProperty_Integer:
    return "integer";
  case JSCProperty_MultyType:
    return "multy type";
  case JSCProperty_Null:
    return "null";
  case JSCProperty_Number:
    return "number";
  case JSCProperty_Object:
    return "object";
  case JSCProperty_Ref:
    return "ref";
  case JSCProperty_String:
    return "string";
  case JSCProperty_Unknown:
    return "unknown";
  }
  return "";
}

JSCPropertyType JSCProperty::propertyStringToType(const std::string& str) {
  if ("any" == str) {
    return JSCProperty_Any;
  } else if ("bool" == str) {
    return JSCProperty_Boolean;
  } else if ("integer" == str) {
    return JSCProperty_Integer;
  } else if ("number" == str) {
    return JSCProperty_Number;
  } else if ("string" == str) {
    return JSCProperty_String;
  }

  return JSCProperty_Unknown;
}

JSCProperty::JSCProperty(JSCPropertyType type) {
  m_type = type;
  m_optional = false;
}

void JSCProperty::setTitle(const std::string& title) {
  m_title = title;
}

void JSCProperty::setDescription(const std::string& description) {
  m_description = description;
}

void JSCProperty::setPath(const Path& path) {
  m_fullPath = path;
  m_path.clear();
  for (const auto& subpath : path) {
    if (subpath == "properties" || subpath == "items" || subpath == "item") {
      continue;
    }
    m_path.push_back(subpath);
  }
}

void JSCProperty::setOptional(bool optional) {
  m_optional = optional;
}

const std::string& JSCProperty::title() const {
  return m_title;
}

const std::string& JSCProperty::description() const {
  return m_description;
}

const JSCProperty::Path& JSCProperty::path() const {
  return m_path;
}

const JSCProperty::Path& JSCProperty::fullPath() const {
  return m_fullPath;
}

const std::string JSCProperty::pathName() const {
  if (m_path.size() > 0) {
    return m_path[m_path.size() - 1];
  }

  return "";
}

const std::string JSCProperty::fullPathName() const {
  if (m_fullPath.size() > 0) {
    return m_fullPath[m_fullPath.size() - 1];
  }

  return "";
}

JSCPropertyType JSCProperty::type() const {
  return m_type;
}

bool JSCProperty::optional() const {
  return m_optional;
}