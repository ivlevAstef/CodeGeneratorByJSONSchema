//
//  IJSCLanguage.h
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/13/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#pragma once
#ifndef JSON_SCHEMA_CODE_LANGUAGE_INTERFACE_H__
#define JSON_SCHEMA_CODE_LANGUAGE_INTERFACE_H__

#include <map>
#include <set>
#include <string>
#include "JSCEnum.h"
#include "JSCObject.h"
#include "JSCOutput.h"

class IJSCLanguage {
 public:
  typedef std::set<std::string> IgnoreList;
  typedef std::map<std::string, std::string> RenameMap;

 public:
  virtual void setPrefix(std::string prefix) = 0;

  virtual void setIgnoreList(const IgnoreList& ignoreList) = 0;
  virtual void setRenameMap(const RenameMap& renameMap) = 0;

  virtual void add(const JSCEnumPointer& enumObj) = 0;
  virtual void add(const JSCObjectPointer& object) = 0;

  virtual const std::vector<JSCOutput>& outputs() const = 0;
};

#endif /* JSON_SCHEMA_CODE_LANGUAGE_INTERFACE_H__ */
