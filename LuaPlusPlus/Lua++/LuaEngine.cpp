#include "Script.h"
#include <PluginSDK.h>

void Script::ReportErrors(int state)
{
	if (state != 0)
	{
		GGame->PrintChat(lua_tostring(State, state));
		lua_pop(State, 1);
	}
}

void Script::ExecuteFile(const char* file)
{
	if (file == NULL)
		return;
	int state = luaL_dofile(State, file);
	ReportErrors(state);
}

void Script::ExecuteString(const char* expression)
{
	if (expression == NULL)
		return;
	int state = luaL_dostring(State, expression);
	ReportErrors(state);
}
