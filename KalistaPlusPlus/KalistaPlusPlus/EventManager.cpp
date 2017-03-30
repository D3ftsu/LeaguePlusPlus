#include "EventManager.h"
#include "SpellManager.h"
#include <string>

IUnit* Player;
IUnit* SoulBound;
float LastR;

float GetDistance(Vec3 v1, Vec3 v2)
{	
	return sqrt(pow(v1.x-v2.x, 2.f) + pow(v1.z-v2.z, 2.f));
}

float GetDistance(IUnit* v1, IUnit* v2)
{
	return GetDistance(v1->GetPosition(), v2->GetPosition());
}

float GetDistance(Vec3 v1)
{
	Vec3 v2(Player->GetPosition());
	return sqrt(pow(v1.x - v2.x, 2.f) + pow(v1.z - v2.z, 2.f));
}

float GetDistance(IUnit* v1)
{
	return GetDistance(v1->GetPosition());
}

int CountEnemyChampionsInRange(Vec3 pos, float range)
{
	int count = 0;
	for (IUnit* enemy : GEntityList->GetAllHeros(false, true))
	{
		if (enemy->IsValidTarget(Player, range))
			count++;
	}
	return count;
}

double GetEDmg(IUnit* target)
{
	int count = GBuffData->GetStacks(target->GetBuffDataByName("kalistaexpungemarker"));
	int level = Player->GetSpellBook()->GetLevel(eSpellSlot::kSlotE);
	if (level == 0 || count == 0)
		return 0;
	return GDamage->CalcPhysicalDamage(Player, target, (double)((new float[5]{20, 30, 40, 50, 60})[level - 1] + 0.6 * (Player->TotalPhysicalDamage())) + ((count - 1) * ((new float[5]{10, 14, 19, 25, 32})[level - 1] + (new float[5]{0.2f, 0.225f, 0.25f, 0.275f, 0.3f})[level - 1] * (Player->TotalPhysicalDamage()))));
}

PLUGIN_EVENT(void) OnRender()
{
	if (Config::Drawings::Q()) GPluginSDK->GetRenderer()->DrawOutlinedCircle(Player->GetPosition(), Vec4(205, 92, 92, 150), SpellManager::Q->Range());
	if (Config::Drawings::W()) GPluginSDK->GetRenderer()->DrawOutlinedCircle(Player->GetPosition(), Vec4(147, 112, 219, 150), SpellManager::W->Range());
	if (Config::Drawings::E()) GPluginSDK->GetRenderer()->DrawOutlinedCircle(Player->GetPosition(), Vec4(0, 150, 0, 150), SpellManager::E->Range());
	if (Config::Drawings::R()) GPluginSDK->GetRenderer()->DrawOutlinedCircle(Player->GetPosition(), Vec4(255, 0, 0, 150), SpellManager::R->Range());
	if (Config::Drawings::Damage())
	{
		for (IUnit* enemy : GEntityList->GetAllHeros(false, true))
		{
			if (!enemy->IsDead() && enemy->IsVisible() && enemy->HasBuff("kalistaexpungemarker"))
			{
				double damage = GetEDmg(enemy);
				Vec2 drawPos;
				enemy->GetHPBarPosition(drawPos);
				drawPos.x = drawPos.x + 150;
				drawPos.y = drawPos.y + 15;
				GPluginSDK->GetRenderer()->DrawTextW(drawPos, Vec4(255, 255, 255, 255), "%s", (damage > enemy->GetHealth() ? "100%" : std::to_string((int)(100.f * damage / enemy->GetHealth())).append("%").c_str()));
			}
		}

		for (IUnit* mob : GEntityList->GetAllMinions(false, false, true))
		{
			if (!mob->IsDead() && mob->IsVisible() && mob->HasBuff("kalistaexpungemarker"))
			{
				double damage = GetEDmg(mob);
				Vec2 drawPos;
				GGame->Projection(mob->GetPosition(), &drawPos);;
				GPluginSDK->GetRenderer()->DrawTextW(drawPos, Vec4(255, 255, 255, 255), "%s", (damage > mob->GetHealth() ? "100%" : std::to_string((int)(100.f * damage / mob->GetHealth())).append("%").c_str()));
			}
		}
	}
}

PLUGIN_EVENT(void) OnBuffAdd(IUnit* unit, void* BuffData)
{
	if (GBuffData->GetCaster(BuffData) == Player && GBuffData->GetBuffName(BuffData) == "kalistacoopstrikeally")
	{
		SoulBound = unit;
	}
}

PLUGIN_EVENT(void) OnGameUpdate()
{
	switch (GOrbwalking->GetOrbwalkingMode()) 
	{
		case eOrbwalkingMode::kModeCombo :
			EventManager::Combo();
			break;
		case eOrbwalkingMode::kModeMixed :
			EventManager::Harass();
			break;
		case eOrbwalkingMode::kModeLaneClear :
			EventManager::Clear();
			break;
	}
	EventManager::PermaActive();
}

Vec3 SentinelP1(9882.892f, -71.24f, 4438.446f);
Vec3 SentinelP2(5087.77f, -71.24f, 10471.3808f);

void EventManager::PermaActive()
{
	if (SpellManager::E->IsReady() && Config::Misc::EDeath() && Player->HealthPercent() < (Player->GetLevel() < 6 ? 3.f : 5.f)) // E before death
		SpellManager::E->CastOnPlayer();

	if (SpellManager::W->IsReady() && Config::Keys::Sentinel()) // Sentinel bug
	{
		if (GetDistance(SentinelP1) < GetDistance(SentinelP2) && GetDistance(SentinelP1) < SpellManager::W->Range())
			SpellManager::W->CastOnPosition(SentinelP1);
		else if (GetDistance(SentinelP2) < SpellManager::W->Range())
			SpellManager::W->CastOnPosition(SentinelP2);
	}

	if (SoulBound != nullptr && SpellManager::R->IsReady() && Config::Misc::R() && !Player->IsRecalling()) // Auto R
	{
		if (!SoulBound->IsDead() && CountEnemyChampionsInRange(SoulBound->GetPosition(), 850.f) > 0 && SoulBound->HealthPercent() < 1.75f * (1 - (SoulBound->Armor() / 400 + SoulBound->MagicResist() / 400) / 2) * 1.2 * (1 + 0.35 * (CountEnemyChampionsInRange(SoulBound->GetPosition(), 850.f) - 1)) && LastR < GGame->Time() - 5.f)
		{
			if (SpellManager::R->CastOnUnit(SoulBound))
				LastR = GGame->Time();
		}
	}

	if (SoulBound != nullptr && SpellManager::R->IsReady() && Config::Misc::BalistaCombo() && Player->GetHealth() > 400 && SoulBound->ChampionName() == "Blitzcrank") // Balista
	{
		for (IUnit* enemy : GEntityList->GetAllHeros(false, true))
		{
			if (Player->IsValidTarget(enemy, 2450.f) && enemy->GetHealth() > 400.f && GetDistance(SoulBound, enemy) > 400.f && GetDistance(enemy) > 400.f && GetDistance(enemy) > GetDistance(enemy, SoulBound) + 100.f && enemy->HasBuff("rocketgrab2"))
			{
				SpellManager::R->CastOnUnit(SoulBound);
				break;
			}
		}
	}
	
	if (SpellManager::E->IsReady() && Config::Misc::ECannon())
	{
		for (IUnit* minion : GEntityList->GetAllMinions(false, true, false))
		{
			if (!minion->IsDead() && (std::string(minion->GetBaseSkinName()).find("Super") != std::string::npos || std::string(minion->GetBaseSkinName()).find("Siege") != std::string::npos) && GetDistance(minion) < SpellManager::E->Range() && GetEDmg(minion) > minion->GetHealth())
			{
				SpellManager::E->CastOnPlayer();
				break;
			}
		}
	}

	for (IUnit* enemy : GEntityList->GetAllHeros(false, true))
	{
		if (SpellManager::E->IsReady() && Config::Killsteal::E() && Player->IsValidTarget(enemy, 1100.f + enemy->BoundingRadius()) && GetEDmg(enemy) > enemy->GetHealth())
		{
			SpellManager::E->CastOnPlayer();
			break;
		}
		if (SpellManager::Q->IsReady() && Config::Killsteal::Q() && Player->IsValidTarget(enemy, SpellManager::Q->Range()) && GDamage->GetSpellDamage(Player, enemy, eSpellSlot::kSlotQ) > enemy->GetHealth())
		{
			SpellManager::Q->CastOnTarget(enemy, ePredictionChance::kHitChanceHigh);
			break;
		}
	}
}

void EventManager::Combo()
{	
	IUnit* Target = GTargetSelector->FindTarget(eTargetPriority::QuickestKill, eDamageType::PhysicalDamage, SpellManager::Q->Range());
	if (Target != nullptr)
	{
		if (SpellManager::Q->IsReady() && Config::Combo::Q() && GetDistance(Target) > Player->GetRealAutoAttackRange(Target) && Player->GetMana() > Player->GetSpellBook()->GetManaCost(eSpellSlot::kSlotQ)+Player->GetSpellBook()->GetManaCost(eSpellSlot::kSlotE))
		{
			SpellManager::Q->CastOnTarget(Target, ePredictionChance::kHitChanceHigh);
			return;
		}
		if (SpellManager::E->IsReady())
		{
			if (Config::Combo::E() && GetDistance(Target) < SpellManager::E->Range() + Target->BoundingRadius() && GetEDmg(Target) > Target->GetHealth())
			{
				SpellManager::E->CastOnPlayer();
				return;
			}
			if (Config::Combo::ESlow())
			{
				for (IUnit* minion : GEntityList->GetAllMinions(false, true, true))
				{
					if (!minion->IsDead() && GetDistance(minion) < SpellManager::E->Range() + minion->BoundingRadius() && GetDistance(Target) > Player->GetRealAutoAttackRange(Target) && Target->HasBuff("kalistaexpungemarker") && GetEDmg(minion) > minion->GetHealth())
					{
						SpellManager::E->CastOnPlayer();
						break;
					}
				}
			}
		}
	}
}

void EventManager::Harass()
{
	if (Player->ManaPercent() < Config::Harass::Mana() || !SpellManager::Q->IsReady() || !Config::Harass::Q())
		return;
	IUnit* Target = GTargetSelector->FindTarget(eTargetPriority::QuickestKill, eDamageType::PhysicalDamage, SpellManager::Q->Range());
	if (Target != nullptr)
		SpellManager::Q->CastOnTarget(Target, ePredictionChance::kHitChanceHigh);
}

std::vector<const char*> Epics = {"SRU_Baron", "SRU_RiftHerald", "sru_dragon_water", "sru_dragon_fire", "sru_dragon_earth", "sru_dragon_air", "sru_dragon_elder", "TT_Spiderboss"};
std::vector<const char*> Mobs = {"SRU_Baron", "SRU_RiftHerald", "sru_dragon_water", "sru_dragon_fire", "sru_dragon_earth", "sru_dragon_air", "sru_dragon_elder", "SRU_Red", "SRU_Blue", "SRU_Krug", "SRU_Murkwolf", "SRU_Razorbeak", "SRU_Gromp", "Sru_Crab", "TT_Spiderboss"};

void EventManager::Clear()
{
	if (!SpellManager::E->IsReady())
		return;
	int KillableMinions;
	if (Player->ManaPercent() < Config::LaneClear::Mana() && Config::LaneClear::E())
	{
		for (IUnit* minion : GEntityList->GetAllMinions(false, true, false))
		{
			if (!minion->IsDead() && GetEDmg(minion) > minion->GetHealth() && GetDistance(minion) < SpellManager::E->Range() + minion->BoundingRadius())
				KillableMinions++;
		}
		if (KillableMinions > Config::LaneClear::MinKillableE())
		{
			SpellManager::E->CastOnPlayer();
			return;
		}
	}
	if (Config::JungleClear::ExecuteType() != 0 && Config::JungleClear::E() && (!Config::JungleClear::NoE() || GGame->Time() > 130))
	{
		for (IUnit* mob : GEntityList->GetAllMinions(false, false, true))
		{
			if (!mob->IsDead() && GetEDmg(mob) > mob->GetHealth() && GetDistance(mob) < SpellManager::E->Range() + mob->BoundingRadius())
			{
				switch (Config::JungleClear::ExecuteType())
				{
					case 1:
						for (const char* Epic : Epics)
						{
							if (strcmp(Epic, mob->GetBaseSkinName()) == 0)
							{
								SpellManager::E->CastOnPlayer();
								break;
							}
						}
						break;
					case 2:
						for (const char* Mob : Mobs)
						{
							if (strcmp(Mob, mob->GetBaseSkinName()) == 0)
							{
								SpellManager::E->CastOnPlayer();
								break;
							}
						}
						break;
					case 3:
						{
							SpellManager::E->CastOnPlayer();
							break;
						}
				}
			}
		}
	}
}

void EventManager::Initialize()
{
	Player = GEntityList->Player();
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnRenderBehindHud, OnRender);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnGameUpdate, OnGameUpdate);
	GEventManager->AddEventHandler(eLeagueEvents::kEventOnBuffAdd, OnBuffAdd);
}

void EventManager::Unload()
{
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnRenderBehindHud, OnRender);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnGameUpdate, OnGameUpdate);
	GEventManager->RemoveEventHandler(eLeagueEvents::kEventOnBuffAdd, OnBuffAdd);
}