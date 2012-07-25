#include "stdAfx.h"

#pragma warning ( push )
#pragma warning( disable : 4996 )
#include <boost/algorithm/string.hpp>
#pragma warning ( pop )

#include "ControllerContext.hpp"

namespace LHP {

ControllerContext::ControllerContext(const std::string &controller,const std::string &action,const std::string &httpMethodName,const std::string &documentRoot)
{
	_documentRoot = documentRoot;
	_controller =  controller;
	_action = action;
	_httpMethodName = httpMethodName;

	boost::to_lower(_controller);
	boost::to_lower(_action);
	boost::to_upper(_httpMethodName);
}

const std::string &ControllerContext::GetController() const
{
	return _controller;
}

const std::string &ControllerContext::GetAction() const
{
	return _action;
}

const std::string &ControllerContext::GetHttpMethodName() const
{
	return _httpMethodName;
}

const std::string &ControllerContext::GetDocumentRoot() const
{
	return _documentRoot;
}

}