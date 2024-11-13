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

Styles GetStyle(std::string styleName)
{
    if (styleName == "TwoHand")
        return Styles::TwoHand;
    if (styleName == "Bow")
        return Styles::Bow;
    if (styleName == "DualSword")
        return Styles::DualSword;
    if (styleName == "DualFist")
        return Styles::DualFist;
    if (styleName == "DualMagic")
        return Styles::DualMagic;
    if (styleName == "DualStaff")
        return Styles::DualStaff;
    if (styleName == "OnlySword")
        return Styles::OnlySword;
    if (styleName == "OnlyMagic")
        return Styles::OnlyMagic;
    if (styleName == "OnlyStaff")
        return Styles::OnlyStaff;
    if (styleName == "ShieldSword")
        return Styles::ShieldSword;
    if (styleName == "ShieldFist")
        return Styles::ShieldFist;
    if (styleName == "ShieldMagic")
        return Styles::ShieldMagic;
    if (styleName == "ShieldStaff")
        return Styles::ShieldStaff;
    if (styleName == "SwordFist")
        return Styles::SwordFist;
    if (styleName == "SwordMagic")
        return Styles::SwordMagic;
    if (styleName == "SwordStaff")
        return Styles::SwordStaff;
    if (styleName == "MagicSword")
        return Styles::MagicSword;
    if (styleName == "MagicFist")
        return Styles::MagicFist;
    if (styleName == "MagicStaff")
        return Styles::MagicStaff;
    if (styleName == "StaffSword")
        return Styles::StaffSword;
    if (styleName == "StaffFist")
        return Styles::StaffFist;
    if (styleName == "StaffMagic")
        return Styles::StaffMagic;
    return Styles::Null;
}

} // namespace Style