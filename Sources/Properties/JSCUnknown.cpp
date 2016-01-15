
#include "JSCUnknown.h"

JSCUnknown::JSCUnknown(const std::vector<JSCPropertyPointer>& properties) : JSCProperty(JSCProperty_Unknown) {
  m_properties = properties;
}

const std::vector<JSCPropertyPointer>& JSCUnknown::properties() const {
  return m_properties;
}