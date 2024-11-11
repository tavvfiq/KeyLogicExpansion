#pragma once
#include "custom.h"

namespace Config
{

#define NameToStr(n) str(std::string(#n))

// Features
extern bool enableHoldSprint;
extern bool enableHoldSneak;
extern uint32_t enableSheatheAttack;
extern bool enableReverseHorseAttack;

// Vanilla Input
extern uint32_t normalAttack;
extern uint32_t powerAttack;
extern uint32_t block;
extern uint32_t MagicModifier;
extern uint32_t BFCO_SpecialAttackModifier;
extern uint32_t BFCO_ComboAttack;

extern uint32_t altTweenMenu;
extern uint32_t altTogglePOV;

// Expand Input
extern uint32_t warAsh;
extern uint32_t warAshModifier;

extern uint32_t altZoomIn;
extern uint32_t altZoomOut;
extern uint32_t zoomModifier;

// Modifier Input
extern std::unordered_map<uint32_t, uint32_t> needModifier;

char *str(std::string ss);
void loadInI();
void saveInI();
} // namespace Config