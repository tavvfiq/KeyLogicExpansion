#pragma once
#include "config.h"
#include "keyDecoder.h"

using namespace RE;
using namespace REL;

namespace Hook
{
	// using AutoMoveHandler as InputEvent Receiver
	class AutoMoveHandler
	{
	public:
		bool CanProcess(InputEvent *a_event);

		bool ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data);

		static void Hook();
	private:
		static AutoMoveHandler* that;
		typedef bool (AutoMoveHandler::*FnCanProcess)(InputEvent *);
		typedef bool (AutoMoveHandler::*FnProcessButton)(ButtonEvent *, PlayerControlsData *);
		static FnCanProcess FnCP;
		static FnProcessButton FnPB;
	};

	class MovementHandler {
	public:
		bool CanProcess(InputEvent *a_event);
		bool ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data);
		static void Hook();
	};

	class AttackBlockHandler
	{
	public:
		bool CanProcess(InputEvent *a_event);
		bool ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data);
		static void Hook();
	};

	class MenuOpenHandler
	{
	public:
		bool CanProcess(InputEvent *a_event);
		bool CP(InputEvent *a_event);

		bool ProcessButton(ButtonEvent *a_event);
		bool PB(ButtonEvent *a_event);

		static void Hook();
	private:
		static MenuOpenHandler* that;
		typedef bool (MenuOpenHandler::*FnCanProcess)(InputEvent *);
		typedef bool (MenuOpenHandler::*FnProcessButton)(ButtonEvent *);
		static FnCanProcess FnCP;
		static FnProcessButton FnPB;
	};

	void Hook();
}
