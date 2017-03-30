#include "SpellManager.h"

ISpell2* SpellManager::Q;
ISpell2* SpellManager::W;
ISpell2* SpellManager::E;
ISpell2* SpellManager::R;

void SpellManager::Initialize()
{
	Q = GPluginSDK->CreateSpell2(eSpellSlot::kSlotQ, eSpellType::kLineCast, true, false, kCollidesWithYasuoWall);
	Q->SetSkillshot(0.25f, 40.f, 2400.f, 1200.f);
	W = GPluginSDK->CreateSpell2(eSpellSlot::kSlotW, eSpellType::kLineCast, false, false, kCollidesWithNothing);
	W->SetOverrideRange(5200.f);
	E = GPluginSDK->CreateSpell2(eSpellSlot::kSlotE, eSpellType::kTargetCast, false, false, kCollidesWithNothing);
	E->SetOverrideRange(1000.f);
	R = GPluginSDK->CreateSpell2(eSpellSlot::kSlotR, eSpellType::kTargetCast, false, false, kCollidesWithNothing);
	R->SetOverrideRange(1400.f);
}