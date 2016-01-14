
#include "JSCProperty.h"

JSCProperty::JSCProperty(JSCPropertyType type) {
  m_type = type;
  m_optional = false;
}

void JSCProperty::setPath(const Path& path) {
  m_path = path;
}

void JSCProperty::setOptional(bool optional) {
  m_optional = optional;
}

const JSCProperty::Path& JSCProperty::path() const {
  return m_path;
}

JSCPropertyType JSCProperty::type() const {
  return m_type;
}

bool JSCProperty::optional() const {
  return m_optional;
}