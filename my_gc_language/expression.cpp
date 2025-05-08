#include "expression.h"
#include "scope.h"
#include "variable.h"

namespace MaiaLang
{
	static auto getConstValueExpressionValue(const ExpressionParameters& parameters) -> ExpressionValue;

	Expression::Expression(const ExpressionParameters& parameters) :
		m_parameters(parameters)
	{
	}

	auto Expression::evaluate(Scope &scope) const -> std::optional<ExpressionValue>
	{
		switch (m_parameters.type()) {
		case OperationType::ConstValue:
			return getConstValueExpressionValue(m_parameters);
		case OperationType::VariableDeclaration:
		{
			const VariableDeclarationParameters& variableDeclaration = m_parameters.getVariableDeclaration();
			auto variable = declareVariable(scope, variableDeclaration);
			if (variableDeclaration.assignment.has_value()) {
				variable.lock()->assign(variableDeclaration.assignment.value().expression->evaluate(scope).value().memoryValue); //TODO: handle failure
			}
			return std::optional<ExpressionValue>();
		}
		case OperationType::VariableAssignment:
			return ExpressionValue{ assignVariable(scope, m_parameters.getVariableAssignmentParameters()) };
		case OperationType::FunctionCall:
			return callFunction(scope, m_parameters.getFunctionCallParameters());
		default:
			throw std::runtime_error("Invalid expression");
		}
	}

	auto Expression::callFunction(Scope& scope, const FunctionCallParameters& parameters) -> std::optional<ExpressionValue> {
		auto function = scope.getFunction(parameters.functionName);

		std::vector<MemoryAllocation> expressionValues(parameters.parameterExpressions.size());

		size_t i = 0;
		for (auto& expression : parameters.parameterExpressions) {			
			auto expressionValue = expression->evaluate(scope);

			if (!expressionValue.has_value()) {
				throw std::runtime_error("Function parameter expressions must result in value");
			}

			expressionValues[i] = expressionValue.value().memoryValue;
			++i;
		}

		auto functionResult = function.lock()->execute(expressionValues);

		if (functionResult.has_value()) {
			return ExpressionValue{
				functionResult.value()
			};
		}

		return std::optional<ExpressionValue>();
	}

	auto Expression::assignVariable(Scope& scope, const VariableAssignmentParameters& parameters) -> MemoryAllocation {
		(void)scope;
		(void)parameters;

		return MemoryAllocation();
	}

	auto Expression::declareVariable(Scope &scope, const VariableDeclarationParameters& parameters) -> std::weak_ptr<Variable> {

		TypeInfo type;

		if (parameters.typeName == "int") {
			type = TypeInfo::BuiltIn::integer();
		}
		else if (parameters.typeName == "string") {
			type = TypeInfo::BuiltIn::string();
		}
		else {
			throw std::runtime_error("Unknown type: " + parameters.typeName);
		}

		return scope.declareVariable(parameters.variableName, type);
	}

	static auto getConstValueExpressionValue(const ExpressionParameters& parameters) -> ExpressionValue
	{
		auto& value = parameters.getConstValue();

		if (value.starts_with("\"")) {
			if (!value.ends_with("\"")) {
				throw std::runtime_error("Invalid symbol");
			}
			auto strValue = value.length() > 2 ?
				value.substr(1, value.substr().length() - 2) :
				"";

			auto memoryValue = GC::allocate(TypeInfo::BuiltIn::string());

			new(memoryValue.internalPtr) std::string(strValue);

			return ExpressionValue{
				memoryValue
			};
		}

		if (int32_t integerValue = std::stoi(value); std::to_string(integerValue) == value) {
			auto memoryValue = GC::allocate(TypeInfo::BuiltIn::integer());

			*(reinterpret_cast<int32_t*>(memoryValue.internalPtr)) = integerValue;

			return ExpressionValue{
				memoryValue
			};
		}

		throw std::runtime_error("invalid symbol");
	}
}