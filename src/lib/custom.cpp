#include "custom.h"

namespace Custom
{
bool enableCustomInput;
const static std::string custom_dir = "Data/SKSE/Plugins/KeyLogicExpansion/";
CSimpleIniA ini;

// Custom Input
std::vector<CustomInput> custom;

void LoadCustom()
{
    ini.SetUnicode(true);
    try
    {
        for (const auto &entry : std::filesystem::directory_iterator(custom_dir))
            if (entry.path().extension() == ".ini")
            {
                ini.LoadFile(entry.path().c_str());
                std::list<CSimpleIniA::Entry> sections;
                ini.GetAllSections(std::ref(sections));
                for (auto section : sections)
                {
                    bool enable = ini.GetBoolValue(section.pItem, "enable");
                    uint32_t trigger = ini.GetBoolValue(section.pItem, "trigger");
                    uint32_t modifier = ini.GetBoolValue(section.pItem, "modifier");
                    uint32_t priority = ini.GetBoolValue(section.pItem, "priority");
                    CustomEvent event = (CustomEvent)ini.GetLongValue(section.pItem, "event");
                    std::string content = ini.GetValue(section.pItem, "content");
                    custom.push_back(CustomInput{enable, trigger, modifier, priority, event, content});
                }
            }
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        logger::error("Error:{}", e.what());
    }
}
} // namespace Custom