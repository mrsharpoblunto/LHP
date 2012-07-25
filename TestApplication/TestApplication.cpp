#include "stdafx.h"

#include "TestApplication.hpp"
#include "TestController.hpp"

void TestApplication::OnCreate(LHP::ApplicationController &controller)
{
	controller.RegisterController<TestController>("Test");
}
