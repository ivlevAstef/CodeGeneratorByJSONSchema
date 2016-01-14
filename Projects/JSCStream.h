//
//  JSCStream.h
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/13/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#pragma once
#ifndef JSON_SCHEMA_CODE_STREAM_H__
#define JSON_SCHEMA_CODE_STREAM_H__

#include <string>

class JSCStream {
 public:
  JSCStream(const char* filePath);

  size_t length() const;
  const char& operator[](size_t index) const;

 private:
  void readFile(const char* filePath);

 private:
  std::string m_fileData;
};

#endif /* JSON_SCHEMA_CODE_STREAM_H__ */
