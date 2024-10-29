#pragma once
#include "SimpleIni.h"
#include "stances.h"
#include "utils.h"

namespace Custom
{

enum CustomEvent
{
    AddPerk,
    RemovePerk,
    UseItem,
    ChangeActorValue,
    SendGraphEvent
};

typedef struct
{
    uint32_t modifier;
    std::vector<CustomEvent> downEvent;
    std::vector<CustomEvent> upEvent;
    std::vector<std::variant<std::string, RE::TESForm *>> downContent;
    std::vector<std::variant<std::string, RE::TESForm *>> upContent;
} CustomAction;

// Custom Input
extern bool enableCustomInput;
extern std::unordered_map<uint32_t, std::map<uint32_t, CustomAction, std::greater<uint32_t>>> customSearchList;

void LoadCustom();

void doCustomAction(CustomEvent event, std::string content);
} // namespace Custom