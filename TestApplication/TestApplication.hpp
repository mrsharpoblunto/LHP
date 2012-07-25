#pragma once

#include "IApplication.hpp"

class TestApplication: public LHP::IApplication {
public:
	virtual ~TestApplication(){}
	virtual void OnCreate(LHP::ApplicationController &controller);
};