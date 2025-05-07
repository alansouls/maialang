#pragma once
#include "expression.h"

namespace MaiaLang
{
	class Scope;

	struct FunctionParameter
	{
		TypeInfo typeInfo;
		std::string name;
	};

	class Function
	{
	public:
		Function(Scope* scope,
			const std::string& name,
			const std::vector<FunctionParameter> parameters,
			const std::optional<TypeInfo>& returnType,
			const std::vector<Expression> expressions);

		struct BuiltIn
		{
			static auto println() -> const std::weak_ptr<Function>&;
		};

		auto execute(const std::vector<MemoryAllocation> &parameterValues) const -> std::optional<MemoryAllocation>;

	private:
		Scope* m_scope;
		std::string m_name;
		std::vector<FunctionParameter> m_parameters;
		std::optional<TypeInfo> m_returnType;
		std::vector<Expression> m_expressions;
	};
}