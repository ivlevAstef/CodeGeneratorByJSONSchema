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
  static bool Load(std::string filename);

  static const std::string& languageName() { return config.m_languageName; }
  static const std::string& languageLib() { return config.m_languageLib; }

  static const std::string& tab() { return config.m_tab; }
  static const std::string& prefix() { return config.m_prefix; }
  static const std::string& inputFile() { return config.m_inputFile; }

  static const std::string& outputDirectory() { return config.m_outputDirectory; }

  static const std::string& licenceHeader() { return config.m_licenceHeader; }

  static const IgnoreList& ignoreList() { return config.m_ignoreList; }
  static const RenameMap& renameMap() { return config.m_renameMap; }

  static const std::vector<LeafClass>& leafClasses() { return config.m_leafClasses; }
  static const std::vector<AdditionalClass>& additionalClasses() { return config.m_additionalClasses; }

 private:
  static bool privateLoad(std::string filename);

 private:
  std::string m_languageName;
  std::string m_languageLib;

  std::string m_inputFile;
  std::string m_tab;
  std::string m_prefix;
  std::string m_outputDirectory;
  std::string m_licenceHeader;

  IgnoreList m_ignoreList;
  std::vector<LeafClass> m_leafClasses;
  RenameMap m_renameMap;

  std::vector<AdditionalClass> m_additionalClasses;
};


#endif /* JSON_SCHEMA_CODE_CONFIG_H__ */
