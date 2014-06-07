#ifndef SL_PARSE_H_123
#define SL_PARSE_H_123
#include <string>

namespace SL_Utilities{
	class IParse{
	protected:
		std::string _remove_comments_and_linebreaks(const std::string& s) const;
		std::string _remove_extraspaces(const std::string& s) const;
	public:
		virtual ~IParse(){}
		virtual void Run(void* obj, const char* text, size_t len) = 0;

	};
};

#endif