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

		struct StateMachine
		{
			enum State
			{
				VariableDeclaration,
				FunctionDeclaration,
				FunctionCall,
				VariableValue,
				VariableAssignment,
				Unknown
			};
			
			State state;
			std::optional<std::string> typeName;
			std::optional<std::string> symbolName;
		};
	};
}