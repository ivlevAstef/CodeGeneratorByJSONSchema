//
//  main.cpp
//  CodeGeneratorByJSONScheme
//
//  Created by Alexander.Ivlev on 1/13/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#include "JSCParser.h"
#include "JSCStream.h"
#include "JSCTokens.h"

int main(int argc, const char* argv[]) {
  JSCStream stream("test-schema.json");
  JSCTokens tokens(stream);

  JSCParser parser(tokens);

  return 0;
}