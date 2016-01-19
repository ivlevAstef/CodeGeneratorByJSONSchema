//
//  JSCTypes.h
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/18/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#pragma once
#ifndef JSON_SCHEMA_CODE_TYPES_H__
#define JSON_SCHEMA_CODE_TYPES_H__

#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

typedef std::set<std::string> IgnoreList;              // names
typedef std::map<std::string, std::string> RenameMap;  // last name, new name

typedef struct AdditionalClass {
  std::string className;
  std::string name;
  bool optional;
  std::vector<std::tuple<std::string, std::string, bool>> properties;  //type, name, optional
} AdditionalClass;

typedef struct LeafClass {
  std::string name;
  std::vector<std::string> children;
} LeafClass;

#endif /* JSON_SCHEMA_CODE_TYPES_H__ */
