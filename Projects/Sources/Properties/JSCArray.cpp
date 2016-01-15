
#include "JSCArray.h"

JSCArray::JSCArray(JSCPropertyPointer propertyType) : JSCProperty(JSCProperty_Array) {
  m_propertyType = propertyType;
}

JSCPropertyPointer JSCArray::propertyType() const {
  return m_propertyType;
}