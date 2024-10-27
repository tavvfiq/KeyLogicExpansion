#pragma once
#include "stances.h"

using namespace RE;
using namespace REL;

namespace Hook
{
    class MenuOpenHandler
    {
    public:
        bool CanProcess(InputEvent *a_event);
        static bool CP(InputEvent *a_event);
        bool ProcessButton(ButtonEvent *a_event);
        static bool PB(ButtonEvent *a_event);
        static void Hook();

    private:
        static MenuOpenHandler *that;
        typedef bool (MenuOpenHandler::*FnCanProcess)(InputEvent *);
        typedef bool (MenuOpenHandler::*FnProcessButton)(ButtonEvent *);
        static FnCanProcess FnCP;
        static FnProcessButton FnPB;
    };
    class AutoMoveHandler
    {
    public:
        bool ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data);
        static bool PB(ButtonEvent *a_event, PlayerControlsData *a_data);
        static void Hook();

    private:
        static AutoMoveHandler *that;
        typedef bool (AutoMoveHandler::*FnProcessButton)(ButtonEvent *, PlayerControlsData *);
        static FnProcessButton FnPB;
    };

    class FirstPersonState
    {
    public:
        bool CanProcess(InputEvent *a_event);
        static bool CP(InputEvent *a_event);
        bool ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data);
        static bool PB(ButtonEvent *a_event, PlayerControlsData *a_data);
        static void Hook();

    private:
        static FirstPersonState *that;
        typedef bool (FirstPersonState::*FnCanProcess)(InputEvent *);
        typedef bool (FirstPersonState::*FnProcessButton)(ButtonEvent *, PlayerControlsData *);
        static FnCanProcess FnCP;
        static FnProcessButton FnPB;
    };

    class AttackBlockHandler
    {
    public:
        bool CanProcess(InputEvent *a_event);
        static bool CP(InputEvent *a_event);
        bool ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data);
        static bool PB(ButtonEvent *a_event, PlayerControlsData *a_data);
        static void Hook();

    private:
        static AttackBlockHandler *that;
        typedef bool (AttackBlockHandler::*FnCanProcess)(InputEvent *);
        typedef bool (AttackBlockHandler::*FnProcessButton)(ButtonEvent *, PlayerControlsData *);
        static FnCanProcess FnCP;
        static FnProcessButton FnPB;
    };

    class TogglePOVHandler
    {
    public:
        bool CanProcess(InputEvent *a_event);
        static bool CP(InputEvent *a_event);
        bool ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data);
        static bool PB(ButtonEvent *a_event, PlayerControlsData *a_data);
        static void Hook();

    private:
        static TogglePOVHandler *that;
        typedef bool (TogglePOVHandler::*FnCanProcess)(InputEvent *);
        typedef bool (TogglePOVHandler::*FnProcessButton)(ButtonEvent *, PlayerControlsData *);
        static FnCanProcess FnCP;
        static FnProcessButton FnPB;
    };

    class MovementHandler
    {
    public:
        bool CanProcess(InputEvent *a_event);
        static bool CP(InputEvent *a_event);
        bool ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data);
        static bool PB(ButtonEvent *a_event, PlayerControlsData *a_data);
        static void Hook();

    private:
        static MovementHandler *that;
        typedef bool (MovementHandler::*FnCanProcess)(InputEvent *);
        typedef bool (MovementHandler::*FnProcessButton)(ButtonEvent *, PlayerControlsData *);
        static FnCanProcess FnCP;
        static FnProcessButton FnPB;
    };

    class ThirdPersonState
    {
    public:
        bool CanProcess(InputEvent *a_event);
        static bool CP(InputEvent *a_event);
        bool ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data);
        static bool PB(ButtonEvent *a_event, PlayerControlsData *a_data);
        static void Hook();

    private:
        static ThirdPersonState *that;
        typedef bool (ThirdPersonState::*FnCanProcess)(InputEvent *);
        typedef bool (ThirdPersonState::*FnProcessButton)(ButtonEvent *, PlayerControlsData *);
        static FnCanProcess FnCP;
        static FnProcessButton FnPB;
    };

    void Hook();
}