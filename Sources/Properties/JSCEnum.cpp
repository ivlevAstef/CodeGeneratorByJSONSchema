
#include "JSCEnum.h"

JSCEnum::JSCEnum(const std::vector<std::string>& identifiers) : JSCProperty(JSCProperty_Enum) {
  m_identifiers = identifiers;
}

const std::vector<std::string>& JSCEnum::identifiers() const {
  return m_identifiers;
}

std::string JSCEnum::enumName() const {
  std::string result;
  for (std::string subpath : path()) {
    if (!subpath.empty()) {
      subpath[0] = toupper(subpath[0]);
      result += subpath;
    }
  }

  return result;
}