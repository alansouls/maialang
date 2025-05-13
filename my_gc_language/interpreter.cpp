#include "interpreter.h"
#include "expression.h"
#include "scope.h"
#include "string_utils.h"
#include <format>
#include <regex>
#include "lexer.h"

namespace MaiaLang
{
	void Interpreter::execute(const std::string& fileName, const std::string& codeData) {
		Scope fileScope(fileName, Scope::global().lock());

		Lexer lexer(codeData);

		std::vector<StateMachine> stateMachines;

		for (auto& token : lexer.tokens()) {
			if (stateMachines.empty()) {
				if (token.type == Lexer::TokenType::Let) {
					stateMachines.push_back({
						.state = StateMachine::State::VariableDeclaration,
						.typeName = std::optional<std::string>(),
						.symbolName = std::optional<std::string>()
					});
				}
				else if (token.type == Lexer::TokenType::Identitifer) {
					stateMachines.push_back({
						.state = StateMachine::State::Unknown,
						.typeName = std::optional<std::string>(),
						.symbolName = std::optional<std::string>()
						});
				}
				else if (token.type == Lexer::TokenType::SemiCollon) {
					continue;
				}
				else {
					throw std::runtime_error("Invalid syntax");
				}
			}

			auto& firstState = stateMachines[0];

			if (firstState.state == StateMachine::State::VariableDeclaration) {
				if (!firstState.typeName.has_value()) {
					if (token.type == Lexer::TokenType::Identitifer) {
						firstState.typeName = token.token;
					}
					else {
						throw std::runtime_error("Expected type identifier");
					}
				}
				else if (!firstState.symbolName.has_value()) {
					if (token.type == Lexer::TokenType::Identitifer) {
						firstState.typeName = token.token;
					}
					else {
						throw std::runtime_error("Expected variable name");
					}
				}
				else if (token.type == Lexer::TokenType::Assignment) {

				}
				else if (token.type != Lexer::TokenType::SemiCollon) {
					throw std::runtime_error("")
				}
			}
		}

		GC::printInfo();
	}
} // namespace MaiaLang
