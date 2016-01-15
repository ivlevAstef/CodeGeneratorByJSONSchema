
#include <sstream>
#include "JSCRef.h"

JSCRef::JSCRef(const std::string& refPath) : JSCProperty(JSCProperty_Ref) {
  m_refPath = refPath;
}

void JSCRef::setRefProperty(JSCPropertyPointer refProperty) {
  m_refProperty = refProperty;
}


const std::string& JSCRef::refPath() const {
  return m_refPath;
}

std::vector<std::string> JSCRef::separatedRefPath() const {
  std::stringstream stream(m_refPath);

  std::vector<std::string> result;

  std::string segment;
  while (std::getline(stream, segment, '/')) {
    result.push_back(segment);
  }

  return result;
}

JSCPropertyPointer JSCRef::refProperty() const {
  return m_refProperty;
}