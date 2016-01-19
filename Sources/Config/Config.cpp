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
  m_prefix = "";
  m_outputDirectory = "result/";

  m_licenceHeader = "";
  m_ignoreList = {""};
  m_renameMap = {};
  m_additionalClasses = {};
}

static std::string trim(std::string str) {
  size_t first = str.find_first_not_of(' ');
  size_t last = str.find_last_not_of(' ');
  if (first == std::string::npos) {
    return "";
  }

  return str.substr(first, (last - first + 1));
}

static std::string getValueBetweenQuote(std::istringstream& stream) {
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

static std::string parseInputFile(std::istringstream& stream) {
  std::string result;
  std::getline(stream, result);

  return trim(result);
}

static std::string parseOutputDirectory(std::istringstream& stream) {
  std::string result;
  std::getline(stream, result);

  return trim(result);
}

static std::string parseLicenceHeader(std::istringstream& inputLineStream, std::ifstream& fileStream) {
  std::string result;
  std::string temp;

  std::string line;
  std::getline(inputLineStream, line);

  bool started = false;

  do {
    std::istringstream lineStream(line);

    while (!std::getline(lineStream, temp, '"').eof()) {
      if (started) {
        result += temp;
        return result;
      }
      started = !started;
    }

    if (started) {
      result += temp + '\n';
    }

  } while (std::getline(fileStream, line));

  return result;
}

static IgnoreList parseIgnoreList(std::istringstream& lineStream) {
  IgnoreList result;
  std::string temp;

  while (std::getline(lineStream, temp, ',')) {
    result.insert(trim(temp));
  }

  return result;
}

static RenameMap parseRenameMap(std::istringstream& inputLineStream, std::ifstream& fileStream) {
  RenameMap result;
  std::string temp;

  std::string line;
  std::getline(inputLineStream, line);

  do {
    std::istringstream lineStream(line);

    std::string key;
    if (!std::getline(lineStream, key, ':').eof()) {
      std::string value;
      std::getline(lineStream, value);
      result[trim(key)] = trim(value);
    } else if ("End" == line) {
      break;
    }

  } while (std::getline(fileStream, line));

  return result;
}

static bool parseBool(std::istringstream& stream) {
  std::string temp;
  std::getline(stream, temp, ',');
  temp = trim(temp);
  if (temp == "true" || temp == "t" || temp == "T") {
    return true;
  }
  return false;
}

static LeafClass parseLeafClass(std::istringstream& lineStream) {
  LeafClass result;

  std::getline(lineStream, result.name, ':');
  result.name = trim(result.name);

  std::string temp;
  while (std::getline(lineStream, temp, ',')) {
    result.children.push_back(trim(temp));
  }

  return result;
}

static AdditionalClass parseAdditionalClass(std::istringstream& inputLineStream, std::ifstream& fileStream) {
  AdditionalClass result;

  std::getline(inputLineStream, result.className, ',');
  std::getline(inputLineStream, result.name, ',');
  result.className = trim(result.className);
  result.name = trim(result.name);
  result.optional = parseBool(inputLineStream);

  std::string line;
  while (std::getline(fileStream, line)) {
    std::istringstream lineStream(line);

    if ("End" == line) {
      break;
    } else {
      std::string type;
      std::string name;
      bool optional = false;

      std::getline(lineStream, type, ',');
      std::getline(lineStream, name, ',');
      optional = parseBool(lineStream);

      result.properties.push_back({trim(type), trim(name), optional});
    }
  };

  return result;
}

bool Config::Load(std::string filename) {
  std::ifstream fileStream(filename);

  if (!fileStream.is_open()) {
    SIAWarning("Can't find config file: %s", filename.c_str());
    return false;
  }

  config.m_additionalClasses.clear();
  config.m_leafClasses.clear();

  std::string line;
  while (std::getline(fileStream, line)) {
    if ('#' == line[0]) {  //comment
      continue;
    }

    std::istringstream lineStream(line);

    std::string key;
    if (!std::getline(lineStream, key, '=').eof()) {
      key = trim(key);
      if ("Tab" == key) {
        config.m_tab = getValueBetweenQuote(lineStream);
      } else if ("Input file" == key) {
        config.m_inputFile = parseInputFile(lineStream);
      } else if ("Prefix" == key) {
        config.m_prefix = parsePrefixValue(lineStream);
      } else if ("Output Directory" == key) {
        config.m_outputDirectory = parseOutputDirectory(lineStream);
      } else if ("Licence Header" == key) {
        config.m_licenceHeader = parseLicenceHeader(lineStream, fileStream);
      } else if ("Ignore List" == key) {
        config.m_ignoreList = parseIgnoreList(lineStream);
      } else if ("Leaf Class" == key) {
        config.m_leafClasses.push_back(parseLeafClass(lineStream));
      } else if ("Rename Map" == key) {
        config.m_renameMap = parseRenameMap(lineStream, fileStream);
      } else if ("Additional Class" == key) {
        config.m_additionalClasses.push_back(parseAdditionalClass(lineStream, fileStream));
      }
    }
  }

  return true;
}
