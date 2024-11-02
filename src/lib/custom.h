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
    uint32_t modifier;
    CustomInputMode inputMode;
    std::vector<std::vector<std::any>> conditon;
    std::vector<std::vector<std::any>> event;
} CustomAction;

// Custom Input
extern bool enableCustomInput;
extern std::unordered_map<uint32_t, std::map<uint32_t, std::vector<CustomAction>, std::greater<uint32_t>>>
    customSearchList;
extern std::deque<NewInput> inputQueue;

void LoadCustom();

void CustomEventDecoder();
} // namespace Custom