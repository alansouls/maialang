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
	Scope(const std::string& name, const std::shared_ptr<Scope>& parentScope);
	~Scope();

	static auto global() -> std::weak_ptr<Scope>;

	auto declareVariable(const std::string& name,
		const TypeInfo& typeInfo) -> std::weak_ptr<Variable>;

	auto getVariable(const std::string& name) -> std::weak_ptr<Variable>;

	auto getFunction(const std::string& name) -> std::weak_ptr<Function>;

	auto declareFunction(const std::string& name,
		const std::vector<FunctionParameter> parameters,
		const std::optional<TypeInfo>& returnType,
		const std::vector<Expression> expressions) -> std::weak_ptr<Function>;

	void printInfo();
private:
	std::string m_name;
	std::optional<std::shared_ptr<Scope>> m_parentScope;
	std::map<std::string, std::shared_ptr<Variable>> m_variables;
	std::map<std::string, std::shared_ptr<Function>> m_functions;

	static std::shared_ptr<Scope> m_global;
};
}