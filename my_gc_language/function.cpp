#include "function.h"
#include "scope.h"
#include <iostream>
namespace MaiaLang
{
	static std::weak_ptr<Function> printlnFunction = Scope::global().lock()->declareFunction("println",
		{ FunctionParameter {.typeInfo = TypeInfo::BuiltIn::string(), .name = "text"} },
		std::optional<TypeInfo>(),
		{});

	static auto executePrintln(const std::vector<MemoryAllocation>& parameterValues) {
		if (parameterValues.size() != 1) {
			throw std::runtime_error("Invalid call");
		}

		auto& parameter = parameterValues[0];

		if (parameter.typeInfo == TypeInfo::BuiltIn::string() && parameter.typeInfo != TypeInfo::BuiltIn::integer()) {
			std::string& textToPrint = *reinterpret_cast<std::string*>(parameterValues[0].internalPtr);

			std::cout << textToPrint << "\n";
		}
		else if (parameter.typeInfo == TypeInfo::BuiltIn::integer()) {
			int32_t& intToPrint = *reinterpret_cast<int32_t*>(parameterValues[0].internalPtr);
			std::cout << intToPrint << "\n";
		}
		else {
			throw std::runtime_error("Invalid call, parameter must be a string or int");
		}
	}

	static auto isBuiltInFunction(const Function* function) -> bool
	{
		if (printlnFunction.lock().get() == function) {
			return true;
		}

		return false;
	}

	static auto executeBuiltIn(const Function* function, const std::vector<MemoryAllocation>& parameterValues) -> std::optional<MemoryAllocation>
	{
		if (printlnFunction.lock().get() == function) {
			executePrintln(parameterValues);

			return std::optional<MemoryAllocation>();
		}

		throw std::runtime_error("Invalid call, function is not built in");
	}

	auto Function::BuiltIn::println() -> const std::weak_ptr<Function>&
	{
		return printlnFunction;
	}

	Function::Function(Scope* scope,
		const std::string& name,
		const std::vector<FunctionParameter> parameters,
		const std::optional<TypeInfo>& returnType,
		const std::vector<Expression> expressions) :
		m_scope(scope),
		m_name(name),
		m_parameters(parameters),
		m_returnType(returnType),
		m_expressions(expressions)
	{
	}

	auto Function::execute(const std::vector<MemoryAllocation>& parameterValues) const -> std::optional<MemoryAllocation>
	{
		if (isBuiltInFunction(this)) {
			executeBuiltIn(this, parameterValues);
		}

		return std::optional<MemoryAllocation>();
	}
}