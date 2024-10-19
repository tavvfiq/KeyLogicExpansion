#pragma once
#include "config.h"
#include "keyDecoder.h"

using namespace RE;
using namespace REL;

// using AutoMoveHandler as InputEvent Receiver
class HookAutoMoveHandler
{
public:
	typedef bool (HookAutoMoveHandler::*FnCanProcess)(InputEvent *);
	bool CanProcess(InputEvent *a_event);

	typedef bool (HookAutoMoveHandler::*FnProcessButton)(ButtonEvent *, PlayerControlsData *);
	bool ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data);

	static void Hook();

private:
	static std::unordered_map<uintptr_t, FnCanProcess> fnCP;
	static std::unordered_map<uintptr_t, FnProcessButton> fnPB;
};

class HookAttackBlockHandler
{
public:
	typedef bool (HookAttackBlockHandler::*FnCanProcess)(InputEvent *);
	bool CanProcess(InputEvent *a_event);

	typedef bool (HookAttackBlockHandler::*FnProcessButton)(ButtonEvent *);
	bool ProcessButton(ButtonEvent *a_event);

	static void Hook();

private:
	static std::unordered_map<uintptr_t, FnCanProcess> fnCP;
	static std::unordered_map<uintptr_t, FnProcessButton> fnPB;
};

extern std::deque<BSFixedString> menuEvent;
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