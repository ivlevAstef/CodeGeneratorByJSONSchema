//
//  JSCTokens.h
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/13/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#pragma once
#ifndef JSON_SCHEMA_CODE_TOKENS_H__
#define JSON_SCHEMA_CODE_TOKENS_H__

#include "JSCStream.h"

typedef std::string JSCToken;

class JSCTokens {
 public:
  JSCTokens(const JSCStream& stream);

  const JSCToken& operator[](size_t index) const;
};

#endif /* JSON_SCHEMA_CODE_TOKENS_H__ */
