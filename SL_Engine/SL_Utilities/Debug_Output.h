#ifndef SL_DEBUG_OUTPUT_H_123
#define SL_DEBUG_OUTPUT_H_123
#include <iostream>
#include <string>
#include <sstream>

template<typename... Args>
void DEBUG_MSG(const char *s, Args... args)
{
#ifdef _DEBUG
	std::string result = "";
	SL_Utilities::_INTERNAL::xsprintf(result, s, args...);
	std::cout << result << std::endl;
#endif
}
namespace SL_Utilities{
	namespace _INTERNAL{

		inline void xsprintf(std::string& result, const char *s)
		{
			while (*s) {
				if (*s == '%') {
					if (*(s + 1) == '%') {
						++s;
					}
					else {
						throw std::runtime_error("invalid format string: missing arguments");
					}
				}
				result += *s++;
			}
		}

		template<typename T, typename... Args>
		void xsprintf(std::string& result, const char *s, T value, Args... args)
		{
			while (*s) {
				if (*s == '%') {
					if (*(s + 1) == '%') {
						++s;
					}
					else {
						std::stringstream stream;
						stream << value;
						result += stream.str();
						xsprintf(result, s + 1, args...); // call even when *s == 0 to detect extra arguments
						return;
					}
				}
				result += *s++;
			}
			throw std::logic_error("extra arguments provided to printf");
		}
	}
}











#endif

