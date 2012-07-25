#pragma once

#include <string>
#include <map>
#include <list>

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
 provides a wrapper to the standard lua state and functions including the processing of files and strings
 with exception handling. In addition this class provides a shared pointer to a given lua_state which
 can be shared between LuaState classes and will be closed correctly when the last user of the state
 is destroyed.
 WARNING: LuaState classes cannot be passed between DLL's or from an exe to a DLL or vice versa because
 of the reliance on static variables to do reference counting.
 \author gcconner
 */
class LuaState {
public:
	/**
	enableStacktracing means that a stack trace is kept for the current lua state
	in debug mode, this is always enabled regardless of the value passed to the constructor
	*/
	LuaState(bool enableStackTracing = false);
	LuaState(const LuaState &s);//copy constructor from reference
	LuaState(const LuaState *s);//copy constructor from pointer
	LuaState(const lua_State *s);//copy constructor from raw lua_State pointer
	~LuaState();

	/**
	get the name of a luabind error from off the top of the lua stack
	\return a string containing the error message
	*/
	static std::string GetErrorString(luabind::error &err);

	/**
	if an error has occurred, this returns the message from the top of the lua stack
	and the callstack (if stack tracing is enabled)
	\return a string containing the error message (and possible a stacktrace)
	*/
	std::string GetErrorString();

	/**
	returns the current callstack for this lua state
	*/
	std::list<std::string> GetCallStack();

	/**
	get the raw lua_State pointer
	WARNING: manipulating the pointer outside of the class will cause problems
	\return the raw pointer to the lua_state
	*/
	const lua_State *GetStatePtr();

	/**
	loads a lua script from a file and runs it
	\param filename the script file to run
	*/
	void DoFile(std::string filename); //throws std::exception

	/**
	loads a lua script contained in a string and runs it
	\param strName the string to run
	*/
	void DoString(std::string strName); //throws std::exception

	/**
	opens the standard libraries into the current state
	*/
	void OpenLibs();
	void OpenLuaBind();

	bool IsOpenLibs();
	bool IsOpenLuaBind();

	/**
	the overloading of the cast operator allows the implicit conversion of this class to a lua_State
	meaning that a reference to this object can be used as a parameter to any of the functions designed
	to accept a lua_State pointer with no explicit casts
	*/
	operator lua_State *();
	operator const lua_State *();

	LuaState& operator=(const LuaState &s);//assign from reference
	LuaState& operator=(const LuaState *s);//assign from pointer
	LuaState& operator=(const lua_State *s);//assign from raw lua_State pointer

	/**
	determine if two wrappers share the same underlying lua_State
	*/
	bool operator==(const LuaState &s);

protected:
	lua_State *_state;
	bool _openedLuaBind;
	bool _openedAllLibs;

	std::list<std::string> _callStack;
private:
	typedef struct {
		LuaState *Instance;
		int RefCount;
	} LuaStateInstance;

	static std::map<lua_State *,LuaStateInstance> &GetRefCounts();

	static void FunctionHook(lua_State *l, lua_Debug *ar);
};



}