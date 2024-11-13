#include "utils.h"

namespace Style
{
enum AttackType : std::uint8_t
{
    Right = 0,
    Left,
    Dual,
    VanillaLeft,
    VanillaRight
};
enum Styles : std::uint8_t
{
    Null = 0,
    TwoHand,
    Bow,
    DualSword,
    DualFist,
    DualMagic,
    DualStaff,
    OnlySword,
    OnlyMagic,
    OnlyStaff,
    ShieldSword,
    ShieldFist,
    ShieldMagic,
    ShieldStaff,
    SwordFist,
    SwordMagic,
    SwordStaff,
    MagicSword,
    MagicFist,
    MagicStaff,
    StaffSword,
    StaffFist,
    StaffMagic,
};
typedef struct
{
    AttackType normalAttackType;
    bool repeatNormalAttack;
    bool sheatheNormalAttack;
    AttackType powerAttackType;
    bool repeatPowerAttack;
    bool sheathePowerAttack;
    uint32_t attackTypeModifier;
    AttackType altNormalAttackType;
    AttackType altPowerAttackType;
} CombatStyle;

extern Styles currentStyle;
extern std::unordered_map<Styles, CombatStyle> styleMap;

const char *GetStyleName(Styles style);
} // namespace Style
