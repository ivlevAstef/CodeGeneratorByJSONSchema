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

class JSCStream {
 public:
  JSCStream(std::string filePath);

  char operator[](size_t index) const;

 private:
};

#endif /* JSON_SCHEMA_CODE_STREAM_H__ */
