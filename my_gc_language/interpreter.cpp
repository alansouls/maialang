#include "interpreter.h"
#include "expression.h"
#include "scope.h"
#include "string_utils.h"
#include "variable.h"
#include <regex>
#include <format>

namespace MaiaLang
{
	static const char* variableDeclarationPattern = R"(let[\s]+(\w+)\s+(\w+)(\s*|\s*=(.+));)";

	static const std::regex variableDeclarationExpressionRegex(variableDeclarationPattern, std::regex_constants::icase);

	static const char* constValuePattern = R"(".*"|\d+)";

	static const std::regex constValueRegex(constValuePattern);

	static const std::string expressionPattern(std::format("({})|({})", variableDeclarationPattern, constValuePattern));

	static const std::regex expressionRegex(expressionPattern, std::regex_constants::icase);

	void Interpreter::execute(const std::string& fileName, const std::string& codeData)
	{
		Scope fileScope(fileName);

		auto expressionMatchBegin = std::sregex_iterator(codeData.begin(), codeData.end(), expressionRegex);
		auto expressionMatchEnd = std::sregex_iterator();

		for (std::sregex_iterator i = expressionMatchBegin; i != expressionMatchEnd; ++i) {
			std::smatch match = *i;
			processExpression(fileScope, match);
		}

		Function::BuiltIn::println().lock()->execute({ fileScope.getVariable("c").lock()->allocation() });
		
		GC::printInfo();
	}

	void Interpreter::processVariableDeclarationExpression(Scope& scope, const std::smatch& variableDeclarationMatch)
	{
		auto typeName = variableDeclarationMatch[2].str();
		auto varName = variableDeclarationMatch[3].str();

		TypeInfo type;
		if (typeName == "int") {
			type = TypeInfo::BuiltIn::integer();
		}
		else if (typeName == "string") {
			type = TypeInfo::BuiltIn::string();
		}
		else {
			throw std::runtime_error("Unknown type: " + typeName);
		}

		auto var = scope.declareVariable(varName, type);

		if (variableDeclarationMatch[5].matched) {
			auto valueExpression = StringUtils::trim(variableDeclarationMatch[5].str());
			std::smatch match;
			if (!std::regex_match(valueExpression, match, expressionRegex)) {
				throw std::runtime_error("Invalid expression in variable assignment");
			}
			auto expressionValue = processExpression(scope, match);
			if (!expressionValue.has_value()) {
				throw std::runtime_error("Expression to assign to variable resulted in no value");
			}

			var.lock()->assign(expressionValue.value());
		}
	}

	auto Interpreter::processConstValueExpression(Scope& scope, const std::smatch& constValueExpression) -> MemoryAllocation
	{
		auto expressionString = constValueExpression.str();

		Expression exp = ExpressionParameters(expressionString);

		return exp.evaluate().value().memoryValue;
	}

	auto Interpreter::processExpression(Scope& scope, const std::smatch& expressionMatch) -> std::optional<MemoryAllocation>
	{
		auto expressionString = expressionMatch.str();

		if (expressionString.starts_with("let "))
		{
			processVariableDeclarationExpression(scope, expressionMatch);
			return std::optional<MemoryAllocation>();
		}

		if (std::smatch constValueExpressionMatch; std::regex_match(expressionString, constValueExpressionMatch, constValueRegex)) {
			return std::optional<MemoryAllocation>(processConstValueExpression(scope, expressionMatch));
		}

		throw std::runtime_error("Could not process expression");
	}
}