//
//  JSCOutput.cpp
//  CodeGeneratorByJSONSchema
//
//  Created by Alexander.Ivlev on 1/18/16.
//  Copyright © 2016 themsteam. All rights reserved.
//

#include "JSCOutput.h"
#include "SIALogger.h"

JSCOutput::JSCOutput(std::string file, std::string text) {
  m_fileName = file;
  m_text = text;
}

void JSCOutput::save() const {
  SIAInfo("FILENAME: %s", m_fileName.c_str());
  SIAInfo("%s\n\n", m_text.c_str());
}