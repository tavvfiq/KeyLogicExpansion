#pragma once
#include "SimpleIni.h"
#include "stances.h"
#include "utils.h"

namespace Custom
{
extern bool enableCustomInput;

enum CustomEvent
{
    None = 0,
    Spell,
    Perk,
    Command
};

typedef struct
{
    bool enable;
    uint32_t trigger;
    uint32_t modifier;
    uint32_t priority;
    CustomEvent event;
    std::string content;
} CustomInput;

// Custom Input
extern std::vector<CustomInput> custom;

void LoadCustom();
} // namespace Custom