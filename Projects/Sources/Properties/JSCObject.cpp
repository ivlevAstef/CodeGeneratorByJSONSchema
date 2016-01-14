
#include "JSCObject.h"

JSCObject::JSCObject(const std::string& name, const std::vector<JSCPropertyPointer>& properties) : JSCProperty(JSCProperty_Object) {
  m_name = name;
  m_properties = properties;
}

const std::vector<JSCPropertyPointer>& JSCObject::properties() const {
  return m_properties;
}

const std::string& JSCObject::name() const {
  return m_name;
}