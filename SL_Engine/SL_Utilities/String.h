#ifndef SL_STRING_H_123
#define SL_STRING_H_123
#include <vector>
#include <string>
#include <sstream>

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

#endif