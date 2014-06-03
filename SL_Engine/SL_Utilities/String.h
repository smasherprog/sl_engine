#ifndef SL_STRING_H_123
#define SL_STRING_H_123
#include <vector>
#include <string>
#include <sstream>

inline std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}
inline std::string trim(const std::string& str)
{
	std::size_t first = str.find_first_not_of(' ');
	if (first == std::string::npos) return "";
	std::size_t last = str.find_last_not_of(' ');
	return str.substr(first, last - first + 1);
}
inline bool starts_with(const std::string& string_to_search, const std::string& search_term )
{
	return string_to_search.find_first_of(search_term) == 0;
}
inline bool ends_with(std::string const& string_to_search, std::string const& search_term) {
	return search_term.size() <= string_to_search.size() && string_to_search.find(search_term, string_to_search.size() - search_term.size()) != string_to_search.npos;
}

#endif