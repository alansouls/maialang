#include "interpreter.h"
#include "expression.h"
#include "lexer.h"
#include "scope.h"

namespace MaiaLang
{
void Interpreter::execute(const std::string &fileName, const std::string &codeData)
{
    Scope fileScope(fileName, Scope::global().lock());

    Lexer lexer(codeData);

    std::vector<StateMachine> stateMachines;

    for (auto &token : lexer.tokens())
    {
        if (stateMachines.empty())
        {
            if (token.type == Lexer::TokenType::Let)
            {
                stateMachines.push_back({.state = StateMachine::State::VariableDeclaration,
                                         .typeName = std::optional<std::string>(),
                                         .symbolName = std::optional<std::string>()});
            }
            else if (token.type == Lexer::TokenType::Identitifer)
            {
                stateMachines.push_back({.state = StateMachine::State::Unknown,
                                         .typeName = std::optional<std::string>(),
                                         .symbolName = std::optional<std::string>()});
            }
            else if (token.type == Lexer::TokenType::SemiCollon)
            {
                continue;
            }
            else
            {
                throw std::runtime_error("Invalid syntax");
            }
        }

        auto &firstState = stateMachines[0];

        if (firstState.state == StateMachine::State::VariableDeclaration)
        {
            handleVariableDeclaration(firstState, stateMachines, token);
        }
    }

    GC::printInfo();
}

void Interpreter::handleVariableDeclaration(StateMachine &firstState, std::vector<StateMachine> &stateMachines,
                                            const Lexer::Token &token)
{
    if (!firstState.typeName.has_value())
    {
        if (token.type == Lexer::TokenType::Identitifer)
        {
            firstState.typeName = token.token;
            return;
        }

        throw std::runtime_error("Expected type identifier");
    }
    else if (!firstState.symbolName.has_value())
    {
        if (token.type == Lexer::TokenType::Identitifer)
        {
            firstState.typeName = token.token;
            return;
        }

        throw std::runtime_error("Expected variable name");
    }

    if (stateMachines.size() > 1)
    {
        auto &secondState = stateMachines[1];
        return;
    }
    else if (token.type == Lexer::TokenType::Assignment)
    {
        stateMachines.push_back({.state = StateMachine::State::VariableAssignment});
        return;
    }

    if (token.type != Lexer::TokenType::SemiCollon)
    {
        throw std::runtime_error("Invaid token, expected semicollon");
    }
}
} // namespace MaiaLang
