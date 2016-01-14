
#include "JSCProperty.h"

JSCProperty::JSCProperty(JSCPropertyType type) {
  m_type = type;
  m_optional = false;
}

void JSCProperty::setName(std::string name) {
  m_name = name;
}

void JSCProperty::setPath(std::string path) {
  m_path = path;
}

void JSCProperty::setOptional(bool optional) {
  m_optional = optional;
}

const std::string& JSCProperty::name() const {
  return m_name;
}

const std::string& JSCProperty::path() const {
  return m_path;
}

JSCPropertyType JSCProperty::type() const {
  return m_type;
}

bool JSCProperty::optional() const {
  return m_optional;
}