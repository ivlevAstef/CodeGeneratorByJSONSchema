
#include "JSCObject.h"

JSCObject::JSCObject() : JSCProperty(JSCProperty_Object) {
}

bool JSCObject::addProperty(const JSCPropertyPointer property) {
  m_properties.push_back(property);
  return true;
}

const std::vector<JSCPropertyPointer>& JSCObject::properties() const {
  return m_properties;
}