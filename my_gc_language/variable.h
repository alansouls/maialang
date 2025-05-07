#pragma once

#include "gc.h"
#include "scope.h"
#include <optional>

namespace MaiaLang
{
class Variable {
public:
	Variable(const Scope* scope, const std::string &name, const TypeInfo& typeInfo);
	~Variable();

	Variable(const Variable&) = delete;
	Variable& operator= (const Variable&) = delete;

	void assign(const MemoryAllocation& allocation);

	auto allocation() -> const MemoryAllocation&;

private:
	const Scope* m_scope;
	const std::string m_name;
	const TypeInfo m_typeInfo;

	std::optional<MemoryAllocation> m_allocation;
};
}