#pragma once
#include <string>

namespace MaiaLang::StringUtils
{
	static auto trim(const std::string& str) -> std::string
	{
		size_t first = str.find_first_not_of(" \t\n\r");
		if (first == std::string::npos) {
			return "";
		}
		size_t last = str.find_last_not_of(" \t\n\r");
		return str.substr(first, (last - first + 1));
	}

	static auto split(const std::string& str, char separator) -> std::vector<std::string> {
		if (str.empty()) {
			return { str };
		}

		std::vector<std::string> result;

		size_t start = 0;
		size_t i = 0;

		for (auto& c : str) {
			if (c == separator) {
				result.push_back(str.substr(start, i - start));
				start = i + 1;
			}

			++i;
		}

		result.push_back(str.substr(start));

		return result;
	}
}