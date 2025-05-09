#include "scope.h"
#include "variable.h"
#include <iostream>

namespace MaiaLang
{
	std::shared_ptr<Scope> Scope::m_global = std::make_shared<Scope>("global");

	Scope::Scope(const std::string& name) : m_name(name), m_parentScope(), m_variables(), m_functions()
	{
	}

	Scope::Scope(const std::string& name, const std::shared_ptr<Scope>& parentScope)
		: m_name(name), m_parentScope(parentScope), m_variables(), m_functions()
	{
	}

	Scope::~Scope()
	{
	}

	auto Scope::global() -> std::weak_ptr<Scope>
	{
		return m_global;
	}

	auto Scope::declareVariable(const std::string& name, const TypeInfo& typeInfo) -> std::weak_ptr<Variable>
	{
		auto variable = std::make_shared<Variable>(this, name, typeInfo);

		if (m_variables.contains(name)) {
			throw std::runtime_error("Variable with same name already declared");
		}

		m_variables[name] = variable;

		return variable;
	}

	auto Scope::getVariable(const std::string& name) -> std::weak_ptr<Variable>
	{
		if (!m_variables.contains(name)) {
			if (m_parentScope.has_value()) {
				return m_parentScope.value()->getVariable(name);
			}
			throw std::runtime_error("Variable not declared");
		}

		return m_variables[name];
	}

	auto Scope::getFunction(const std::string& name) -> std::weak_ptr<Function>
	{
		if (!m_functions.contains(name)) {
			if (m_parentScope.has_value()) {
				return m_parentScope.value()->getFunction(name);
			}
			throw std::runtime_error("Function not declared");
		}

		return m_functions[name];
	}

	auto Scope::declareFunction(const std::string& name,
		const std::vector<FunctionParameter> parameters,
		const std::optional<TypeInfo>& returnType,
		const std::vector<Expression> expressions) -> std::weak_ptr<Function>
	{
		auto function = std::make_shared<Function>(this, name, parameters, returnType, expressions);

		if (m_functions.contains(name)) {
			throw std::runtime_error("Function with same name already declared");
		}

		m_functions[name] = function;

		return function;
	}

	void Scope::printInfo()
	{
		std::cout << "Scope: " << m_name << "\n";

		std::cout << "Variables:" << "\n";
		for (const auto& [name, variable] : m_variables) {
			std::cout << "  " << name << "\n";
		}

		std::cout << "Functions:" << "\n";
		for (const auto& [name, function] : m_functions) {
			std::cout << "  " << name << "\n";
		}
	}

}
