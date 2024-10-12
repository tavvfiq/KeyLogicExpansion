#pragma once
#include "PCH.h"
#include "config.h"
#include "utils.h"

using namespace RE;
using namespace REL;
using namespace SKSE::log;

namespace AltTweenMenu
{
	static uint32_t tweenMenuKey[INPUT_DEVICE::kTotal];
	class HookMenuOpenHandler
	{
	public:
		typedef bool (HookMenuOpenHandler::*FnCanProcess)(InputEvent *);
		bool CanProcess(InputEvent *a_event);

		typedef bool (HookMenuOpenHandler::*FnProcessButton)(ButtonEvent *);
		bool ProcessButton(ButtonEvent *a_event);

		static void Hook();

	private:
		static std::unordered_map<uintptr_t, FnCanProcess> fnCP;
		static std::unordered_map<uintptr_t, FnProcessButton> fnPB;
	};
	static void Hook() { HookMenuOpenHandler::Hook(); }
}