#include "gc.h"
#include <algorithm>

namespace MaiaLang
{
std::unique_ptr<GC> GC::m_instance;

const MemoryAllocation MaiaLang::GC::allocate(const TypeInfo &typeInfo)
{
	if (m_instance == nullptr) {
		init();
	}
	void* ptr = std::malloc(typeInfo.size);

	auto allocation = MemoryAllocation {
		.internalPtr = ptr,
		.refCount = new int(0),
		.typeInfo = typeInfo
	};

	m_instance->m_allocations.push_back(allocation);

	return allocation;
}

void GC::collect()
{
	if (m_instance == nullptr) {
		init();
	}

	std::vector<MemoryAllocation>& allocations = m_instance->m_allocations;

	auto to_remove = std::remove_if(allocations.begin(), allocations.end(), [](MemoryAllocation& allocation) {
		if (*allocation.refCount == 0) {
			allocation.freeInternalPtr();
			delete allocation.refCount;
			return true;
		}

		return false;
	});

	allocations.erase(to_remove, allocations.end());
}

void GC::printInfo()
{
	if (m_instance == nullptr) {
		init();
	}
	long long totalAllocated = 0;

	for (auto& allocation : m_instance->m_allocations) {
		totalAllocated += allocation.typeInfo.size;
	}

	std::printf("Size allocated: %lld bytes\n", totalAllocated);
	std::printf("Total allocations: %lld\n", m_instance->m_allocations.size());
}

void MaiaLang::GC::init()
{
	m_instance = std::make_unique<GC>();
}
}
