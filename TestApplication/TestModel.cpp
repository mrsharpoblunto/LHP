#include "stdafx.h"

#include "TestModel.hpp"

std::string TestModel::GetValue() const
{
	return _value;
}

void TestModel::SetValue(std::string value)
{
	_value = value;
}

void TestModel::Bind(lua_State *state)
{
	luabind::module(state)
	[
		luabind::class_<TestModel>("TestModel")
		.property("Value", &TestModel::GetValue,&TestModel::SetValue)
	];
}
