#pragma once
#include <string>
#include <span>
#include <regex>
#include <optional>

namespace MaiaLang
{
	struct MemoryAllocation;
	class Scope;
	class Interpreter
	{
	public:
		void execute(const std::string& fileName, const std::string& codeData);
	private:
		static void processVariableDeclarationExpression(Scope& scope, const std::smatch& variableDeclarationMatch);
		static auto processConstValueExpression(Scope& scope, const std::smatch& constValueExpression) -> MemoryAllocation;
		static auto processExpression(Scope& scope, const std::smatch& expressionMatch) -> std::optional<MemoryAllocation>;
	};
}