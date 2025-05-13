#pragma once
#include "lexer.h"
#include <optional>
#include <regex>
#include <span>
#include <string>

namespace MaiaLang
{
class Expression;
struct MemoryAllocation;
class Scope;

class Interpreter
{
  public:
    void execute(const std::string &fileName, const std::string &codeData);

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

    void handleVariableDeclaration(StateMachine &firstState, std::vector<StateMachine> &stateMachines,
                                   const Lexer::Token &token);
};
} // namespace MaiaLang