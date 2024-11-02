#pragma once
#include "SimpleIni.h"
#include "stances.h"

namespace Custom
{

typedef struct
{
    uint32_t code;
    uint64_t time;
} NewInput;

enum CustomInputMode
{
    Click,
    DoubleClick,
    Hold
};

enum CustomCondition
{
    ModExist,
    PlayerStatus,
    Has,
    ActorValueRange,
    ModValueRange
};

enum CustomEvent
{
    AddPerk,
    RemovePerk,
    ApplySpellEffect,
    RemoveSpellEffect,
    CastSpell,
    ChangeActorValue,
    ChangeGraphValue,
    ChangeModValue,
    SendGraphEvent
};

typedef struct
{
    char logic;
    CustomCondition condition;
    std::any func;
    std::any arg0;
    std::any arg1;
    std::any arg2;
} Condition;

typedef struct
{
    CustomEvent event;
    std::any func;
    std::any arg0;
    std::any arg1;
    std::any arg2;
} Event;

typedef struct
{
    uint32_t modifier;
    CustomInputMode inputMode;
    std::vector<Condition> conditon;
    std::vector<Event> event;
} CustomAction;

// Custom Input
extern bool enableCustomInput;
extern std::unordered_map<uint32_t, std::map<uint32_t, std::vector<CustomAction>, std::greater<uint32_t>>>
    customSearchList;
extern std::deque<NewInput> inputQueue;

void LoadCustom();

void CustomEventDecoder();
} // namespace Custom