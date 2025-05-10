#include "interpreter.h"
#include "expression.h"
#include "scope.h"
#include "string_utils.h"
#include <format>
#include <regex>

namespace MaiaLang {
static const char *functionCallPattern = R"((\w+)\((.*)\))";

static const std::regex functionCallRegex(functionCallPattern,
                                          std::regex_constants::icase);

static const char *variableDeclarationPattern =
    R"(let[\s]+(\w+)\s+(\w+)(\s*|\s*=(.+)))";

static const std::regex
    variableDeclarationExpressionRegex(variableDeclarationPattern,
                                       std::regex_constants::icase);

static const char *constValuePattern = R"(".*"|\d+)";

static const std::regex constValueRegex(constValuePattern);

static const std::string valueExpressionPattern(
    std::format("({})|({})", constValuePattern, functionCallPattern));

static const std::regex valueExpressionRegex(valueExpressionPattern,
                                             std::regex_constants::icase);

static const std::string
    functionParametersPattern(std::format("({}),?", valueExpressionPattern));

static const std::regex functionParameteresRegex(functionParametersPattern,
                                                 std::regex_constants::icase);

static const std::string expressionPattern(
    std::format("(({})|({})|({}))\\s*;", variableDeclarationPattern,
                constValuePattern, functionCallPattern));

static const std::regex expressionRegex(expressionPattern,
                                        std::regex_constants::icase);

void Interpreter::execute(const std::string &fileName,
                          const std::string &codeData) {
  Scope fileScope(fileName, Scope::global().lock());

  auto expressionMatchBegin =
      std::sregex_iterator(codeData.begin(), codeData.end(), expressionRegex);
  auto expressionMatchEnd = std::sregex_iterator();

  for (std::sregex_iterator i = expressionMatchBegin; i != expressionMatchEnd;
       ++i) {
    std::smatch match = *i;
    processExpression(fileScope, match[1]);
  }

  GC::printInfo();
}

auto Interpreter::parseVariableDeclarationExpression(
    const std::smatch &variableDeclarationMatch)
    -> std::unique_ptr<Expression> {
  auto typeName = variableDeclarationMatch[1].str();
  auto varName = variableDeclarationMatch[2].str();

  VariableDeclarationParameters declarationParameters{
      .typeName = typeName, .variableName = varName, .assignment = {}};

  if (variableDeclarationMatch[4].matched) {
    auto valueExpression = StringUtils::trim(variableDeclarationMatch[4].str());
    std::smatch match;
    if (!std::regex_match(valueExpression, match, valueExpressionRegex)) {
      throw std::runtime_error("Invalid expression in variable assignment");
    }

    declarationParameters.assignment = VariableAssignmentParameters{
        .variableName = varName, .expression = parseExpression(match.str())};
  }

  return std::make_unique<Expression>(
      ExpressionParameters(declarationParameters));
}

auto Interpreter::parseConstValueExpression(
    const std::smatch &constValueExpression) -> std::unique_ptr<Expression> {
  return std::make_unique<Expression>(
      ExpressionParameters(constValueExpression.str()));
}

auto Interpreter::processExpression(Scope &scope,
                                    const std::string &expressionString)
    -> std::optional<MemoryAllocation> {
  auto expression = parseExpression(expressionString);

  auto result = expression->evaluate(scope);

  if (result.has_value()) {
    return result.value().memoryValue;
  }

  return std::optional<MemoryAllocation>();
}

auto Interpreter::parseExpression(const std::string &expressionString)
    -> std::unique_ptr<Expression> {
  std::smatch subMatch;

  if (std::regex_match(expressionString, subMatch,
                       variableDeclarationExpressionRegex)) {
    return parseVariableDeclarationExpression(subMatch);
  }

  if (std::regex_match(expressionString, subMatch, constValueRegex)) {
    return parseConstValueExpression(subMatch);
  }

  if (std::regex_match(expressionString, subMatch, functionCallRegex)) {
    return parseFunctionCallExpression(subMatch);
  }

  throw std::runtime_error("Could not parse expression");
}

auto Interpreter::parseFunctionCallExpression(
    const std::smatch &functionCallMatch) -> std::unique_ptr<Expression> {
  auto functionName = functionCallMatch[1].str();

  auto parameters = functionCallMatch[2].str();

  std::vector<std::shared_ptr<Expression>> parameterExpressions;

  auto expressionMatchBegin = std::sregex_iterator(
      parameters.begin(), parameters.end(), expressionRegex);
  auto expressionMatchEnd = std::sregex_iterator();

  for (std::sregex_iterator i = expressionMatchBegin; i != expressionMatchEnd;
       ++i) {
    std::smatch match = *i;
    parameterExpressions.push_back(parseExpression(match[1]));
  }

  return std::make_unique<Expression>(
      FunctionCallParameters{.functionName = functionName,
                             .parameterExpressions = parameterExpressions});
}
} // namespace MaiaLang
