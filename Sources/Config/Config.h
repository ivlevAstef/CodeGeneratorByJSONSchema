//
//  Config.h
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/18/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#pragma once
#ifndef JSON_SCHEMA_CODE_CONFIG_H__
#define JSON_SCHEMA_CODE_CONFIG_H__

#include "JSCTypes.h"

class Config {
 private:
  static Config config;

  Config();

 public:
  static void Load(std::string filename);

  static const std::string& tab() { return config.m_tab; }
  static const std::string& prefix() { return config.m_prefix; }

  static const std::string& licenceHeader() { return config.m_licenceHeader; }

  static const IgnoreList& ignoreList() { return config.m_ignoreList; }
  static const RenameMap& renameMap() { return config.m_renameMap; }

 private:
  std::string m_tab;
  std::string m_prefix;
  std::string m_licenceHeader;

  IgnoreList m_ignoreList;
  RenameMap m_renameMap;
};


#endif /* JSON_SCHEMA_CODE_CONFIG_H__ */
