#include "variable.h"

namespace MaiaLang 
{
Variable::Variable(const Scope* scope, const std::string &name, const TypeInfo& typeInfo) 
	: m_scope(scope), m_name(name), m_typeInfo(typeInfo), m_allocation()
{
}

Variable::~Variable()
{
	if (m_allocation.has_value()) {
		--(*m_allocation.value().refCount);
	}
}

void Variable::assign(const MemoryAllocation& allocation)
{
	if (m_typeInfo != allocation.typeInfo) {
		throw std::runtime_error("Invalid type");
	}

	if (m_allocation.has_value()) {
		--(*m_allocation.value().refCount);
	}

	++(*allocation.refCount);
	m_allocation = allocation;
}

auto Variable::allocation() -> const MemoryAllocation&
{
	if (!m_allocation.has_value()) {
		throw std::runtime_error("Variable not initalized");
	}

	return m_allocation.value();
}

}
