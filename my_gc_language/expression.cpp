#include "expression.h"
#include "scope.h"
namespace MaiaLang
{
	static auto getConstValueExpressionValue(const ExpressionParameters& parameters) -> ExpressionValue;

	Expression::Expression(const ExpressionParameters& parameters) :
		m_parameters(parameters)
	{
	}

	auto Expression::evaluate(Scope &scope) const -> std::optional<ExpressionValue>
	{
		(void)scope;

		switch (m_parameters.type()) {
		case OperationType::ConstValue:
			return getConstValueExpressionValue(m_parameters);
			break;
		case OperationType::VariableDeclaration:
			return std::optional<ExpressionValue>();
			break;
		case OperationType::VariableAssignment:
			return std::optional<ExpressionValue>();
			break;
		default:
			throw std::runtime_error("Invalid expression");
		}
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