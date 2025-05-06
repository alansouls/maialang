#pragma once
#include <string>
#include <vector>
#include <memory>
#include "function.h"
#include <map>

namespace MaiaLang
{
class Variable;
class Scope {
public:
	Scope(const std::string& name);
	~Scope();

	static auto global() -> Scope&;

	auto declareVariable(const std::string& name,
		const TypeInfo& typeInfo) -> std::weak_ptr<Variable>;

	auto getVariable(const std::string& name) -> std::weak_ptr<Variable>;

	auto declareFunction(const std::string& name,
		const std::vector<FunctionParameter> parameters,
		const std::optional<TypeInfo>& returnType,
		const std::vector<Expression> expressions) -> std::weak_ptr<Function>;

	void printInfo();
private:
	std::string m_name;

	static Scope m_global;

	std::map<std::string, std::shared_ptr<Variable>> m_variables;
	std::map<std::string, std::shared_ptr<Function>> m_functions;
};
}