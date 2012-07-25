#include "StdAfx.h"
#include <sstream>
#include "luaState.hpp"

namespace LHP {

std::map<lua_State *,LuaState::LuaStateInstance> &LuaState::GetRefCounts() {
	static std::map<lua_State *,LuaStateInstance> refCounts;
	return refCounts;
}

LuaState::LuaState(bool enableStackTracing) {
	_state = luaL_newstate();

	LuaStateInstance inst;
	inst.Instance = this;
	inst.RefCount = 1;
	GetRefCounts()[_state] = inst;

	_openedAllLibs = false;
	_openedLuaBind = false;

	//hook up function call and return events to enable stack tracing
#ifdef _DEBUG
	lua_sethook(_state,&LuaState::FunctionHook,LUA_MASKCALL | LUA_MASKRET,0);
#else
	if (enableStackTracing) {
		lua_sethook(_state,&LuaState::FunctionHook,LUA_MASKCALL | LUA_MASKRET,0);
	}
#endif
}

void LuaState::FunctionHook(lua_State *l, lua_Debug *ar)
{
    lua_getinfo(l, "Sln", ar);
	LuaState *state = GetRefCounts()[l].Instance;
	//push function calls to the top of the callstack
	if (ar->event == LUA_HOOKCALL) {
		std::stringstream ss;
		ss << ar->short_src << ":"<< ar->linedefined << ": " << (ar->name == NULL ? "[UNKNOWN]" : ar->name) << " (" << ar->namewhat << ")";
		state->_callStack.push_front(ss.str());
	}
	//pop the returned function from the callstack
	else if (ar->event ==LUA_HOOKRET) {
		if (state->_callStack.size()>0)
		{
			state->_callStack.pop_front();
		}
	}
}

LuaState::LuaState(const LuaState &s) {
	++(GetRefCounts()[s._state].RefCount);
	_state = s._state;
	_openedAllLibs = s._openedAllLibs;
	_openedLuaBind = s._openedLuaBind;
}

LuaState::LuaState(const LuaState *s) {
	_ASSERT(s!=NULL);
	++(GetRefCounts()[s->_state].RefCount);
	_state = s->_state;
	_openedAllLibs = s->_openedAllLibs;
	_openedLuaBind = s->_openedLuaBind;
}

LuaState::LuaState(const lua_State *s) {
	_ASSERT(s!=NULL);
	if (GetRefCounts().find(const_cast<lua_State *>(s))!=GetRefCounts().end()) {
		++(GetRefCounts()[const_cast<lua_State *>(s)].RefCount);
	}
	else {
		GetRefCounts()[const_cast<lua_State *>(s)].RefCount=1;
	}
	_state = const_cast<lua_State *>(s);	
	_openedAllLibs = false;
	_openedLuaBind = false;
}

LuaState::~LuaState() {
	--(GetRefCounts()[_state].RefCount);
	if (GetRefCounts()[_state].RefCount==0) {
		GetRefCounts().erase(GetRefCounts().find(_state));
		lua_close(_state);
	}
}

std::string LuaState::GetErrorString(luabind::error &err)
{
	return lua_tostring(err.state(),-1);
}

std::string LuaState::GetErrorString()
{
	std::string result = lua_tostring(_state,-1);
	if (_callStack.size()>0) {
		result+="\r\n";
	}
	for(std::list<std::string>::iterator iter = _callStack.begin();iter!=_callStack.end();++iter) {
		result+=(*iter);
		result+="\r\n";
	}
	return result;
}

LuaState::operator lua_State *() {
	return _state;
}

LuaState::operator const lua_State *() {
	return _state;
}

LuaState& LuaState::operator=(const LuaState &s) {
    if (this != &s) {  // make sure not same object
		//remove current data
		--(GetRefCounts()[_state].RefCount);
		if (GetRefCounts()[_state].RefCount==0) {
			GetRefCounts().erase(GetRefCounts().find(_state));
			lua_close(_state);
		}
		//add new data
		++(GetRefCounts()[s._state].RefCount);
		_state = s._state;
		_openedAllLibs = s._openedAllLibs;
		_openedLuaBind = s._openedLuaBind;
    }
    return *this;
}

LuaState& LuaState::operator=(const LuaState *s) {
	_ASSERT(s!=NULL);
    if (this != s) {  // make sure not same object
		//remove current data
		--(GetRefCounts()[_state].RefCount);
		if (GetRefCounts()[_state].RefCount==0) {
			GetRefCounts().erase(GetRefCounts().find(_state));
			lua_close(_state);
		}
		//add new data
		++(GetRefCounts()[s->_state].RefCount);
		_state = s->_state;
		_openedAllLibs = s->_openedAllLibs;
		_openedLuaBind = s->_openedLuaBind;
    }
    return *this;
}

LuaState& LuaState::operator=(const lua_State *s) {
	_ASSERT(s!=NULL);
	if (_state!=s) {//the state is different than the current internal state
		//remove current data
		--(GetRefCounts()[_state].RefCount);
		if (GetRefCounts()[_state].RefCount==0) {
			GetRefCounts().erase(GetRefCounts().find(_state));
			lua_close(_state);
		}

		if (GetRefCounts().find(const_cast<lua_State *>(s))!=GetRefCounts().end()) {
			++(GetRefCounts()[const_cast<lua_State *>(s)].RefCount);
		}
		else {
			GetRefCounts()[const_cast<lua_State *>(s)].RefCount=1;
		}
		_state = const_cast<lua_State *>(s);	
		_openedAllLibs = false;
		_openedLuaBind = false;
	}
	return *this;
}

bool LuaState::operator==(const LuaState &s) {
	return (_state == s._state);
}

const lua_State *LuaState::GetStatePtr() {
	return _state;
}


/**
 * process a file with the lua interpreter and throw an exception if it fails
 */
void LuaState::DoFile(std::string filename)
{
    if (luaL_loadfile(_state,filename.c_str()) || lua_pcall(_state,0,0,0))
    {
		std::string err = GetErrorString();
		throw std::exception(err.c_str());
    }
}

/**
 * process a string with the lua interpreter and throw an exception if it fails
 */
void LuaState::DoString(std::string strName)
{
	if (luaL_loadstring(_state,strName.c_str()) || lua_pcall(_state,0,0,0))
    {
		std::string err = GetErrorString();
		throw std::exception(err.c_str());
    }
}

/**
opens the standard libraries into the current state
*/
void LuaState::OpenLibs() {
	if (!_openedAllLibs) {
		luaL_openlibs(_state);
		_openedAllLibs = true;
	}
}

void LuaState::OpenLuaBind() {
	if (!_openedLuaBind) {
		luabind::open(_state);
		_openedLuaBind = true;
	}
}

}
