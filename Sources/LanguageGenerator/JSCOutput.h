//
//  JSCOutput.h
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/13/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#pragma once
#ifndef JSON_SCHEMA_CODE_OUTPUT_H__
#define JSON_SCHEMA_CODE_OUTPUT_H__

class JSCOutput {
 public:
  JSCOutput(std::string file, std::string text);

  void save() const;
};

#endif /* JSON_SCHEMA_CODE_OUTPUT_H__ */
