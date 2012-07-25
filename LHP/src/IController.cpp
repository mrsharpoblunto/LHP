#include "StdAfx.h"

#include <sstream>
#include <boost/lexical_cast.hpp>

#include "IController.hpp"

namespace LHP {

	void IController::Bind(lua_State *state)
	{
		luabind::module(state)
		[
			luabind::class_<Server::Response>("response")
			.def("write", &Server::Response::WriteContent)
		];
	}

	std::string IController::GetScriptFile(const ControllerContext &context)
	{
		return context.GetDocumentRoot()+"\\Views\\"+context.GetController()+"\\"+context.GetAction()+".lhp";
	}

	void IController::RenderJson(const Json::Object &json,Server::Response &response)
	{
		try {
			response.Status = Server::Response::Ok;
			std::ostringstream stream;
			stream << json;
			response.Content = stream.str();
			response.Headers[0].Name = "Content-Length";
			response.Headers[0].Value = boost::lexical_cast<std::string>(response.Content.size());
			response.Headers[1].Name = "Content-Type";
			response.Headers[1].Value = "text/javascript";
		}
		catch (const std::exception &e) {
			RenderErrorHtml(response,e.what());
		}
	}

	void IController::RenderErrorHtml(Server::Response &response,const std::string &error)
	{
		response.Status = Server::Response::InternalServerError;
		response.Content = "<html><head><title>Internal Server Error</title></head><body><h1>500 Internal Server Error</h1><p>"+error+"</p></body></html>";
		response.Headers.resize(2);
		response.Headers[0].Name = "Content-Length";
		response.Headers[0].Value = boost::lexical_cast<std::string>(response.Content.size());
		response.Headers[1].Name = "Content-Type";
		response.Headers[1].Value = "text/html";
	}

}