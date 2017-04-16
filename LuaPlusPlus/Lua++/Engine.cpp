#include "Script.h"
#include "Engine.h"
#include <PluginSDK.h>
#include <algorithm>

std::vector<Script*> Engine::Instances;
std::vector<std::string> Engine::Scripts;

namespace LuaIntf
{
	LUA_USING_LIST_TYPE(std::vector)
}

#define MULTI_LINE_STRING(...) #__VA_ARGS__

void Engine::Init(lua_State *L)
{
	using namespace LuaIntf;
	LuaBinding(L)
		.beginClass<Vec2>("Vector2")
			.addConstructor(LUA_ARGS(float, float))
			.addFunction("__tostring", &Vec2::ToString)
			.addFunction("__type", &Vec2::Type)
			.addFunction("__eq", &Vec2::Compare)
			.addFunction("__add", &Vec2::operator+)
			.addFunction("__sub", (Vec2(Vec2::*)(const Vec2&) const)&Vec2::operator-)
			.addFunction("__unm", (Vec2(Vec2::*)() const)&Vec2::operator-)
			.addFunction("__mul", (Vec2(Vec2::*)(const float) const)&Vec2::operator*)
			.addFunction("__div", (Vec2(Vec2::*)(const float) const)&Vec2::operator/)
			.addFunction("Set", &Vec2::Set)
			.addFunction("Zero", &Vec2::Zero)
			.addFunction("Normalize", &Vec2::VectorNormalize)
			.addFunction("Extend", static_cast<Vec2(Vec2::*)(Vec2 const, float)>(&Vec2::Extend))
			.addFunction("Length", &Vec2::Length)
			.addFunction("DotProduct", &Vec2::DotProduct)
			.addVariable("x", &Vec2::x)
			.addVariable("y", &Vec2::y)
		.endClass()

		.beginClass<Vec3>("Vector3")
			.addConstructor(LUA_ARGS(float, float, float))
			.addFunction("__tostring", &Vec3::ToString)
			.addFunction("__type", &Vec3::Type)
			.addFunction("__eq", &Vec3::Compare)
			.addFunction("__add", &Vec3::operator+)
			.addFunction("__sub", (Vec3(Vec3::*)(const Vec3&) const)&Vec3::operator-)
			.addFunction("__unm", (Vec3(Vec3::*)() const)&Vec3::operator-)
			.addFunction("__mul", (Vec3(Vec3::*)(const float) const)&Vec3::operator*)
			.addFunction("__div", (Vec3(Vec3::*)(const float) const)&Vec3::operator/)
			.addFunction("Set", &Vec3::Set)
			.addFunction("Zero", &Vec3::Zero)
			.addFunction("Normalize", &Vec3::VectorNormalize)
			.addFunction("Extend", static_cast<Vec3(Vec3::*)(Vec3 const, float)>(&Vec3::Extend))
			.addFunction("Length", &Vec3::Length)
			.addFunction("DotProduct", &Vec3::DotProduct)
			.addVariable("x", &Vec3::x)
			.addVariable("y", &Vec3::y)
			.addVariable("z", &Vec3::z)
		.endClass()

		.beginClass<Vec4>("Vector4")
			.addConstructor(LUA_ARGS(float, float, float, float))
			.addFunction("__tostring", &Vec4::ToString)
			.addFunction("__type", &Vec4::Type)
			.addFunction("__eq", &Vec4::Compare)
			.addFunction("__add", &Vec4::operator+)
			.addFunction("__sub", (Vec4(Vec4::*)(const Vec4&) const)&Vec4::operator-)
			.addFunction("__unm", (Vec4(Vec4::*)() const)&Vec4::operator-)
			.addFunction("__mul", (Vec4(Vec4::*)(const float) const)&Vec4::operator*)
			.addFunction("__div", (Vec4(Vec4::*)(const float) const)&Vec4::operator/)
			.addFunction("Set", &Vec4::Set)
			.addFunction("Zero", &Vec4::Zero)
			.addFunction("Normalize", &Vec4::VectorNormalize)
			.addFunction("Negate", &Vec4::Negate)
			.addFunction("Length", &Vec4::Length)
			.addFunction("DotProduct", static_cast<float(Vec4::*)(const Vec4&)>(&Vec4::DotProduct))
			.addVariable("x", &Vec4::x)
			.addVariable("y", &Vec4::y)
			.addVariable("z", &Vec4::z)
			.addVariable("w", &Vec4::w)
		.endClass()

		.beginClass<NavigationPath>("NavigationPath")
			.addVariable("index", &NavigationPath::CurrentWaypoint_)
			.addVariable("startPos", &NavigationPath::StartPosition_)
			.addVariable("endPos", &NavigationPath::EndPosition)
			.addVariable("waypointStart", &NavigationPath::WaypointStart_)
			.addVariable("waypointEnd", &NavigationPath::WaypointStart_)
		.endClass()

		.beginClass<ItemData>("ItemData")
			.addVariable("id", &ItemData::Id_)
			.addVariableRef("name", &ItemData::Name_)
		.endClass()

		.beginClass<UnitDash>("DashData")
			.addVariable("source", &UnitDash::Source)
			.addVariable("startPos", &UnitDash::StartPosition)
			.addVariable("endPos", &UnitDash::EndPosition)
			.addVariable("startTick", &UnitDash::StartTick)
			.addVariable("endTick", &UnitDash::EndTick)
			.addVariable("duration", &UnitDash::Duration)
			.addVariable("speed", &UnitDash::Speed)
		.endClass()

		.beginClass<CastedSpell>("SpellInfo")
			.addVariable("source", &CastedSpell::Caster_)
			.addVariable("target", &CastedSpell::Target_)
			.addVariable("isAutoAttack", &CastedSpell::AutoAttack_)
			.addVariable("SData", &CastedSpell::Data_)
			.addVariableRef("name", &CastedSpell::Name_)
			.addVariable("windUpTime", &CastedSpell::Windup_)
			.addVariable("animationTime", &CastedSpell::Animation_)
			.addVariable("startPos", &CastedSpell::Position_)
			.addVariable("endPos", &CastedSpell::EndPosition_)
			.addVariable("radius", &CastedSpell::Radius_)
			.addVariable("speed", &CastedSpell::Speed_)
			.addVariable("range", &CastedSpell::Range_)
			.addVariable("damage", &CastedSpell::Damage_)
		.endClass()

		.beginClass<InterruptibleSpell>("IInterruptibleSpell")
			.addVariable("source", &InterruptibleSpell::Source)
			.addVariable("dangerLevel", &InterruptibleSpell::DangerLevel)
			.addVariable("endTime", &InterruptibleSpell::EndTime)
			.addVariable("movementInterupts", &InterruptibleSpell::MovementInterupts)
			.addVariable("SData", &InterruptibleSpell::Data)
		.endClass()

		.beginClass<GapCloserSpell>("IGapCloserSpell")
			.addVariable("source", &GapCloserSpell::Source)
			.addVariable("startPos", &GapCloserSpell::StartPosition)
			.addVariable("endPos", &GapCloserSpell::EndPosition)
			.addVariable("isTargeted", &GapCloserSpell::IsTargeted)
			.addVariable("startTick", &GapCloserSpell::StartTick)
			.addVariable("slot", &GapCloserSpell::Slot)
			.addVariable("SData", &GapCloserSpell::Data)
		.endClass()

		.beginClass<OnTeleportArgs>("IOnTeleportArgs")
			.addVariable("source", &OnTeleportArgs::Source)
			.addVariable("type", &OnTeleportArgs::Type)
			.addVariable("status", &OnTeleportArgs::Status)
			.addVariable("duration", &OnTeleportArgs::Duration)
		.endClass()

		.beginClass<JungleNotifyData>("IJungleNotifyData")
			.addVariable("pos", &JungleNotifyData::Position)
		.endClass()

		.beginClass<HeroMastery>("IHeroMastery")
			.addVariable("pageId", &HeroMastery::PageId)
			.addVariable("masteryId", &HeroMastery::MasteryId)
			.addVariable("points", &HeroMastery::Points)
		.endClass()

		.beginClass<LastCastedSpellArgs>("SpellInfoArgs")
			.addVariable("data", &LastCastedSpellArgs::Data)
			.addVariable("tick", &LastCastedSpellArgs::Tick)
		.endClass()

		.beginClass<ISpellBook>("ISpellBook")
			.addPropertyReadOnly("isCharging", &ISpellBook::IsCharging)
			.addPropertyReadOnly("isChanneling", &ISpellBook::IsChanneling)
			.addPropertyReadOnly("castEndTime", &ISpellBook::GetCastEndTime)
			.addPropertyReadOnly("castTime", &ISpellBook::GetCastTime)
		.endClass()

		.beginClass<ISpellData>("ISpellData")
			.addFunction("GetCaster", &ISpellData::GetCaster)
			.addFunction("GetTarget", &ISpellData::GetTarget)
			.addFunction("IsAutoAttack", &ISpellData::IsAutoAttack)
			.addFunction("GetSpellName", &ISpellData::GetSpellName)
			.addFunction("GetWindupTime", &ISpellData::GetWindupTime)
			.addFunction("GetAnimationDelay", &ISpellData::GetAnimationDelay)
			.addFunction("GetStartPosition", &ISpellData::GetStartPosition)
			.addFunction("GetEndPosition", &ISpellData::GetEndPosition)
			.addFunction("CastConeAngle", &ISpellData::CastConeAngle)
			.addFunction("ManaCost", &ISpellData::ManaCost)
			.addFunction("GetRadius", &ISpellData::GetRadius)
			.addFunction("GetSpeed", &ISpellData::GetSpeed)
			.addFunction("GetBaseDamage", &ISpellData::GetBaseDamage)
			.addFunction("GetRange", &ISpellData::GetRange)
			.addFunction("GetCastTime", &ISpellData::SpellCastTime)
			.addFunction("GetSlot", &ISpellData::GetSlot)
			.addFunction("GetLevel", &ISpellData::GetLevel)
		.endClass()

		.beginClass<IBuffData>("IBuffData")
			.addFunction("IsValid", &IBuffData::IsValid)
			.addFunction("GetStacks", &IBuffData::GetStacks)
			.addFunction("GetCaster", &IBuffData::GetCaster)
			.addFunction("GetSourceName", &IBuffData::GetSourceName)
			.addFunction("GetBuffName", &IBuffData::GetBuffName)
			.addFunction("GetStartTime", &IBuffData::GetStartTime)
			.addFunction("GetEndTime", &IBuffData::GetEndTime)
			.addFunction("IsActive", &IBuffData::IsActive)
			.addFunction("GetBuffType", &IBuffData::GetBuffType)
		.endClass()

		.beginClass<IMissileData>("IMissileData")
			.addFunction("GetSlot", &IMissileData::GetSpellSlot)
			.addFunction("GetTarget", &IMissileData::GetTarget)
			.addFunction("GetCaster", &IMissileData::GetCaster)
			.addFunction("GetCastConeAngle", &IMissileData::GetCastConeAngle)
			.addFunction("GetStartPosition", &IMissileData::GetStartPosition)
			.addFunction("GetEndPosition", &IMissileData::GetEndPosition)
			.addFunction("GetName", &IMissileData::GetName)
			.addFunction("IsAutoAttack", &IMissileData::IsAutoAttack)
			.addFunction("GetRadius", &IMissileData::GetRadius)
			.addFunction("GetSpeed", &IMissileData::GetSpeed)
			.addFunction("GetBaseDamage", &IMissileData::GetBaseDamage)
			.addFunction("GetRange", &IMissileData::GetRange)
			.addFunction("GetCastTime", &IMissileData::GetSpellCastTime)
			.addFunction("GetSlot", &IMissileData::GetSpellSlot)
		.endClass()

		.beginClass<INavMesh>("INavMesh")
			.addFunction("GetCollisionFlags", &INavMesh::GetCollisionFlagsForPoint)
			.addFunction("IsWall", &INavMesh::IsPointWall)
			.addFunction("IsGrass", &INavMesh::IsPointGrass)
			.addFunction("GetHeight", &INavMesh::GetHeightForPoint)
		.endClass()

		.beginClass<IUnit>("Unit")
			.addFunction("__type", &IUnit::Type)
			.addPropertyReadOnly("dashData", &IUnit::DashData)
			.addPropertyReadOnly("isOnScreen", &IUnit::IsOnScreen)
			.addPropertyReadOnly("hpBarRendered", &IUnit::IsHpBarBeingRendered)
			.addPropertyReadOnly("cdr", &IUnit::GetCooldownReductionPercent)
			.addPropertyReadOnly("attackSpeed", &IUnit::AttackSpeed)
			.addPropertyReadOnly("ms", &IUnit::MovementSpeed)
			.addPropertyReadOnly("flatArmorReduction", &IUnit::ArmorReductionFlat)
			.addPropertyReadOnly("flatMagicReduction", &IUnit::MagicReductionFlat)
			.addPropertyReadOnly("percentArmorReduction", &IUnit::ArmorReductionPercent)
			.addPropertyReadOnly("percentMagicReduction", &IUnit::MagicReductionPercent)
			.addPropertyReadOnly("armorPenPercent", &IUnit::ArmorPenetrationPercent)
			.addPropertyReadOnly("bonusArmorPenPercent", &IUnit::BonusArmorPenetrationPercent)
			.addPropertyReadOnly("magicPenPercent", &IUnit::MagicPenetrationPercent)
			.addPropertyReadOnly("armorPen", &IUnit::ArmorPenetrationFlat)
			.addPropertyReadOnly("magicPen", &IUnit::MagicPenetrationFlat)
			.addPropertyReadOnly("percentMagicDamageMod", &IUnit::PercentMagicDamageMod)
			.addPropertyReadOnly("armor", &IUnit::Armor)
			.addPropertyReadOnly("bonusArmor", &IUnit::GetBonusArmor)
			.addPropertyReadOnly("magicResist", &IUnit::MagicResist)
			.addPropertyReadOnly("physicalDamage", &IUnit::PhysicalDamage)
			.addPropertyReadOnly("physicalBonusDamage", &IUnit::BonusDamage)
			.addPropertyReadOnly("physicalDamageMod", &IUnit::PhysicalDamageMod)
			.addPropertyReadOnly("ap", &IUnit::MagicDamage)
			.addPropertyReadOnly("bonusAP", &IUnit::BonusMagicDamage)
			.addPropertyReadOnly("range", &IUnit::AttackRange)
			.addPropertyReadOnly("totalPhysicalDamage", &IUnit::TotalPhysicalDamage)
			.addPropertyReadOnly("totalMagicDamage", &IUnit::TotalMagicDamage)
			.addPropertyReadOnly("serverpos", &IUnit::ServerPosition)
			.addPropertyReadOnly("navigationPath", &IUnit::GetNavigationPath)
			.addPropertyReadOnly("boundingRadius", &IUnit::BoundingRadius)
			.addPropertyReadOnly("pos", &IUnit::GetPosition)
			.addPropertyReadOnly("isMelee", &IUnit::IsMelee)
			.addPropertyReadOnly("isRanged", &IUnit::IsRanged)
			.addPropertyReadOnly("handle", &IUnit::GetTargetId)
			.addPropertyReadOnly("charName", &IUnit::SkinName)
			.addPropertyReadOnly("mana", &IUnit::GetMana)
			.addPropertyReadOnly("maxMana", &IUnit::GetMaxMana)
			.addPropertyReadOnly("manaPercent", &IUnit::ManaPercent)
			.addPropertyReadOnly("healthPercent", &IUnit::HealthPercent)
			.addPropertyReadOnly("level", &IUnit::GetLevel)
			.addFunction("HasBuff", &IUnit::HasBuff)
			.addPropertyReadOnly("networkID", &IUnit::GetNetworkId)
			.addFunction("HasItem", &IUnit::HasItemId)
			.addPropertyReadOnly("name", &IUnit::GetObjectName)
			.addPropertyReadOnly("team", &IUnit::GetTeam)
			.addPropertyReadOnly("flType", &IUnit::GetType)
			.addPropertyReadOnly("isCreep", &IUnit::IsCreep)
			.addPropertyReadOnly("isHero", &IUnit::IsHero)
			.addPropertyReadOnly("isTurret", &IUnit::IsTurret)
			.addPropertyReadOnly("isMissile", &IUnit::IsMissile)
			.addPropertyReadOnly("isCamp", &IUnit::IsJungleCreep)
			.addPropertyReadOnly("dead", &IUnit::IsDead)
			.addPropertyReadOnly("health", &IUnit::GetHealth)
			.addPropertyReadOnly("maxHealth", &IUnit::GetMaxHealth)
			.addFunction("CanUseSpell", &IUnit::GetSpellState)
			.addPropertyReadOnly("items", &IUnit::AllItems)
			.addPropertyReadOnly("buffs", &IUnit::GetBuffs)
			.addFunction("GetBuffCount", &IUnit::GetBuffCount)
			.addFunction("GetBuffStartTime", &IUnit::GetBuffStartTime)
			.addFunction("GetBuffCaster", &IUnit::GetBuffCaster)
			.addFunction("GetBuff", &IUnit::GetBuffDataByIndex)
			.addFunction("GetBuffByName", &IUnit::GetBuffDataByName)
			.addPropertyReadOnly("attackCastDelay", &IUnit::WindupTime)
			.addPropertyReadOnly("attackDelay", &IUnit::AnimationTime)
			.addPropertyReadOnly("path", &IUnit::GetWaypointList)
			.addPropertyReadOnly("attackData", &IUnit::GetAutoAttack)
			.addFunction("IsValidTarget", static_cast<bool(IUnit::*)()>(&IUnit::IsValidTarget))
			.addPropertyReadOnly("visible", &IUnit::IsVisible)
			.addPropertyReadOnly("isRecalling", &IUnit::IsRecalling)
			.addFunction("HasBuffOfType", &IUnit::HasBuffOfType)
			.addPropertyReadOnly("hpRegen", &IUnit::HPRegenRate)
			.addFunction("GetRealAutoAttackRange", &IUnit::GetRealAutoAttackRange)
			.addFunction("SetSkinId", &IUnit::SetSkinId)
			.addPropertyReadOnly("skinId", &IUnit::GetSkinId)
			.addPropertyReadOnly("isInvulnerable", &IUnit::IsInvulnerable)
			.addPropertyReadOnly("isTargetable", &IUnit::IsTargetable)
			.addPropertyReadOnly("hpBarPos", &IUnit::GetHPBarPos)
			.addPropertyReadOnly("isClone", &IUnit::IsClone)
			.addFunction("IsFacing", &IUnit::IsFacing)
			.addPropertyReadOnly("direction", &IUnit::Direction)
			.addFunction("LevelUpSpell", &IUnit::LevelUpSpell)
			.addFunction("charState", &IUnit::GetCharacterState)
			.addPropertyReadOnly("isWindingUp", &IUnit::IsWindingUp)
			.addPropertyReadOnly("buffCount", &IUnit::GetNumberOfBuffs)
			.addFunction("GetSpellbook", &IUnit::GetSpellBook)
			.addPropertyReadOnly("crit", &IUnit::Crit)
			.addPropertyReadOnly("isMoving", &IUnit::IsMoving)
			.addPropertyReadOnly("critDamage", &IUnit::CritDamageMultiplier)
			.addPropertyReadOnly("isWard", &IUnit::IsWard)
			.addPropertyReadOnly("valid", &IUnit::IsValidObject)
			.addPropertyReadOnly("type", &IUnit::GetClassIdentifier)
			.addPropertyReadOnly("isDashing", &IUnit::IsDashing)
			.addPropertyReadOnly("kills", &IUnit::GetKills)
			.addPropertyReadOnly("deaths", &IUnit::GetDeaths)
			.addPropertyReadOnly("assists", &IUnit::GetAssists)
			.addPropertyReadOnly("exp", &IUnit::GetExperience)
			.addFunction("CalculatePath", &IUnit::CalcPath)
			.addPropertyReadOnly("masteries", &IUnit::Masteries)
			.addPropertyReadOnly("activeSpell", &IUnit::GetLastCastedSpell)
		.endClass()

		.beginClass<IEntityList>("objManager")
			.addPropertyReadOnly("maxObjects", &IEntityList::NumberOfEntities)
			.addFunction("getObject", &IEntityList::GetEntityAt)
			.addFunction("GetObjectByNetworkId", &IEntityList::GetEntityByNetworkId)
			.addFunction("GetObjectByHandle", &IEntityList::GetEntityByTargetId)
			.addFunction("GetHeroes", &IEntityList::GetAllHeros)
			.addFunction("GetMinions", &IEntityList::GetAllMinions)
			.addFunction("GetMinionsAndWards", &IEntityList::GetAllMinionsAndWards)
			.addFunction("GetTurrets", &IEntityList::GetAllTurrets)
			.addFunction("GetMissiles", &IEntityList::GetAllMissiles)
			.addFunction("GetInhibitors", &IEntityList::GetAllInhibitors)
			.addPropertyReadOnly("player", &IEntityList::Player)
			.addPropertyReadOnly("allyNexus", &IEntityList::GetTeamNexus)
			.addPropertyReadOnly("enemyNexus", &IEntityList::GetEnemyNexus)
		.endClass()

		.beginClass<IDamage>("IDamage")
			.addFunction("GetAutoAttackDamage", &IDamage::GetAutoAttackDamage)
			.addFunction("CalcPhysicalDamage", &IDamage::CalcPhysicalDamage)
			.addFunction("CalcMixedDamage", &IDamage::CalcMixedDamage)
			.addFunction("CalcMagicDamage", &IDamage::CalcMagicDamage)
			.addFunction("GetSummonerSpellDamage", &IDamage::GetSummonerSpellDamage)
			.addFunction("GetSpellDamage", static_cast<double(IDamage::*)(IUnit*, IUnit*, int, int)>(&IDamage::GetSpellDamage))
		.endClass()

		.beginClass<ITargetSelector>("ITargetSelector")
			.addFunction("GetTarget", &ITargetSelector::FindTarget)
			.addFunction("ForceTarget", &ITargetSelector::SetOverrideFocusedTarget)
			.addPropertyReadOnly("ForcedTarget", &ITargetSelector::GetFocusedTarget)
		.endClass()

		.beginClass<IPrediction>("IPrediction")
			.addFunction("GetFuturePosition", &IPrediction::GetFuturePosition)
			.addFunction("GetBestLinearPosition", &IPrediction::GetBestLinearPosition)
			.addFunction("GetBestCircularPosition", &IPrediction::GetBestCircularPosition)
			.addFunction("IsWall", &IPrediction::IsPointWall)
			.addFunction("IsGrass", &IPrediction::IsPointGrass)
		.endClass()

		.beginClass<IMenu>("IMenu")
			.addFunction("New", &IMenu::AddMenu)
			.addFunction("Get", &IMenu::GetOption)
			.addFunction("Remove", &IMenu::Remove)
			.addFunction("AddBoolean", &IMenu::CheckBox)
			.addFunction("AddFloat", &IMenu::AddFloat)
			.addFunction("AddInteger", &IMenu::AddInteger)
			.addFunction("AddKey", &IMenu::AddKey)
			.addFunction("AddSelection", &IMenu::AddSelection)
		.endClass()

		.beginClass<IMenuOption>("IMenuOption")
			.addFunction("IsEnabled", &IMenuOption::Enabled)
			.addFunction("IsKeyPressed", &IMenuOption::KeyPress)
			.addFunction("GetFloat", &IMenuOption::GetFloat)
			.addFunction("GetInteger", &IMenuOption::GetInteger)
			.addFunction("SetFloat", &IMenuOption::UpdateFloat)
			.addFunction("SetInteger", &IMenuOption::UpdateInteger)
		.endClass()

		.beginClass<IHealthPrediction>("IHealthPrediction")
			.addFunction("GetPredictedHealth", &IHealthPrediction::GetPredictedHealth)
			.addFunction("GetIncomingDamage", &IHealthPrediction::GetKSDamage)
		.endClass()

		.beginClass<IGame>("IGame")
			.addFunction("PrintChat", &IGame::PrintChat)
			.addPropertyReadOnly("Latency", &IGame::Latency)
			.addPropertyReadOnly("Status", &IGame::Status)
			.addPropertyReadOnly("Time", &IGame::Time)
			.addPropertyReadOnly("MousePosition", &IGame::CursorPosition)
			.addFunction("IssueOrder", static_cast<bool(IGame::*)(IUnit*, eGameObjectOrder, Vec3 const&)>(&IGame::IssueOrder))
			//.addFunction("IssueOrder", static_cast<bool(IGame::*)(IUnit*, eGameObjectOrder, IUnit*)>(&IGame::IssueOrder))
			.addPropertyReadOnly("IntervalPerTick", &IGame::IntervalPerTick)
			.addFunction("WorldToMinimap", &IGame::WorldToMinimap)
			.addPropertyReadOnly("Target", &IGame::GetSelectedTarget)
			.addFunction("ShowPing", static_cast<void(IGame::*)(int, Vec3 const&, bool)>(&IGame::ShowPing))
			//.addFunction("ShowPing", static_cast<void(IGame::*)(int, IUnit*, bool)>(&IGame::ShowPing))
			.addFunction("SendPing", static_cast<void(IGame::*)(int, Vec3 const&)>(&IGame::SendPing))
			//.addFunction("SendPing", static_cast<void(IGame::*)(int, IUnit*)>(&IGame::SendPing))
			.addPropertyReadOnly("Type", &IGame::GetGameType)
			.addPropertyReadOnly("MapId", &IGame::GetMapId)
			.addFunction("Say", &IGame::SendChat)
			.addFunction("Taunt", &IGame::Taunt)
			.addFunction("BuyItem", &IGame::BuyItem)
			.addFunction("SellItem", &IGame::SellItem)
			.addFunction("SwapItems", &IGame::SwapItems)
			.addFunction("Quit", &IGame::Quit)
			.addFunction("CanLevelUpSpell", &IGame::CanLevelUpSpell)
			.addFunction("WorldToScreen", static_cast<bool(IGame::*)(Vec3 const&, Vec2*)>(&IGame::Projection))
			//.addFunction("WorldToScreen", static_cast<bool(IGame::*)(Vec3 const&, Vec3*)>(&IGame::Projection))
			.addPropertyReadOnly("TickCount", &IGame::TickCount)
			.addPropertyReadOnly("CurrentTick", &IGame::CurrentTick)
			.addPropertyReadOnly("IsChatOpen", &IGame::IsChatOpen)
			.addFunction("ScreenToWorld", &IGame::ScreenToWorld)
			.addFunction("WithinFogOfWar", &IGame::WithinFogOfWar)
			.addPropertyReadOnly("IsShopOpen", &IGame::IsShopOpen)
			.addPropertyReadOnly("IsScoreboardOpen", &IGame::IsScoreboardOpen)
		.endClass()

		.beginClass<IOrbwalking>("IOrbwalker")
			.addFunction("SetOverrideTarget", &IOrbwalking::SetOverrideTarget)
			.addFunction("SetOverridePosition", &IOrbwalking::SetOverridePosition)
			.addFunction("SetMovement", &IOrbwalking::SetMovementAllowed)
			.addFunction("SetAttack", &IOrbwalking::SetAttacksAllowed)
			.addPropertyReadOnly("LastTarget", &IOrbwalking::GetLastTarget)
			.addPropertyReadOnly("LastPosition", &IOrbwalking::GetLastPosition)
			.addFunction("CanAttack", &IOrbwalking::CanAttack)
			.addFunction("CanMove", &IOrbwalking::CanMove)
			.addFunction("ResetAA", &IOrbwalking::ResetAA)
			.addFunction("GetAutoAttackRange", &IOrbwalking::GetAutoAttackRange)
			.addFunction("Orbwalk", &IOrbwalking::Orbwalk)
		.endClass()

		.beginClass<ISpell2>("ISpell")
			.addFunction("CastSpell", &ISpell2::CastOnPlayer)
			.addFunction("Cast", &ISpell2::CastOnTarget)
			.addFunction("CastAoE", &ISpell2::CastOnTargetAoE)
			.addFunction("CastSkillShot", &ISpell2::CastOnPosition)
			.addFunction("CastOnTarget", &ISpell2::CastOnUnit)
			.addFunction("GetBestCastPosition", &ISpell2::GetBestCastPosition)
			.addFunction("GetTarget", &ISpell2::FindTarget)
			.addPropertyReadOnly("isReady", &ISpell2::IsReady)
			.addPropertyReadOnly("isCharging", &ISpell2::IsCharging)
			.addPropertyReadOnly("manaCost", &ISpell2::ManaCost)
			.addPropertyReadOnly("range", &ISpell2::Range)
			.addPropertyReadOnly("radius", &ISpell2::Radius)
			.addPropertyReadOnly("speed", &ISpell2::Speed)
			.addPropertyReadOnly("delay", &ISpell2::GetDelay)
			.addPropertyReadOnly("chargePercent", &ISpell2::GetChargePercent)
			.addPropertyReadOnly("slot", &ISpell2::GetSlot)
			.addPropertyReadOnly("type", &ISpell2::GetType)
			.addPropertyReadOnly("rangeCheckFrom", &ISpell2::GetRangeCheckFrom)
			.addPropertyReadOnly("collisionFlags", &ISpell2::GetCollisionFlags)
			.addPropertyReadOnly("name", &ISpell2::GetSpellName)
			.addPropertyReadOnly("chargedBuffName", &ISpell2::GetChargedBuffName)
			.addFunction("SetDelay", &ISpell2::SetOverrideDelay)
			.addFunction("SetRadius", &ISpell2::SetOverrideRadius)
			.addFunction("SetRange", &ISpell2::SetOverrideRange)
			.addFunction("SetSpeed", &ISpell2::SetOverrideSpeed)
			.addFunction("SetSkillshot", &ISpell2::SetSkillshot)
			.addFunction("SetCharged", &ISpell2::SetCharged)
			.addFunction("StartCharging", static_cast<void(ISpell2::*)(Vec3 const&)>(&ISpell2::StartCharging))
			.addFunction("SetFrom", &ISpell2::SetFrom)
			.addFunction("SetRangeCheckFrom", &ISpell2::SetRangeCheckFrom)
			.addFunction("SetSpellName", &ISpell2::SetSpellName)
			.addFunction("SetChargedBuffName", &ISpell2::SetChargedBuffName)
			.addFunction("CastFrom", &ISpell2::CastFrom)
		.endClass()

		.beginClass<IInventoryItem>("IInventoryItem")
			.addPropertyReadOnly("isOwned", &IInventoryItem::IsOwned)
			.addPropertyReadOnly("isReady", &IInventoryItem::IsReady)
			.addPropertyReadOnly("stacks", &IInventoryItem::Stacks)
			.addPropertyReadOnly("charges", &IInventoryItem::Charges)
			.addPropertyReadOnly("name", &IInventoryItem::GetName)
			.addPropertyReadOnly("displayName", &IInventoryItem::DisplayName)
			.addPropertyReadOnly("requiredSpellName", &IInventoryItem::RequiredSpellName)
			.addPropertyReadOnly("requiredChampion", &IInventoryItem::RequiredChampion)
			.addPropertyReadOnly("requiredLevel", &IInventoryItem::RequiredLevel)
			.addPropertyReadOnly("id", &IInventoryItem::GetName)
			.addPropertyReadOnly("slot", &IInventoryItem::ItemSpellSlot)
			.addPropertyReadOnly("maxStack", &IInventoryItem::MaxStack)
			.addPropertyReadOnly("price", &IInventoryItem::Price)
			.addPropertyReadOnly("consumed", &IInventoryItem::Consumed)
			.addPropertyReadOnly("consumedOnAcquire", &IInventoryItem::ConsumeOnAcquire)
			.addPropertyReadOnly("usableInStore", &IInventoryItem::UsableInStore)
			.addPropertyReadOnly("clickable", &IInventoryItem::Clickable)
			.addPropertyReadOnly("shown", &IInventoryItem::ShowInActiveItemDisplay)
			.addPropertyReadOnly("canBeSold", &IInventoryItem::CanBeSold)
			.addPropertyReadOnly("sellBackMod", &IInventoryItem::SellBackModifier)
			.addPropertyReadOnly("inStore", &IInventoryItem::InStore)
			.addPropertyReadOnly("usableInStore", &IInventoryItem::UsableInStore)
			.addFunction("CastSpell", &IInventoryItem::CastOnPlayer)
			.addFunction("CastOnTarget", &IInventoryItem::CastOnTarget)
			.addFunction("CastSkillShot", &IInventoryItem::CastOnPosition)
			.addFunction("IsTargetInRange", &IInventoryItem::IsTargetInRange)
			.addFunction("IsPosInRange", &IInventoryItem::IsPointInRange)
			.addFunction("SetRange", &IInventoryItem::SetRange)
			.addFunction("SetItemId", &IInventoryItem::SetItemId)
		.endClass()

		.beginClass<IPluginSDK>("ISDK")
			.addFunction("MenuConfig", &IPluginSDK::AddMenu)
			.addFunction("CreateSpell", &IPluginSDK::CreateSpell2)
			.addFunction("CreateItem", &IPluginSDK::CreateItemForId)
			.addFunction("GetWebResult", &IPluginSDK::GetWebResult)
		.endClass()

		.beginClass<IUtility>("IUtility")
			.addPropertyReadOnly("IsOnTop", &IUtility::IsLeagueWindowFocused)
			.addFunction("InFountain", &IUtility::IsPositionInFountain)
			.addFunction("UnderTurret", &IUtility::IsPositionUnderTurret)
			.addFunction("CreateConsole", &IUtility::CreateDebugConsole)
			.addFunction("CloseConsole", &IUtility::DestroyDebugConsole)
			.addFunction("LogConsole", &IUtility::ConsoleLog)
			.addFunction("LogFile", &IUtility::FileLog)
			.addFunction("ClearLogFile", &IUtility::ClearLogFile)
			.addFunction("GetRecallDuration", &IUtility::GetRecallDuration)
		.endClass()

		.beginClass<IFont>("IFont")
			.addFunction("SetColor", &IFont::SetColor)
			.addFunction("Draw", &IFont::Draw)
		.endClass()
		
		.beginClass<ITexture>("ISprite")
			.addFunction("Scale", static_cast<void(ITexture::*)(float)>(&ITexture::Scale))
			.addFunction("Resize", static_cast<void(ITexture::*)(float)>(&ITexture::Resize))
			.addFunction("SetColor", &ITexture::SetColor)
			.addFunction("Draw", &ITexture::DrawSprite)
			.addFunction("DrawElipse", &ITexture::DrawElipse)
			.addProperty("size", &ITexture::Size)
		.endClass()

		.beginClass<IRender>("IDraw")
			.addFunction("Text", &IRender::Text)
			.addFunction("Line", &IRender::DrawLine)
			.addFunction("FilledBox", &IRender::DrawFilledBox)
			.addFunction("DrawOutinedBox", &IRender::DrawOutinedBox)
			.addFunction("OutlinedCircle", static_cast<void(IRender::*)(Vec3 const&, Vec4 const&, float)>(&IRender::DrawOutlinedCircle))
			.addFunction("OutlinedCircle2D", static_cast<void(IRender::*)(Vec2 const&, Vec4 const&, float)>(&IRender::DrawOutlinedCircle))
			.addFunction("FilledCircle", &IRender::DrawFilledCircle)
			.addFunction("CreateFont", &IRender::CreateFont)
			.addFunction("CreateSprite", &IRender::CreateTextureFromFile)
			.addFunction("Circle", &IRender::DrawCircle)
			.addPropertyReadOnly("Resolution", &IRender::Size)
		.endClass()
	;

	Lua::setGlobal(L, "Game", GGame);
	Lua::setGlobal(L, "Orbwalker", GOrbwalking);
	Lua::setGlobal(L, "objManager", GEntityList);
	Lua::setGlobal(L, "Draw", GRender);
	Lua::setGlobal(L, "TargetSelector", GTargetSelector);
	Lua::setGlobal(L, "BuffData", GBuffData);
	Lua::setGlobal(L, "SpellData", GSpellData);
	Lua::setGlobal(L, "MissileData", GMissileData);
	Lua::setGlobal(L, "Damage", GDamage);
	Lua::setGlobal(L, "Prediction", GPrediction);
	Lua::setGlobal(L, "HealthPrediction", GHealthPrediction);
	Lua::setGlobal(L, "NavMesh", GNavMesh);
	Lua::setGlobal(L, "Utility", GUtility);
	Lua::setGlobal(L, "SDK", GPluginSDK);
	Lua::setGlobal(L, "myHero", GEntityList->Player());
	Lua::setGlobal(L, "LPP_PATH", GPluginSDK->GetDirectory().append("\\"));
	Lua::setGlobal(L, "SCRIPT_PATH", GPluginSDK->GetDirectory().append("\\LuaPlusPlus\\Scripts\\"));
	Lua::setGlobal(L, "COMMON_PATH", GPluginSDK->GetDirectory().append("\\LuaPlusPlus\\Scripts\\Common\\"));
	Lua::setGlobal(L, "SPRITE_PATH", GPluginSDK->GetDirectory().append("\\Textures"));
	Lua::setGlobal(L, "LuaPlusPlus", 0.01);

	lua_getglobal(L, "package");
	lua_getfield(L, -1, "path");
	std::string cur_path = lua_tostring(L, -1);
	cur_path.append(";");
	cur_path.append(GetStartupPath("LuaPlusPlus\\Scripts\\Common\\"));
	cur_path.append("?.lua;");
	cur_path.append(GetStartupPath("LuaPlusPlus\\Scripts\\Common\\socket"));
	cur_path.append("?.dll;");
	cur_path.append(GetStartupPath("LuaPlusPlus\\Scripts\\"));
	cur_path.append("?.lua;");
	cur_path.append("?.lua");
	lua_pop(L, 1);
	lua_pushstring(L, cur_path.c_str());
	lua_setfield(L, -2, "path");
	lua_pop(L, 1);
}

std::string Engine::GetStartupPath(std::string append)
{
	std::string x;
	GPluginSDK->GetBaseDirectory(x);
	x.append("\\");
	x.append(append);
	return x;
}

void Engine::LoadScript(std::string file)
{
	using namespace LuaIntf;
	std::string x;
	GPluginSDK->GetBaseDirectory(x);
	x.append("\\LuaPlusPlus\\Scripts\\");
	x.append(file);
	if (file.find(".lua") == std::string::npos)
	{
		x.append(".lua");
		file.append(".lua");
	}
	Script* luaptr = new Script();
	Instances.push_back(luaptr);
	luaptr->ExecuteString(
		MULTI_LINE_STRING(
			local rawtype = type
			function type(variable)
				local Type = rawtype(variable)
				if Type == 'userdata' and variable.__type and rawtype(variable:__type()) == 'string' then
					return variable:__type()
				end
				return rawtype(variable)
			end
			function print(...)
				local t, len = {}, select('#', ...)
				for i = 1, len do
					local v = select(i, ...)
					local _type = type(v)
					if _type == 'string' then t[i] = v
					elseif _type == 'IUnit' then t[i] = 'Unit : '..v.charName
					elseif _type == 'Vec2' then t[i] = tostring(v)
					elseif _type == 'number' then t[i] = tostring(v)
					elseif _type == 'table' then t[i] = 'table'
					elseif _type == 'boolean' then t[i] = tostring(v)
					elseif _type == 'userdata' then t[i] = tostring(v)
					elseif _type == 'function' then t[i] = 'function'
					else t[i] = _type
					end
				end
				if len>0 then Game:PrintChat(table.concat(t)) end
			end
			function class(name) _ENV[name] = {} _ENV[name].__index = _ENV[name] local mt = {__call = function(self, ...) local b = {} setmetatable(b, _ENV[name]) b:__init(...) return b end} setmetatable(_ENV[name], mt) end
			function FileExist(path)
				assert(type(path) == "string", "FileExist: wrong argument types (<string> expected for path)")
				local file = io.open(path, "r")
				if file then 
					file:close() 
					return true
				else 
					return false 
				end		
			end
		)
	);
	Init(Instances.back()->State);
	std::string scriptName = file;
	if (file.find_last_of(".") != std::string::npos)
	{
		scriptName = file.substr(0, file.find_last_of("."));
	}

	std::transform(scriptName.begin(), scriptName.end(), scriptName.begin(), ::tolower);
	Lua::setGlobal(Instances.back()->State, "SCRIPT_NAME", scriptName);
	Lua::setGlobal(Instances.back()->State, "FILE_NAME", file.c_str());
	Instances.back()->ExecuteFile(x.c_str());
}