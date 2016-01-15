
#include "JSCObject.h"

JSCObject::JSCObject(const std::vector<JSCPropertyPointer>& properties) : JSCProperty(JSCProperty_Object) {
  m_properties = properties;
}

void JSCObject::setRootName(const std::string& name) {
  m_rootName = name;
}

const std::vector<JSCPropertyPointer>& JSCObject::properties() const {
  return m_properties;
}

const std::string& JSCObject::rootName() const {
  return m_rootName;
}