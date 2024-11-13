#include "style.h"

namespace Style
{
Styles currentStyle = Styles::Null;
std::unordered_map<Styles, CombatStyle> styleMap;

const char *GetStyleName(Styles style)
{
    switch (style)
    {
    case Styles::TwoHand:
        return "TwoHand";
    case Styles::Bow:
        return "Bow";
    case Styles::DualSword:
        return "DualSword";
    case Styles::DualFist:
        return "DualFist";
    case Styles::DualMagic:
        return "DualMagic";
    case Styles::DualStaff:
        return "DualStaff";
    case Styles::OnlySword:
        return "OnlySword";
    case Styles::OnlyMagic:
        return "OnlyMagic";
    case Styles::OnlyStaff:
        return "OnlyStaff";
    case Styles::ShieldSword:
        return "ShieldSword";
    case Styles::ShieldFist:
        return "ShieldFist";
    case Styles::ShieldMagic:
        return "ShieldMagic";
    case Styles::ShieldStaff:
        return "ShieldStaff";
    case Styles::SwordFist:
        return "SwordFist";
    case Styles::SwordMagic:
        return "SwordMagic";
    case Styles::SwordStaff:
        return "SwordStaff";
    case Styles::MagicSword:
        return "MagicSword";
    case Styles::MagicFist:
        return "MagicFist";
    case Styles::MagicStaff:
        return "MagicStaff";
    case Styles::StaffSword:
        return "StaffSword";
    case Styles::StaffFist:
        return "StaffFist";
    case Styles::StaffMagic:
        return "StaffMagic";
    default:
        return "Null";
    }
}
} // namespace Style