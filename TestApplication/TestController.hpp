#pragma once

#include "IController.hpp"

class TestController: LHP::IController
{
public:
	virtual ~TestController(){}
	TestController(){}
	static void Register(LHP::ApplicationController &controller);

	static void Index(const LHP::ControllerContext &context,const std::string id,const LHP::Server::Request &request,LHP::Server::Response &response);
	static void Submit(const LHP::ControllerContext &context,const LHP::HtmlForm &form,const LHP::Server::Request &request,LHP::Server::Response &response);

};