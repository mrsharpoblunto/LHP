#pragma once

#include "ApplicationController.hpp"

namespace LHP {

/**
 provides the entry point for an LHP web application.
 All controllers should be registered in the OnCreate method
 */
class IApplication
{
public:
	IApplication(){}
	virtual ~IApplication(){}
	virtual void OnCreate(ApplicationController &controller)=0;
};

}
