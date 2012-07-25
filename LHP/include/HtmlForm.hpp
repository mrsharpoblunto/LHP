#pragma once

#include <map>
#include "request.hpp"

namespace LHP {

/**
represents a parsed HTML forms contents
*/
class HtmlForm {
public:
	virtual ~HtmlForm(){}
	HtmlForm(const Server::Request &req);
	std::string GetValue(const std::string &name) const;
	bool HasValue(const std::string &name) const;
private:
	std::string UriDecode(const std::string & sSrc) const;
	std::map<std::string,std::string> _nameValues;
};

}