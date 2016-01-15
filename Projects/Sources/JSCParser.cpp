
#include "JSCArray.h"
#include "JSCEnum.h"
#include "JSCMultyType.h"
#include "JSCObject.h"
#include "JSCParser.h"
#include "JSCProperty.h"
#include "JSCRef.h"
#include "JSCUnknown.h"
#include "SIALogger.h"

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

  if (tokensBuffer.size() > 0 && "," == tokensBuffer.back()) {
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

JSCPropertyPointer JSCParser::createProperty(const std::vector<JSCToken>& tokens, const Path& path, const std::vector<JSCPropertyPointer>& children) const {
  JSCPropertyPointer result(nullptr);

  std::set<JSCPropertyType> types = findAndParseTypes(tokens);
  bool hasNull = types.erase(JSCProperty_Null) > 0;

  if (!types.empty()) {
    if (types.size() > 1) /*many types*/ {
      result = JSCPropertyPointer(new JSCMultyType(types));
    } else {
      result = createPropertyByType(*types.begin(), tokens, path, children);
    }
  }

  if (nullptr == result) {
    result = analyzeUnknownProperty(tokens, path, children);
  }

  SIAAssert(nullptr != result.get());

  result->setPath(path);
  if (hasNull) {
    result->setOptional(true);
  }

  result->setTitle(getPropertyByName("title", tokens));
  result->setDescription(getPropertyByName("description", tokens));

  return result;
}

JSCPropertyPointer JSCParser::createPropertyByType(JSCPropertyType type, const std::vector<JSCToken>& tokens, const Path& path, const std::vector<JSCPropertyPointer>& children) const {
  switch (type) {
  case JSCProperty_Boolean:
    return createBooleanProperty(tokens, path, children);
  case JSCProperty_Integer:
    return createIntegerProperty(tokens, path, children);
  case JSCProperty_Number:
    return createNumberProperty(tokens, path, children);
  case JSCProperty_String:
    return createStringProperty(tokens, path, children);
  case JSCProperty_Object:
    return createObjectProperty(tokens, path, children);
  case JSCProperty_Array:
    return createArrayProperty(tokens, path, children);
  case JSCProperty_Any:
    return createAnyProperty(tokens, path, children);

  case JSCProperty_Unknown:
  case JSCProperty_Enum:
  case JSCProperty_Null:
  case JSCProperty_MultyType:
  case JSCProperty_Ref:
    SIAWarning("Incorrect property type.");
    break;
  }

  return JSCPropertyPointer(nullptr);
}

JSCPropertyPointer JSCParser::createBooleanProperty(const std::vector<JSCToken>& tokens, const Path& path, const std::vector<JSCPropertyPointer>& children) const {
  if (children.size() > 0) {
    SIAWarning("for boolean type find childrens.");
  }
  return JSCPropertyPointer(new JSCProperty(JSCProperty_Boolean));
}

JSCPropertyPointer JSCParser::createIntegerProperty(const std::vector<JSCToken>& tokens, const Path& path, const std::vector<JSCPropertyPointer>& children) const {
  if (children.size() > 0) {
    SIAWarning("for integer type find childrens.");
  }
  return JSCPropertyPointer(new JSCProperty(JSCProperty_Integer));
}

JSCPropertyPointer JSCParser::createNumberProperty(const std::vector<JSCToken>& tokens, const Path& path, const std::vector<JSCPropertyPointer>& children) const {
  if (children.size() > 0) {
    SIAWarning("for number type find childrens.");
  }
  return JSCPropertyPointer(new JSCProperty(JSCProperty_Number));
}

JSCPropertyPointer JSCParser::createStringProperty(const std::vector<JSCToken>& tokens, const Path& path, const std::vector<JSCPropertyPointer>& children) const {
  std::vector<JSCToken> enumIdentfiers = findAndParseSimpleOrArray("enum", tokens);

  if (enumIdentfiers.empty()) {
    return JSCPropertyPointer(new JSCProperty(JSCProperty_String));
  }

  return JSCPropertyPointer(new JSCEnum(enumIdentfiers));
}

JSCPropertyPointer JSCParser::createObjectProperty(const std::vector<JSCToken>& tokens, const Path& path, const std::vector<JSCPropertyPointer>& children) const {
  ///find child with contains properties object
  JSCUnknown* objProperties = nullptr;

  for (const JSCPropertyPointer& child : children) {
    if ("properties" == child->name() && JSCProperty_Unknown == child->type()) {
      SIAAssertMsg(nullptr == objProperties, "Double find properties in object.");
      objProperties = (JSCUnknown*)child.get();
    }
  }

  if (nullptr == objProperties) {
    SIAWarning("Can't find object properties.");
    return JSCPropertyPointer(new JSCObject(children));
  }

  const std::vector<JSCPropertyPointer>& properties = objProperties->properties();

  std::vector<JSCToken> required = findAndParseSimpleOrArray("required", tokens);

  ///removed optional for required names
  for (const JSCToken& requiredName : required) {
    for (const JSCPropertyPointer& property : properties) {
      if (property->name() == requiredName) {
        property->setOptional(false);
      }
    }
  }

  return JSCPropertyPointer(new JSCObject(properties));
}

JSCPropertyPointer JSCParser::createArrayProperty(const std::vector<JSCToken>& tokens, const Path& path, const std::vector<JSCPropertyPointer>& children) const {
  JSCPropertyPointer itemType(nullptr);

  for (const JSCPropertyPointer& child : children) {
    if ("item" == child->name() || "items" == child->name()) {
      SIAAssertMsg(nullptr == itemType.get(), "Double find items in array.");
      itemType = child;
    }
  }

  if (nullptr == itemType.get()) {
    SIAWarning("Can't find item type for array.");
  }

  return JSCPropertyPointer(new JSCArray(itemType));
}

JSCPropertyPointer JSCParser::createAnyProperty(const std::vector<JSCToken>& tokens, const Path& path, const std::vector<JSCPropertyPointer>& children) const {
  if (children.size() > 0) {
    SIAWarning("for any type find childrens.");
  }
  return JSCPropertyPointer(new JSCProperty(JSCProperty_Any));
}

JSCPropertyPointer JSCParser::analyzeUnknownProperty(const std::vector<JSCToken>& tokens, const Path& path, const std::vector<JSCPropertyPointer>& children) const {
  std::vector<JSCToken> ref = findAndParseSimpleOrArray("$ref", tokens);

  if (1 == ref.size()) {
    return JSCPropertyPointer(new JSCRef(ref[0]));
  }

  return JSCPropertyPointer(new JSCUnknown(children));
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
  } else if ("any" == token) {
    return JSCProperty_Any;
  }

  return JSCProperty_Unknown;
}

std::set<JSCPropertyType> JSCParser::findAndParseTypes(const std::vector<JSCToken>& tokens) const {
  std::set<JSCPropertyType> result;

  for (const JSCToken& token : findAndParseSimpleOrArray("type", tokens)) {
    result.insert(typeByToken(token));
  }

  result.erase(JSCProperty_Unknown);
  return result;
}

std::vector<JSCToken> JSCParser::findAndParseSimpleOrArray(const std::string foundToken, const std::vector<JSCToken>& tokens) const {
  std::vector<JSCToken> result;
  const size_t endIndex = tokens.size() < 2 ? 0 : tokens.size() - 2;

  for (size_t i = 0; i < endIndex; i++) {
    if (foundToken == tokens[i] && ":" == tokens[i + 1]) {
      i += 2;
      if ("[" == tokens[i]) {
        do {
          i++;
          result.push_back(tokens[i]);
          if (i + 1 < tokens.size() && "," == tokens[i + 1]) {
            i++;
          }
        } while ("]" != tokens[i] && i < tokens.size());

      } else {
        result.push_back(tokens[i]);
      }
      break;
    }
  }

  return result;
}

JSCToken JSCParser::getPropertyByName(const std::string name, const std::vector<JSCToken>& tokens) const {
  const size_t endIndex = tokens.size() < 2 ? 0 : tokens.size() - 2;

  for (size_t i = 0; i < endIndex; i++) {
    if (name == tokens[i] && ":" == tokens[i + 1] &&
        ("[" != tokens[i + 2] && "{" != tokens[i + 2])) {
      return tokens[i + 2];
    }
  }
  return "";
}
