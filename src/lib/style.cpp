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
    case Styles::ShieldSword:
        return "ShieldSword";
    case Styles::ShieldFist:
        return "ShieldFist";
    case Styles::ShieldMagic:
        return "ShieldMagic";
    case Styles::ShieldStaff:
        return "ShieldStaff";
    case Styles::TorchSword:
        return "TorchSword";
    case Styles::TorchFist:
        return "TorchFist";
    case Styles::TorchMagic:
        return "TorchMagic";
    case Styles::TorchStaff:
        return "TorchStaff";
    case Styles::SwordFist:
        return "SwordFist";
    case Styles::SwordMagic:
        return "SwordMagic";
    case Styles::SwordStaff:
        return "SwordStaff";
    case Styles::FistSword:
        return "FistSword";
    case Styles::FistMagic:
        return "FistMagic";
    case Styles::FistStaff:
        return "FistStaff";
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
bool IsSheild()
{
    switch (currentStyle)
    {
    case Styles::ShieldSword:
        return true;
    case Styles::ShieldFist:
        return true;
    case Styles::ShieldMagic:
        return true;
    case Styles::ShieldStaff:
        return true;
    default:
        return false;
    }
}
bool IsSpellLeft()
{
    switch (currentStyle)
    {
    case Styles::DualMagic:
        return true;
    case Styles::MagicSword:
        return true;
    case Styles::MagicFist:
        return true;
    case Styles::MagicStaff:
        return true;
    default:
        return false;
    }
}
bool IsSpellRight()
{
    switch (currentStyle)
    {
    case Styles::DualMagic:
        return true;
    case Styles::ShieldMagic:
        return true;
    case Styles::TorchMagic:
        return true;
    case Styles::SwordMagic:
        return true;
    case Styles::FistMagic:
        return true;
    case Styles::StaffMagic:
        return true;
    default:
        return false;
    }
}
bool IsStaffLeft()
{
    switch (currentStyle)
    {
    case Styles::DualStaff:
        return true;
    case Styles::StaffSword:
        return true;
    case Styles::StaffFist:
        return true;
    case Styles::StaffMagic:
        return true;
    default:
        return false;
    }
}
bool IsStaffRight()
{
    switch (currentStyle)
    {
    case Styles::DualStaff:
        return true;
    case Styles::ShieldStaff:
        return true;
    case Styles::TorchStaff:
        return true;
    case Styles::SwordStaff:
        return true;
    case Styles::FistStaff:
        return true;
    case Styles::MagicStaff:
        return true;
    default:
        return false;
    }
}
} // namespace Style