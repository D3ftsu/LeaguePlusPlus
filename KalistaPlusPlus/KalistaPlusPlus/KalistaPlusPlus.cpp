#include <PluginSDK.h>
#include <PluginSDK.cpp>
#include "EventManager.h"
#include "SpellManager.h"

PluginSetup("KalistaPlusPlus");

PLUGIN_API void OnLoad(IPluginSDK* PluginSDK)
{
	PluginSDKSetup(PluginSDK);
	if (strcmp(GEntityList->Player()->ChampionName(), "Kalista") != 0)
	{
		return;
	}
	Config::Initialize();
	SpellManager::Initialize();
	EventManager::Initialize();
}

PLUGIN_API void OnUnload()
{
	Config::Unload();
	EventManager::Unload();
}