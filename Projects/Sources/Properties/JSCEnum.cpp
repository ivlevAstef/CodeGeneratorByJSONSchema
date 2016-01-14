
#include "JSCEnum.h"

JSCEnum::JSCEnum(std::vector<std::string> identifiers) : JSCProperty(JSCProperty_Enum) {
  m_identifiers = identifiers;
}

const std::vector<std::string>& JSCEnum::identifiers() const {
  return m_identifiers;
}