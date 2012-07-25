#pragma once

#include <string>

namespace LHP {

	/**
	 provides context to the currently executing action handler as to the controller that invoked it
	 */
	class ControllerContext
	{
	public:
		virtual ~ControllerContext(){};
		ControllerContext(const std::string &controller,const std::string &action,const std::string &httpMethodName,const std::string &documentRoot);
		const std::string &GetController() const;
		const std::string &GetAction() const;
		const std::string &GetHttpMethodName() const;
		const std::string &GetDocumentRoot() const;
	private:
		std::string _controller;
		std::string _action;
		std::string _httpMethodName;
		std::string _documentRoot;
	};
}