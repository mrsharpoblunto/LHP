#pragma once

#include <map>

#pragma warning ( push )
#pragma warning( disable : 4996 )
#include <boost/algorithm/string.hpp>
#pragma warning ( pop )

#include "HtmlForm.hpp"
#include "ControllerContext.hpp"
#include "json.hpp"
#include "request.hpp"
#include "response.hpp"

namespace LHP {

	class IController;

	typedef std::function<void (const ControllerContext &,const HtmlForm &,const Server::Request &,Server::Response &)> FormPostHandler;
	typedef std::function<void (const ControllerContext &,const Json::Object &,const Server::Request &,Server::Response &)> JsonPostHandler;
	typedef std::function<void (const ControllerContext &,const std::string,const Server::Request &,Server::Response &)> GetHandler;

	struct PostHandler
	{
		FormPostHandler FormPost;
		JsonPostHandler JsonPost;
	};

	struct ControllerActionHandler
	{
		std::map<std::string,GetHandler> GetHandlers;
		std::map<std::string,PostHandler> PostHandlers;
	};

	/**
	 The application controller is responsible for dispatching http requests to the correct controller and action handler.
	 If a controller wants to be notified of certain http requestrs, it needs to register its action handlers using this class
	 */
	class ApplicationController {
	public:
		virtual ~ApplicationController(){}
		ApplicationController(const std::string &documentRoot): _documentRoot(documentRoot){}
		bool HandleRequest(const Server::Request &request,Server::Response &response) const;

		/**
		 Used to register a controller with the application controller
		 */
		template <class T> void RegisterController(std::string controller)
		{
			boost::to_lower(controller);
			if (_actionHandlers.find(controller)==_actionHandlers.end()) {
				ControllerActionHandler handler;
				_actionHandlers[controller] = handler;
				T::Register(*this);
			}
		};

		/**
		 register the handler for an http get action
		 */
		void RegisterControllerGetAction(std::string controller,std::string action,GetHandler handler);

		/**
		 register the handler for an http form submission via a post
		 */
		void RegisterControllerFormPostAction(std::string controller,std::string action,FormPostHandler handler);

		/**
		register the handler for a json data submission via a post
		*/
		void RegisterControllerJsonPostAction(std::string controller,std::string action,JsonPostHandler handler);
	private:
		std::map<std::string,ControllerActionHandler> _actionHandlers;
		std::string _documentRoot;
	};
}