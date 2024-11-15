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
    ShieldSword,
    ShieldFist,
    ShieldMagic,
    ShieldStaff,
    TorchSword,
    TorchFist,
    TorchMagic,
    TorchStaff,
    SwordFist,
    SwordMagic,
    SwordStaff,
    FistSword,
    FistMagic,
    FistStaff,
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
    bool isAltTypeEnable;
    bool isUsingHold;
    uint32_t attackTypeModifier;
    AttackType altNormalAttackType;
    AttackType altPowerAttackType;
} CombatStyle;

extern Styles currentStyle;
extern std::unordered_map<Styles, CombatStyle> styleMap;

const char *GetStyleName(Styles style);
bool IsSheild();
bool IsSpellLeft();
bool IsSpellRight();
bool IsStaffLeft();
bool IsStaffRight();
} // namespace Style
