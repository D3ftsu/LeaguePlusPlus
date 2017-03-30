#include "Config.h"

IMenu* Config::MainMenu;
IMenu* Config::ComboMenu;
IMenu* Config::HarassMenu;
IMenu* Config::KillstealMenu;
IMenu* Config::MiscMenu;
IMenu* Config::LaneClearMenu;
IMenu* Config::JungleClearMenu;
IMenu* Config::DrawingsMenu;
IMenu* Config::KeysMenu;
IMenu* Config::SkinsMenu;

void Config::Initialize()
{
	MainMenu = GPluginSDK->AddMenu("KalistaPlusPlus - Deftsu");

	ComboMenu = MainMenu->AddMenu("Combo");
	ComboMenu->CheckBox("Use Q", true);
	ComboMenu->CheckBox("Use E", true);
	ComboMenu->CheckBox("Kill minions with E to slow enemy", false);

	HarassMenu = MainMenu->AddMenu("Harass");
	HarassMenu->CheckBox("Use Q", true);
	HarassMenu->CheckBox("Kill minions with E to slow enemy", true);
	HarassMenu->AddFloat("Harass Min. Mana %", 0.0f, 80.0f, 30.0f);

	KillstealMenu = MainMenu->AddMenu("Killsteal");
	KillstealMenu->CheckBox("Killsteal with Q", true);
	KillstealMenu->CheckBox("Killsteal with E", true);

	MiscMenu = MainMenu->AddMenu("Miscellaneous");
	MiscMenu->CheckBox("Cast E for cannons LastHit", true);
	MiscMenu->CheckBox("Cast E before death", true);
	MiscMenu->CheckBox("Cast E if enemy is leaving range", true);
	MiscMenu->AddInteger("Min. E Stacks", 1, 20, 7);
	MiscMenu->CheckBox("Use R to save your Soulbound", true);
	for (IUnit* ally : GEntityList->GetAllHeros(true, false))
	{
		if (ally->ChampionName() == "Blitzcrank")
		{
			MiscMenu->CheckBox("Balista Combo", true);
			break;
		}
	}

	LaneClearMenu = MainMenu->AddMenu("LaneClear");
	LaneClearMenu->CheckBox("Use E", true);
	LaneClearMenu->AddInteger("Min. killable minions with E", 1, 10, 2);
	LaneClearMenu->AddFloat("LaneClear Min. Mana %", 0.f, 80.f, 30.f);

	JungleClearMenu = MainMenu->AddMenu("JungleClear");
	JungleClearMenu->CheckBox("Don't cast E in jungle for first 2 mins", true);
	JungleClearMenu->AddSelection("Jungle Execute:", 2, std::vector<std::string>{"OFF", "Epic Only", "Large & Epic Only", "Everything"});
	JungleClearMenu->CheckBox("Use E", true);

	DrawingsMenu = MainMenu->AddMenu("Drawings");
	DrawingsMenu->CheckBox("Draw Q Range", true);
	DrawingsMenu->CheckBox("Draw W Range", false);
	DrawingsMenu->CheckBox("Draw E Range", true);
	DrawingsMenu->CheckBox("Draw R Range", false);
	DrawingsMenu->CheckBox("Draw Damage", true);

	KeysMenu = MainMenu->AddMenu("Key Settings");
	KeysMenu->AddKey("Cast sentinel bug", 84); // T

	SkinsMenu = MainMenu->AddMenu("Skin Manager");
	SkinsMenu->AddSelection("Select A Skin:", 0, std::vector<std::string>{"Classic", "Blood Moon", "Championship"}); // TODO waiting for callback option
}

void Config::Unload()
{
	if (MainMenu != nullptr)
		MainMenu->Remove();
}

bool Config::Combo::Q()
{
	return ComboMenu->GetOption(" Q")->Enabled();
}

bool Config::Combo::E()
{
	return ComboMenu->GetOption("Use E")->Enabled();
}

bool Config::Combo::ESlow()
{
	return ComboMenu->GetOption("Kill minions with E to slow enemy")->Enabled();
}

bool Config::Harass::Q()
{
	return HarassMenu->GetOption("Use Q")->Enabled();
}

float Config::Harass::Mana()
{
	return HarassMenu->GetOption("Harass Min. Mana %")->GetFloat();
}

bool Config::Killsteal::Q()
{
	return KillstealMenu->GetOption("Killsteal with Q")->Enabled();
}

bool Config::Killsteal::E()
{
	return KillstealMenu->GetOption("Killsteal with E")->Enabled();
}

bool Config::Misc::ECannon()
{
	return MiscMenu->GetOption("Cast E for cannons LastHit")->Enabled();
}

bool Config::Misc::EDeath()
{
	return MiscMenu->GetOption("Cast E before death")->Enabled();
}

bool Config::Misc::R()
{
	return MiscMenu->GetOption("Use R to save your Soulbound")->Enabled();
}

bool Config::Misc::BalistaCombo()
{	
	if (MiscMenu->GetOption("Balista Combo") == nullptr)
		return false;
	return MiscMenu->GetOption("Balista Combo")->Enabled();
}

bool Config::LaneClear::E()
{
	return LaneClearMenu->GetOption("Use E")->Enabled();
}

int Config::LaneClear::MinKillableE()
{
	return LaneClearMenu->GetOption("Min. killable minions with E")->GetInteger();
}

float Config::LaneClear::Mana()
{
	return LaneClearMenu->GetOption("LaneClear Min. Mana %")->GetFloat();
}

bool Config::JungleClear::E()
{
	return JungleClearMenu->GetOption("Use E")->Enabled();
}

bool Config::JungleClear::NoE()
{
	return JungleClearMenu->GetOption("Don't cast E in jungle for first 2 mins")->Enabled();
}

int Config::JungleClear::ExecuteType()
{
	return JungleClearMenu->GetOption("Jungle Execute:")->GetInteger();
}

bool Config::Drawings::Q()
{
	return DrawingsMenu->GetOption("Draw Q Range")->Enabled();
}

bool Config::Drawings::W()
{
	return DrawingsMenu->GetOption("Draw W Range")->Enabled();
}

bool Config::Drawings::E()
{
	return DrawingsMenu->GetOption("Draw E Range")->Enabled();
}

bool Config::Drawings::R()
{
	return DrawingsMenu->GetOption("Draw R Range")->Enabled();
}

bool Config::Drawings::Damage()
{
	return DrawingsMenu->GetOption("Draw Damage")->Enabled();
}

bool Config::Keys::Sentinel()
{
	return GetAsyncKeyState(KeysMenu->GetOption("Cast sentinel bug")->GetInteger()) && GPluginSDK->GetUtility()->IsLeagueWindowFocused();
}