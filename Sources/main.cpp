//
//  main.cpp
//  CodeGeneratorByJSONScheme
//
//  Created by Alexander.Ivlev on 1/13/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#include "Config.h"
#include "JSCParser.h"
#include "JSCStream.h"
#include "JSCTokens.h"
#include "SIALogger.h"

#include "JSCObjcJSONModelLanguage.h"

int main(int argc, const char* argv[]) {
  //Config::Load("config.ini");
  SIA::Logger::Initialization();

  JSCStream stream("test-schema.json");
  JSCTokens tokens(stream);

  JSCParser parser(tokens);

  /*
  parser.debugPrint(parser.root());

  SIADebug("All classes:");
  for (auto& obj : parser.objects()) {
    SIADebug("  Class Name:%s", obj->rootName().c_str());
    SIADebug("  Class properties:");
    for (auto& prop : obj->properties()) {
      SIADebug("    Name:%s Type:%s Optional:%d", prop->pathName().c_str(), JSCProperty::propertyTypeToString(prop->type()).c_str(), prop->optional());
    }
  }

  SIADebug("All enums:");
  for (auto& obj : parser.enums()) {
    SIADebug("  Enum Name:%s", obj->enumName().c_str());
    SIADebug("  Enum identifiers:");
    for (auto& identifier : obj->identifiers()) {
      SIADebug("    %s", identifier.c_str());
    }
  }
  */

  JSCObjcJSONModelLanguage language;

  language.setIgnoreList(Config::ignoreList());
  language.setRenameMap(Config::renameMap());
  language.setLicenceHeader(Config::licenceHeader());
  language.setPrefix(Config::prefix());
  language.setTab(Config::tab());

  for (auto& obj : parser.enums()) {
    language.add(obj);
  }

  for (const auto& additionalClass : Config::additionalClasses()) {
    language.add(additionalClass);
  }

  for (auto& obj : parser.objects()) {
    language.add(obj);
  }

  for (const auto& out : language.outputs()) {
    out.save();
  }

  return 0;
}