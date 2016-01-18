//
//  Config.cpp
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/18/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#include "Config.h"
#include "SIALogger.h"

#include <fstream>
#include <sstream>

Config Config::config;

Config::Config() {
  m_tab = "  ";
  m_prefix = "JSC";

  m_licenceHeader = "//\n";
  m_licenceHeader += "//  {FILE_NAME} \n";
  m_licenceHeader += "//  PROJECT_NAME\n";
  m_licenceHeader += "//\n";
  m_licenceHeader += "//  Created by AUTHOR at {DATE}.\n";
  m_licenceHeader += "//  Description.\n";
  m_licenceHeader += "//\n\n";

  m_ignoreList = {""};
  m_renameMap = {{"id", "objId"}};

  AdditionalClass classCooridinate = {
      "DtoCoordinate",
      "coordinate",
      true,
      {{"number", "latitude", false}, {"number", "longitude", false}}};

  AdditionalClass classDate = {
      "DtoTimestamp",
      "timestamp",
      true,
      {{"string", "date_ins", true}, {"string", "date_upd", true}, {"bool", "deleted", true}}};

  m_additionalClasses = {classCooridinate, classDate};
}

static std::string trim(std::string str) {
  size_t first = str.find_first_not_of(' ');
  size_t last = str.find_last_not_of(' ');
  return str.substr(first, (last - first + 1));
}

static std::string parseTabValue(std::istringstream& stream) {
  std::string temp;
  std::getline(stream, temp, '\'');

  std::string result;
  std::getline(stream, result, '\'');

  return result;
}

static std::string parsePrefixValue(std::istringstream& stream) {
  std::string result;
  std::getline(stream, result);

  return trim(result);
}

void Config::Load(std::string filename) {
  std::fstream fileStream(filename);

  if (!fileStream.is_open()) {
    SIAFatal("Can't find config file: %s", filename.c_str());
  }

  std::string line;
  while (std::getline(fileStream, line, '=')) {
    std::istringstream lineStream(line);

    std::string key;
    if (std::getline(lineStream, key, '=')) {
      key = trim(key);
      if ("Tab" == key) {
        config.m_tab = parseTabValue(lineStream);
      } else if ("Prefix" == key) {
        config.m_prefix = parsePrefixValue(lineStream);
      } else if ("Licence Header" == key) {
      } else if ("Ignore List" == key) {
      } else if ("Rename Map" == key) {
      } else {
      }
    }
  }
}
