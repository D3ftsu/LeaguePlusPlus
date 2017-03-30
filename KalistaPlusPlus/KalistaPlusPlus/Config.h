#pragma once

#ifndef _INCL_GUARD_CONFIG_H
#define _INCL_GUARD_CONFIG_H

#include <PluginSDK.h>

static class Config
{
private:
	static IMenu* MainMenu;
	static IMenu* ComboMenu;
	static IMenu* HarassMenu;
	static IMenu* KillstealMenu;
	static IMenu* MiscMenu;
	static IMenu* LaneClearMenu;
	static IMenu* JungleClearMenu;
	static IMenu* DrawingsMenu;
	static IMenu* KeysMenu;
	static IMenu* SkinsMenu;

public:
	static void Initialize();
	static void Unload();

	static class Combo
	{
	public:
		static bool Q();
		static bool E();
		static bool ESlow();
	};
	static class Harass
	{
	public:
		static bool Q();
		static float Mana();
	};
	static class Killsteal
	{
	public:
		static bool Q();
		static bool E();
	};
	static class Misc
	{
	public:
		static bool ECannon();
		static bool EDeath();
		static bool R();
		static bool BalistaCombo();
	};
	static class LaneClear
	{
	public:
		static bool E();
		static int MinKillableE();
		static float Mana();
	};
	static class JungleClear
	{
	public:
		static bool E();
		static bool NoE();
		static int ExecuteType();
	};
	static class Drawings
	{
	public:
		static bool Q();
		static bool W();
		static bool E();
		static bool R();
		static bool Damage();
	};
	static class Keys
	{
	public:
		static bool Sentinel();
	};
};

#endif