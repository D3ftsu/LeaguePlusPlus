#pragma once

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