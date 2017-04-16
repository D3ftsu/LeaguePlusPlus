#pragma once

#pragma once

#include <lua.hpp>
#include <LuaIntf.h>
#include "Script.h"
#include <vector>

static class Engine
{
public:
	static std::vector<Script*> Instances;
	static std::vector<std::string> Scripts;
	static void Init(lua_State* L);
	static std::string GetStartupPath(std::string append);
	static void LoadScript(std::string file);
};