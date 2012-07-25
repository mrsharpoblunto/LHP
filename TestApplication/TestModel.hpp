#pragma once

#include "IModel.hpp"

class TestModel: public LHP::IModel
{
public:
	virtual ~TestModel(){}
	std::string GetValue() const;
	void SetValue(std::string value);
	virtual void Bind(lua_State *state);
private:
	std::string _value;
};
