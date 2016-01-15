
#include "JSCProperty.h"

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
  m_path = path;
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

const std::string& JSCProperty::name() const {
  static std::string sEmpty = "";

  if (m_path.size() > 0) {
    return m_path[m_path.size() - 1];
  }

  return sEmpty;
}

JSCPropertyType JSCProperty::type() const {
  return m_type;
}

bool JSCProperty::optional() const {
  return m_optional;
}