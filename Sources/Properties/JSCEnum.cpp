
#include "JSCEnum.h"

JSCEnum::JSCEnum(const std::vector<std::string>& identifiers) : JSCProperty(JSCProperty_Enum) {
  m_identifiers = identifiers;
}

const std::vector<std::string>& JSCEnum::identifiers() const {
  return m_identifiers;
}

std::string JSCEnum::enumName() const {
  std::string result;

  const auto lPath = path();
  if (lPath.size() > 2) {
    return lPath[1] + "_" + pathName();
  }

  return pathName();
}