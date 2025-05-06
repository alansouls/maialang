#pragma once

#include <memory>
#include <string>
#include <vector>

namespace MaiaLang
{

	struct TypeInfo
	{
		std::string name;
		size_t size;
		void (*deallocData)(void*);

		struct BuiltIn {
			~BuiltIn() = delete;

			inline static auto integer() -> TypeInfo {
				return TypeInfo{
					.name = "integer",
					.size = sizeof(int32_t),
					.deallocData = &std::free
				};
			}

			inline static auto string() -> TypeInfo {
				return TypeInfo{
					.name = "string",
					.size = sizeof(std::string),
					.deallocData = &deallocString
				};
			}

		private:
			static void deallocString(void* rawPtrString) {
				delete reinterpret_cast<std::string*>(rawPtrString);
			}
		};

		auto operator==(const TypeInfo& other) const -> bool {
			return name == other.name;
		}
	};

	struct MemoryAllocation
	{
		void* internalPtr;
		int* refCount;
		TypeInfo typeInfo;

		void freeInternalPtr()
		{
			typeInfo.deallocData(internalPtr);
		}
	};

	class GC
	{
	public:

		static const MemoryAllocation allocate(const TypeInfo& typeInfo);

		static void collect();

		static void printInfo();

	private:
		static std::unique_ptr<GC> m_instance;
		std::vector<MemoryAllocation> m_allocations;

		static void init();
	};
}