
#include "JSCArray.h"
#include "JSCEnum.h"
#include "JSCObject.h"
#include "JSCParser.h"
#include "JSCProperty.h"
#include "JSCUnknown.h"

JSCParser::JSCParser(const JSCTokens& tokens) {
  parse(tokens);
}

const JSCPropertyPointer JSCParser::root() const {
  return m_root;
}

const std::vector<JSCPropertyPointer> JSCParser::allObjects() const {
  return std::vector<JSCPropertyPointer>();
}

void JSCParser::parse(const JSCTokens& tokens) {
  size_t tokenIndex = 0;
  std::vector<std::string> path;

  m_root = parse(tokens, path, tokenIndex);
}

static std::string nameByTokensBuffer(std::vector<JSCToken>& tokensBuffer) {
  if (tokensBuffer.size() < 2) {
    return "";
  }

  if (":" != tokensBuffer.back()) {
    return "";
  }

  tokensBuffer.pop_back();
  std::string result = tokensBuffer.back();
  tokensBuffer.pop_back();

  if ("," == tokensBuffer.back()) {
    tokensBuffer.pop_back();
  }

  return result;
}

JSCPropertyPointer JSCParser::parse(const JSCTokens& tokens, const Path& path, size_t& tokenIndex) {
  std::vector<JSCToken> depthTokens;
  std::vector<JSCPropertyPointer> children;

  do {
    if (isBeginBrace(tokens[tokenIndex])) {
      Path newPath = path;
      newPath.push_back(nameByTokensBuffer(depthTokens));

      tokenIndex++;
      JSCPropertyPointer child = parse(tokens, newPath, tokenIndex);
      children.push_back(child);
    }

    depthTokens.push_back(tokens[tokenIndex]);
    tokenIndex++;
  } while (tokenIndex < tokens.length() && !isEndBrace(tokens[tokenIndex]));
  tokenIndex++;

  return createProperty(depthTokens, path, children);
}

JSCPropertyPointer JSCParser::createProperty(const std::vector<JSCToken>& tokens, const Path& path, const std::vector<JSCPropertyPointer>& children) {
  JSCProperty* result = nullptr;

  std::set<JSCPropertyType> types = findAndParseTypes(tokens);

  if (!types.empty()) {
    //TODO:analysis
  }

  if (nullptr == result) {
    result = new JSCUnknown(children);
  }

  result->setPath(path);
  if (types.count(JSCProperty_Null) > 0) {
    result->setOptional(true);
  }

  return JSCPropertyPointer(result);
}

bool JSCParser::isBeginBrace(const JSCToken& token) const {
  return token == "{";
}

bool JSCParser::isEndBrace(const JSCToken& token) const {
  return token == "}";
}

JSCPropertyType JSCParser::typeByToken(const JSCToken& token) const {
  if ("null" == token) {
    return JSCProperty_Null;
  } else if ("boolean" == token || "bool" == token) {
    return JSCProperty_Boolean;
  } else if ("integer" == token) {
    return JSCProperty_Integer;
  } else if ("number" == token) {
    return JSCProperty_Number;
  } else if ("string" == token) {
    return JSCProperty_String;
  } else if ("object" == token) {
    return JSCProperty_Object;
  } else if ("array" == token) {
    return JSCProperty_Array;
  }

  return JSCProperty_Unknown;
}

std::set<JSCPropertyType> JSCParser::findAndParseTypes(const std::vector<JSCToken>& tokens) const {
  std::set<JSCPropertyType> result;

  for (size_t i = 0; i < tokens.size(); i++) {
    if ("type" == tokens[i] && ":" == tokens[i + 1]) {
      i += 2;
      if ("[" == tokens[i]) {
        do {
          i++;
          result.insert(typeByToken(tokens[i]));
          if ("," == tokens[i + 1]) {
            i++;
          }
        } while ("]" != tokens[i] && i < tokens.size());

      } else {
        result.insert(typeByToken(tokens[i]));
      }
      break;
    }
  }

  result.erase(JSCProperty_Unknown);
  return result;
}
