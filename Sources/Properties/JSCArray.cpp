
#include "JSCArray.h"

JSCArray::JSCArray() : JSCProperty(JSCProperty_Array) {
}

bool JSCArray::addPropertyType(const JSCPropertyPointer property) {
  m_propertyTypes.push_back(property);
  return true;
}

const std::vector<JSCPropertyPointer>& JSCArray::propertyTypes() const {
  return m_propertyTypes;
}