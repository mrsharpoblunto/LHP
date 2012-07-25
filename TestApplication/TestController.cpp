#include "stdafx.h"

#include "TestController.hpp"
#include "TestModel.hpp"

void TestController::Register(LHP::ApplicationController &controller)
{
	controller.RegisterDefaultControllerGetAction("Test","Index");
	controller.RegisterControllerGetAction("Test","Index",&TestController::Index);
	controller.RegisterControllerFormPostAction("Test","Submit",&TestController::Submit);
}

void TestController::Index(const LHP::ControllerContext &context,const std::string id,const LHP::Server::Request &request,LHP::Server::Response &response)
{
	TestModel testModel;
	testModel.SetValue(id.empty() ? "world" : id);
	LHP::ViewData<TestModel> viewData(testModel);

	RenderHtml<TestModel>(context,viewData,response);
}

void TestController::Submit(const LHP::ControllerContext &context,const LHP::HtmlForm &form,const LHP::Server::Request &request,LHP::Server::Response &response)
{
	TestModel testModel;
	testModel.SetValue(form.GetValue("txt"));
	LHP::ViewData<TestModel> viewData(testModel);

	RenderHtml<TestModel>(context,"Test","Index",viewData,response);
}
