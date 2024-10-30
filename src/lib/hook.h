#pragma once
#include "compatibility.h"

using namespace RE;
using namespace REL;

namespace Hook
{
//
// AnimationGraphEventSink
//
class AnimationGraphEventSink : public RE::BSTEventSink<RE::BSAnimationGraphEvent>
{
  public:
    static inline AnimationGraphEventSink *GetSingleton();
    static void Install();

  private:
    RE::BSEventNotifyControl ProcessEvent(const RE::BSAnimationGraphEvent *a_event,
                                          RE::BSTEventSource<RE::BSAnimationGraphEvent> *a_eventSource) override;
};
//
// MenuOpenHandler
//
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

//
// AutoMoveHandler
//
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

//
// FirstPersonState
//
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

//
// HeldStateHandler - AttackBlockHandler
//
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

//
// HeldStateHandler - SprintHandler
//
class SprintHandler
{
  public:
    bool CanProcess(InputEvent *a_event);
    static bool CP(InputEvent *a_event);
    bool ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data);
    static bool PB(ButtonEvent *a_event, PlayerControlsData *a_data);
    static void Hook();

  private:
    static SprintHandler *that;
    typedef bool (SprintHandler::*FnCanProcess)(InputEvent *);
    typedef bool (SprintHandler::*FnProcessButton)(ButtonEvent *, PlayerControlsData *);
    static FnCanProcess FnCP;
    static FnProcessButton FnPB;
};

//
// HeldStateHandler - TogglePOVHandler
//
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

//
// MovementHandler
//
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

//
// ReadyWeaponHandler
//
class ReadyWeaponHandler
{
  public:
    bool CanProcess(InputEvent *a_event);
    static bool CP(InputEvent *a_event);
    bool ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data);
    static bool PB(ButtonEvent *a_event, PlayerControlsData *a_data);
    static void Hook();

  private:
    static ReadyWeaponHandler *that;
    typedef bool (ReadyWeaponHandler::*FnCanProcess)(InputEvent *);
    typedef bool (ReadyWeaponHandler::*FnProcessButton)(ButtonEvent *, PlayerControlsData *);
    static FnCanProcess FnCP;
    static FnProcessButton FnPB;
};

//
// SneakHandler
//
class SneakHandler
{
  public:
    bool CanProcess(InputEvent *a_event);
    static bool CP(InputEvent *a_event);
    bool ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data);
    static bool PB(ButtonEvent *a_event, PlayerControlsData *a_data);
    static void Hook();

  private:
    static SneakHandler *that;
    typedef bool (SneakHandler::*FnCanProcess)(InputEvent *);
    typedef bool (SneakHandler::*FnProcessButton)(ButtonEvent *, PlayerControlsData *);
    static FnCanProcess FnCP;
    static FnProcessButton FnPB;
};

//
// ThirdPersonState
//
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
} // namespace Hook