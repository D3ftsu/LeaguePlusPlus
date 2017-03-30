#pragma once

#ifndef _INCL_GUARD_EVENTMANAGER_H
#define _INCL_GUARD_EVENTMANAGER_H

#include "Config.h"

static class EventManager
{
public:
	static void Initialize();
	static void Unload();
	static void Combo();
	static void Harass();
	static void Clear();
	static void PermaActive();
};

#endif