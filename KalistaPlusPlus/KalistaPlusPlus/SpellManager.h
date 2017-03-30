#pragma once

#ifndef _INCL_GUARD_SPELLMANAGER_H
#define _INCL_GUARD_SPELLMANAGER_H

#include <PluginSDK.h>

static class SpellManager
{
public:
	static ISpell2* Q;
	static ISpell2* W;
	static ISpell2* E;
	static ISpell2* R;
	static void Initialize();
};

#endif