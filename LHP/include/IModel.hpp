#pragma once

extern "C"
{
    #include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

#pragma warning ( push )
#pragma warning( disable : 4996 )
#include <xutility>
#pragma warning ( pop )

#pragma warning ( push )
#pragma warning( disable : 4251 )
#pragma warning( disable : 4996 )
#include <luabind/luabind.hpp>
#pragma warning( pop )

namespace LHP {

	/** 
	 base class for a model object which we can bind to lua for manipulation in lua template files
	 */
	class IModel
	{
	public:
		virtual ~IModel(){}
		virtual void Bind(lua_State *state)=0;
	};

}