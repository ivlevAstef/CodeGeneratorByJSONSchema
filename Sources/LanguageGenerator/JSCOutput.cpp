//
//  JSCOutput.cpp
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/18/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#include <fstream>
#include "JSCOutput.h"
#include "SIALogger.h"

JSCOutput::JSCOutput(std::string file, std::string text) {
  m_fileName = file;
  m_text = text;
}

const std::string& JSCOutput::fileName() const {
  return m_fileName;
}

void JSCOutput::save(const std::string& directory) const {
  SIAInfo("FILENAME: %s", m_fileName.c_str());
  SIAInfo("%s", m_text.c_str());


  std::ofstream outStream(directory + m_fileName);

  if (!outStream.is_open()) {
    SIAWarning("Can't save file: %s", m_fileName.c_str());
    return;
  }

  outStream << m_text;
}