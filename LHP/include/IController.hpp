#pragma once

#include "ViewData.hpp"
#include "ApplicationController.hpp"
#include "ViewData.hpp"
#include "ControllerContext.hpp"
#include "LuaState.hpp"
#include "Preprocessor.hpp"
#include "json.hpp"
#include "response.hpp"

namespace LHP {

class IController
{
public:
	IController(){}

	template <class T> static void RenderHtml(const ControllerContext &oldContext,const std::string &controllerName,const std::string &actionName,const ViewData<T> &viewData,Server::Response &response)
	{
		ControllerContext context(controllerName,actionName,oldContext.GetHttpMethodName(),oldContext.GetDocumentRoot());
		RenderHtml<T>(context,viewData,response);
	}

	/**
	 renders the supplied viewData into HTML using the appropriate matching lua template view
	 */
	template <class T> static void RenderHtml(const ControllerContext &context,const ViewData<T> &viewData,Server::Response &response)
	{
		try 
		{
			Preprocessor preprocessor(context.GetDocumentRoot(),GetScriptFile(context));
			LuaState state(true);
			try 
			{
				state.OpenLibs();//open standard libs
				state.OpenLuaBind();//add luabind functionality to state
				//bind the request and viewdata to lua
				viewData.GetModel().Bind(state);
				Bind(state);
				//pass the viewData and response across to lua
				luabind::globals(state)["viewData"] = &viewData.GetModel();
				luabind::globals(state)["response"] = &response;
				//process the script
				state.DoString(preprocessor.GetProcessedScript());
			} catch(luabind::error e) {
				RenderErrorHtml(response,state.GetErrorString(e));
			} catch(const std::exception &) {
				RenderErrorHtml(response,state.GetErrorString());
			}
		} catch(PreprocessorError e) {
			RenderErrorHtml(response,e.what());
		}
	}

	static void Bind(lua_State *state);
	static std::string GetScriptFile(const ControllerContext &context);

	/**
	 renders a json object into the response stream
	 */
	static void RenderJson(const Json::Object &json,Server::Response &response);

	/**
	 renders an error page
	 */
	static void RenderErrorHtml(Server::Response &response,const std::string &error);
};

}
