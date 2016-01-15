
#include "JSCObject.h"

JSCObject::JSCObject(const std::vector<JSCPropertyPointer>& properties) : JSCProperty(JSCProperty_Object) {
  m_properties = properties;
}

const std::vector<JSCPropertyPointer>& JSCObject::properties() const {
  return m_properties;
}