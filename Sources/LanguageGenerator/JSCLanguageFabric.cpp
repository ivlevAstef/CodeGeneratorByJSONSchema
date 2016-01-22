//
//  JSCLanguageFabric.cpp
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/22/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#include "JSCLanguageFabric.h"

#include "JSCObjcJSONModelLanguage.h"
#include "JSCObjcRealmLanguage.h"

std::unique_ptr<JSCLanguageBase> JSCLanguageFabric::getLanguage(std::string language, std::string lib) {
  if ("objc" == language) {
    if ("Realm" == lib) {
      return std::unique_ptr<JSCLanguageBase>(new JSCObjcRealmLanguage());
    }
    if ("JSONModel" == lib) {
      return std::unique_ptr<JSCLanguageBase>(new JSCObjcJSONModelLanguage());
    }
  }

  return std::unique_ptr<JSCLanguageBase>(nullptr);
}