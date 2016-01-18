//
//  Config.cpp
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/18/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#include "Config.h"

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
}

void Config::Load(std::string filename) {
}


//std::string result;
//result += "//\n";
//result += "//  " + {FILE_NAME} + "\n";
//result += "//  PROJECT_NAME\n";
//result += "//\n";
//result += "//  Created by Alexander.Ivlev on {DATE}.\n";
//result += "//  Copyright © 2016 themsteam. All rights reserved.\n";
//result += "//\n\n";

//IgnoreList{""}
//RenameMap{{"id", "objId"}}