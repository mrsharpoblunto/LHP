#include "stdAfx.h"

#pragma warning ( push )
#pragma warning( disable : 4996 )
#include <boost/algorithm/string.hpp>
#pragma warning ( pop )

#include <sstream>
#include "ApplicationController.hpp"
#include "IController.hpp"

namespace LHP {

	bool ApplicationController::HandleRequest(const Server::Request &request,Server::Response &response) const
	{
		std::vector<std::string> segments;
		boost::split(segments, request.Uri, boost::is_any_of("/"));

		if (segments.size()>=3) {
			ControllerContext context(segments[1],segments[2],request.Method,_documentRoot);
			if (context.GetHttpMethodName()=="GET") {
				auto handler = _actionHandlers.find(context.GetController());
				if (handler !=_actionHandlers.end())
				{
					auto action = handler->second.GetHandlers.find(context.GetAction());
					if (action!=handler->second.GetHandlers.end()) {
						std::string id = "";
						for (unsigned int i=3;i<segments.size();++i) {
							id+=segments[i];
							if (i<segments.size()-1) {
								id+="/";
							}
						}
						action->second(context,id,request,response);
						return true;
					}
				}
			}
			else if (context.GetHttpMethodName()=="POST")
			{
				auto handler = _actionHandlers.find(context.GetController());
				if (handler!=_actionHandlers.end())
				{
					auto postHandler = handler->second.PostHandlers.find(context.GetAction());
					if (postHandler!=handler->second.PostHandlers.end()) {
						//Form post handler
						if (postHandler->second.FormPost!=NULL) 
						{
							HtmlForm form(request);
							postHandler->second.FormPost(context,form,request,response);
						}
						//json post handler
						else 
						{
							Json::Object o;
							std::stringstream s(request.PostData);
							Json::Object::parse(s,o);
							postHandler->second.JsonPost(context,o,request,response);
						}
						return true;
					}
				}
			}
		}
		return false;
	}

	//GET handlers
	void ApplicationController::RegisterControllerGetAction(std::string controller,std::string action,GetHandler handler)
	{
		boost::to_lower(controller);
		boost::to_lower(action);
		if (_actionHandlers.find(controller)!=_actionHandlers.end()) {
			_actionHandlers[controller].GetHandlers[action] = handler;
		}
	}

	//POST handlers
	void ApplicationController::RegisterControllerFormPostAction(std::string controller,std::string action,FormPostHandler handler)
	{
		boost::to_lower(controller);
		boost::to_lower(action);

		if (_actionHandlers.find(controller)!=_actionHandlers.end()) {
			PostHandler postHandler;
			postHandler.JsonPost = NULL;
			postHandler.FormPost = handler;
			_actionHandlers[controller].PostHandlers[action] = postHandler;
		}
	}

	void ApplicationController::RegisterControllerJsonPostAction(std::string controller,std::string action,JsonPostHandler handler)
	{
		boost::to_lower(controller);
		boost::to_lower(action);

		if (_actionHandlers.find(controller)!=_actionHandlers.end()) {
			PostHandler postHandler;
			postHandler.JsonPost = handler;
			postHandler.FormPost = NULL;
			_actionHandlers[controller].PostHandlers[action] = postHandler;
		}
	}
}