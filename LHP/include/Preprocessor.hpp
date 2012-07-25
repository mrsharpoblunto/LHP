#pragma once

#include <map>
#include "PreprocessorError.hpp"

namespace LHP {

class Preprocessor
{
public:
	virtual ~Preprocessor(){}
	Preprocessor(const std::string &documentRoot,const std::string &scriptFile);
	const std::string &GetProcessedScript() const;
private:
	std::string MergePlaceHolders(const std::string &masterPageFile, std::map<std::string,std::string> &placeholders) const;
	std::string ConvertToLuaBlock(const std::string &fragment) const;
	std::string GetLuaBlock(std::ifstream &input) const;
	std::pair<std::string,std::string> ParseMetaTag(std::ifstream &input) const;

	std::string _content;
};

}