
#include <fstream>
#include <streambuf>
#include "JSCStream.h"

JSCStream::JSCStream(const char* filePath) {
  readFile(filePath);
}

size_t JSCStream::length() const {
  return m_fileData.length();
}

const char& JSCStream::operator[](size_t index) const {
  static const char invalid = '\0';

  if (index < m_fileData.length()) {
    return m_fileData[index];
  }

  //TODO: add error
  return invalid;
}

void JSCStream::readFile(const char* filePath) {
  if (nullptr == filePath) {
    //TODO: add error
    return;
  }

  std::ifstream input(filePath);

  if (!input.is_open()) {
    //TODO: add error
    return;
  }

  m_fileData = std::string(std::istreambuf_iterator<char>(input),
                           std::istreambuf_iterator<char>());
}