#pragma once
#include <string>
#include <span>
#include <regex>
#include <optional>

namespace MaiaLang
{
	class Expression;
	struct MemoryAllocation;
	class Scope;
	class Interpreter
	{
	public:
		void execute(const std::string& fileName, const std::string& codeData);
	private:
		static auto parseVariableDeclarationExpression(const std::smatch& variableDeclarationMatch) -> std::unique_ptr<Expression>;
		static auto parseConstValueExpression(const std::smatch& constValueExpression) -> std::unique_ptr<Expression>;
		static auto processExpression(Scope& scope, const std::string& expressionString) -> std::optional<MemoryAllocation>;
		static auto parseExpression(const std::string& expressionString) -> std::unique_ptr<Expression>;
		static auto parseFunctionCallExpression(const std::smatch& functionCallMatch) -> std::unique_ptr<Expression>;
	};
}