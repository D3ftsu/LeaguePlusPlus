#include <PluginSDK.h>
#include <PluginSDK.cpp>
#include "Engine.h"
#include <experimental\filesystem>

PluginSetup("LuaPlusPlus");
using namespace LuaIntf;
namespace fs = std::experimental::filesystem;

IMenu* Menu;
IMenu* ScriptsMenu;

PLUGIN_EVENT(void) OnBuffAdd(IUnit* source, void* buffData)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnBuffAdd");
		if (func.isFunction())
		{
			func(source, buffData);
		}
	}
}

PLUGIN_EVENT(void) OnBuffRemove(IUnit* source, void* buffData)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnBuffRemove");
		if (func.isFunction())
		{
			func(source, buffData);
		}
	}
}

PLUGIN_EVENT(void) OnCreateObj(IUnit* source)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnCreateObj");
		if (func.isFunction())
		{
			func(source);
		}
	}
}

PLUGIN_EVENT(void) OnDash(UnitDash* dash)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnDash");
		if (func.isFunction())
		{
			func(dash);
		}
	}
}

PLUGIN_EVENT(void) OnDeleteObj(IUnit* source)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnDeleteObj");
		if (func.isFunction())
		{
			func(source);
		}
	}
}

PLUGIN_EVENT(void) OnAttack(IUnit* source, IUnit* target)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnAttack");
		if (func.isFunction())
		{
			func(source, target);
		}
	}
}

PLUGIN_EVENT(void) OnAfterAttack(IUnit* source, IUnit* target)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnAfterAttack");
		if (func.isFunction())
		{
			func(source, target);
		}
	}
}

PLUGIN_EVENT(void) OnTargetChange(IUnit* oldTarget, IUnit* newTarget)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnTargetChange");
		if (func.isFunction())
		{
			func(oldTarget, newTarget);
		}
	}
}

PLUGIN_EVENT(void) OnUnkillableMinion(IUnit* minion)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnUnkillableMinion");
		if (func.isFunction())
		{
			func(minion);
		}
	}
}

PLUGIN_EVENT(void) OnGameUpdate()
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnGameUpdate");
		if (func.isFunction())
		{
			func();
		}
	}
}

PLUGIN_EVENT(void) OnRender()
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnRender");
		if (func.isFunction())
		{
			func();
		}
	}
}

PLUGIN_EVENT(void) OnProcessSpell(CastedSpell const& spell)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnProcessSpell");
		if (func.isFunction())
		{
			func(spell);
		}
	}
}

PLUGIN_EVENT(void) OnDeath(IUnit* source)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnDeath");
		if (func.isFunction())
		{
			func(source);
		}
	}
}

PLUGIN_EVENT(void) OnDoCast(CastedSpell const& spell)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnDoCast");
		if (func.isFunction())
		{
			func(spell);
		}
	}
}

PLUGIN_EVENT(void) OnInterruptible(InterruptibleSpell const& spell)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnInterruptible");
		if (func.isFunction())
		{
			func(spell);
		}
	}
}

PLUGIN_EVENT(void) OnGapCloser(GapCloserSpell const& spell)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnGapCloser");
		if (func.isFunction())
		{
			func(spell);
		}
	}
}

bool _blockorder = false;
PLUGIN_EVENT(bool) OnIssueOrder(IUnit* source, eGameObjectOrder orderIdx, Vec3* position, IUnit* target)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnIssueOrder");
		if (func.isFunction())
		{
			LuaRef table = LuaRef::createTable((*it)->State);
			table["flag"] = orderIdx;
			table["target"] = target;
			table["position"] = position;
			table["Process"] = true;
			func(table);
			if (table["Process"].value() == false)
			{
				_blockorder = true;
			}
		}
	}
	if (_blockorder == true)
	{
		_blockorder = false;
		return false;
	}
	return true;
}

PLUGIN_EVENT(void) OnGameEnd()
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnGameEnd");
		if (func.isFunction())
		{
			func();
		}
	}
}

PLUGIN_EVENT(void) OnLevelUp(IUnit* source, int newLevel)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnLevelUp");
		if (func.isFunction())
		{
			func(source, newLevel);
		}
	}
}

bool _blockcast = false;
PLUGIN_EVENT(bool) OnSpellCast(eSpellSlot slot, IUnit* target, Vec3* startPosition, Vec3* endPosition)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnSpellCast");
		if (func.isFunction())
		{
			LuaRef table = LuaRef::createTable((*it)->State);
			table["slot"] = slot;
			table["target"] = target;
			table["startPos"] = startPosition;
			table["endPos"] = endPosition;
			table["Process"] = true;
			func(table);
			if (table["Process"].value() == false)
			{
				_blockcast = true;
			}
		}
	}
	if (_blockcast == true)
	{
		_blockcast = false;
		return false;
	}
	return true;
}

PLUGIN_EVENT(void) OnHudRender()
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnHudRender");
		if (func.isFunction())
		{
			func();
		}
	}
}

bool Loaded = false;

PLUGIN_EVENT(bool) OnWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (wParam == 117 && Loaded) // F6
	{
		for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
		{
			LuaRef func = Lua::getGlobal((*it)->State, "OnUnload");
			if (func.isFunction())
			{
				func();
			}
			lua_close((*it)->State);
		}
		Engine::Instances.clear();
		Loaded = false;
		GGame->PrintChat("[Lua++] Lua Engine succesfully unloaded!");
	}
	else if (wParam == 116 && !Loaded) // F5
	{
		for (std::string str : Engine::Scripts)
		{
			if (ScriptsMenu->GetOption(str.c_str())->Enabled())
				Engine::LoadScript(str);
		}
		Loaded = true;
		GGame->PrintChat("[Lua++] Lua Engine succesfully load!");
		return true;
	}

	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnWndProc");
		if (func.isFunction())
		{
			func(message, wParam);
		}
	}
	return true;
}

PLUGIN_EVENT(void) OnVisionGain(IUnit* source)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnVisionGain");
		if (func.isFunction())
		{
			func(source);
		}
	}
}

PLUGIN_EVENT(void) OnVisionLose(IUnit* source)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnVisionLose");
		if (func.isFunction())
		{
			func(source);
		}
	}
}

PLUGIN_EVENT(void) OnUpdateChargedSpell(eSpellSlot slot, Vec3* position, bool* releaseCast)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnUpdateChargedSpell");
		if (func.isFunction())
		{
			func(slot, position, releaseCast);
		}
	}
}

PLUGIN_EVENT(void) OnPlayAnimation(IUnit* source, std::string const animation)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnPlayAnimation");
		if (func.isFunction())
		{
			func(source, animation);
		}
	}
}

PLUGIN_EVENT(void) OnPauseAnimation(IUnit* source)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnPauseAnimation");
		if (func.isFunction())
		{
			func(source);
		}
	}
}

PLUGIN_EVENT(void) OnJungleNotification(JungleNotifyData* data)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnJungleNotification");
		if (func.isFunction())
		{
			func(data);
		}
	}
}

PLUGIN_EVENT(void) OnNewPath(IUnit* source, std::vector<Vec3> const& path)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnNewPath");
		if (func.isFunction())
		{
			func(source, path);
		}
	}
}

PLUGIN_EVENT(void) OnTeleport(OnTeleportArgs* args)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnTeleport");
		if (func.isFunction())
		{
			func(args);
		}
	}
}

PLUGIN_API void OnLoad(IPluginSDK* PluginSDK)
{
	PluginSDKSetup(PluginSDK);
	if (!(fs::exists(GPluginSDK->GetDirectory() + "\\LuaPlusPlus")) || !(fs::exists(GPluginSDK->GetDirectory() + "\\LuaPlusPlus\\Scripts")) || !(fs::exists(GPluginSDK->GetDirectory() + "\\LuaPlusPlus\\Scripts\\Common")))
	{
		fs::create_directory(GPluginSDK->GetDirectory() + "\\LuaPlusPlus");
		fs::create_directory(GPluginSDK->GetDirectory() + "\\LuaPlusPlus\\Scripts");
		fs::create_directory(GPluginSDK->GetDirectory() + "\\LuaPlusPlus\\Scripts\\Common");
		GGame->PrintChat("[Lua++] Created Directories");
	}
	Menu = GPluginSDK->AddMenu("Lua++");
	ScriptsMenu = Menu->AddMenu("Scripts");
	for (fs::directory_iterator i = fs::directory_iterator(fs::path(GPluginSDK->GetDirectory()+"\\LuaPlusPlus\\Scripts")); i != fs::directory_iterator(); i++)
	{
		if (!fs::is_directory(i->path()) && i->path().filename().string().find_last_of(".lua") != std::string::npos)
		{
			std::string str = i->path().filename().string();
			ScriptsMenu->CheckBox(str.substr(0, str.find_last_of(".lua")-3).c_str(), false);
			Engine::Scripts.push_back(str.substr(0, str.find_last_of(".lua")-3));
		}
	}
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnBuffAdd, OnBuffAdd);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnBuffRemove, OnBuffRemove);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnCreateObject, OnCreateObj);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnDash, OnDash);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnDestroyObject, OnDeleteObj);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnDoCast, OnDoCast);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnEnterVisible, OnVisionLose);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnExitVisible, OnVisionGain);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnGameEnd, OnGameEnd);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnGameUpdate, OnGameUpdate);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnGapCloser, OnGapCloser);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnInterruptible, OnInterruptible);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnIssueOrder, OnIssueOrder);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnJungleNotification, OnJungleNotification);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnLevelUp, OnLevelUp);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnNewPath, OnNewPath);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnPauseAnimation, OnPauseAnimation);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnPlayAnimation, OnPlayAnimation);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnPreCast, OnSpellCast);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnRender, OnRender);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnRenderBehindHud, OnHudRender);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnSpellCast, OnProcessSpell);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnTeleport, OnTeleport);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnUnitDeath, OnDeath);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnUpdateChargedSpell, OnUpdateChargedSpell);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnWndProc, OnWndProc);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOrbwalkAfterAttack, OnAfterAttack);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOrbwalkOnAttack, OnAttack);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOrbwalkNonKillableMinion, OnUnkillableMinion);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOrbwalkTargetChange, OnTargetChange);
}

PLUGIN_API void OnUnload()
{
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnBuffAdd, OnBuffAdd);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnBuffRemove, OnBuffRemove);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnCreateObject, OnCreateObj);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnDash, OnDash);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnDestroyObject, OnDeleteObj);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnDoCast, OnDoCast);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnEnterVisible, OnVisionLose);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnExitVisible, OnVisionGain);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnGameEnd, OnGameEnd);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnGameUpdate, OnGameUpdate);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnGapCloser, OnGapCloser);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnInterruptible, OnInterruptible);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnIssueOrder, OnIssueOrder);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnJungleNotification, OnJungleNotification);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnLevelUp, OnLevelUp);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnNewPath, OnNewPath);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnPauseAnimation, OnPauseAnimation);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnPlayAnimation, OnPlayAnimation);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnPreCast, OnSpellCast);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnRender, OnRender);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnRenderBehindHud, OnHudRender);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnSpellCast, OnProcessSpell);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnTeleport, OnTeleport);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnUnitDeath, OnDeath);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnUpdateChargedSpell, OnUpdateChargedSpell);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnWndProc, OnWndProc);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOrbwalkAfterAttack, OnAfterAttack);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOrbwalkOnAttack, OnAttack);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOrbwalkNonKillableMinion, OnUnkillableMinion);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOrbwalkTargetChange, OnTargetChange);

	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "OnUnload");
		if (func.isFunction())
		{
			func();
		}
	}

	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		lua_close((*it)->State);
	}
	Menu->Remove();
	Engine::Instances.clear();
	Engine::Scripts.clear();
}
