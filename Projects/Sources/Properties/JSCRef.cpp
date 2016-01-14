
#include "JSCRef.h"

JSCRef::JSCRef(const std::string& refPath) : JSCProperty(JSCProperty_Ref) {
  m_refPath = refPath;
}

const std::string& JSCRef::refPath() const {
  return m_refPath;
}
