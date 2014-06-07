#include "IParse.h"
#include "String.h"

//simple parser right now. Basic checks. wont work if there are no line breaks in the string passed, but that is simple to add later
std::string SL_Utilities::IParse::_remove_comments_and_linebreaks(const std::string& s)const {
	std::string lines;
	lines.reserve(s.size());
	int incommentblock = 0;
	//for each line
	for (const auto& line : split(s, '\n')){
		auto temp = trim(line);//trim any extra spaces off the begin and end of each line
		if (starts_with(temp, "/*")){ incommentblock += 1; continue; }//entered comment block skip
		else if (ends_with(temp, "*/")) { incommentblock -= 1; continue; }//finnaly, the comment block has decreased by one
		else if (incommentblock > 0) continue;//still inside of a comment block.. keep going
		else if (starts_with(temp, "//"))continue; //single line comment.. skip
		lines.append(temp);
	}
	return lines;
}
//this removes all spaces except single spaces. So, the text "hey  there  this" becomes  "hey there this"
std::string SL_Utilities::IParse::_remove_extraspaces(const std::string& s) const{
	std::vector<char> otemp;
	otemp.reserve(s.size());

	bool prevspace = false;
	for (auto& x : s){
		if (x == ' '){
			if (prevspace)continue;
			else otemp.push_back(' ');
			prevspace = true;
		}
		else {
			otemp.push_back(x);
			prevspace = false;
		}
	}
	return std::string(&otemp[0], otemp.size());
}