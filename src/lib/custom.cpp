#include "custom.h"

namespace Custom
{
const static std::string custom_dir = "Data/SKSE/Plugins/KeyLogicExpansion/";
CSimpleIniA ini;

// Custom Input
bool enableCustomInput;
std::unordered_map<uint32_t, std::map<uint32_t, CustomAction, std::greater<uint32_t>>> customSearchList;

std::vector<std::string> SplitString(const std::string &str)
{
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;

    while (std::getline(iss, token, ','))
    {
        // Trim leading and trailing spaces
        size_t first = token.find_first_not_of(" \t\n\r\f\v");
        size_t last = token.find_last_not_of(" \t\n\r\f\v");

        // If we found non-space characters
        if (first != std::string::npos && last != std::string::npos)
        {
            token = token.substr(first, (last - first + 1));
        }
        else
        {
            // If all characters are spaces, then clear the token
            token.clear();
        }

        tokens.push_back(token);
    }

    return tokens;
}

bool DecodeEvent(std::string event_str, std::string content_str, std::vector<CustomEvent> &event_ouput,
                 std::vector<std::variant<std::string, RE::TESForm *>> &content_output)
{
    auto spit_event = SplitString(event_str);
    auto spit_content = SplitString(content_str);
    for (int i = 0; i < spit_event.size(); i++)
    {
        if (spit_event[i] == "AddPerk")
        {
            event_ouput.push_back(CustomEvent::AddPerk);
            content_output.push_back(FormUtils::GetForm(spit_content[i]));
        }
        else if (spit_event[i] == "RemovePerk")
        {
            event_ouput.push_back(CustomEvent::RemovePerk);
            content_output.push_back(FormUtils::GetForm(spit_content[i]));
        }
        else
            return false;
    }
    return true;
}

void LoadCustom()
{
    ini.SetUnicode(true);
    try
    {
        for (const auto &entry : std::filesystem::directory_iterator(custom_dir))
            if (entry.path().extension() == ".ini")
            {
                if (entry.path().filename() == "example.ini")
                    continue;
                if (entry.path().filename() == "stances.ini")
                    if (!Stances::enableStances)
                        continue;
                ini.LoadFile(entry.path().c_str());
                std::list<CSimpleIniA::Entry> sections;
                ini.GetAllSections(std::ref(sections));
                for (auto section : sections)
                {
                    bool enable = ini.GetBoolValue(section.pItem, "enable");
                    if (!enable)
                        continue;
                    uint32_t trigger = ini.GetLongValue(section.pItem, "trigger");
                    uint32_t modifier = ini.GetLongValue(section.pItem, "modifier");
                    uint32_t priority = ini.GetLongValue(section.pItem, "priority");
                    std::string downEvent_str = ini.GetValue(section.pItem, "DownEvent");
                    std::string downContent_str = ini.GetValue(section.pItem, "DownContent");
                    std::string upEvent_str = ini.GetValue(section.pItem, "UpEvent");
                    std::string upContent_str = ini.GetValue(section.pItem, "UpContent");
                    std::vector<CustomEvent> downEvent;
                    std::vector<CustomEvent> upEvent;
                    std::vector<std::variant<std::string, RE::TESForm *>> downContent;
                    std::vector<std::variant<std::string, RE::TESForm *>> upContent;
                    if (!DecodeEvent(downEvent_str, downContent_str, std::ref(downEvent), std::ref(downContent)) ||
                        !DecodeEvent(upEvent_str, upContent_str, std::ref(upEvent), std::ref(upContent)))
                    {
                        logger::error("Can't resolve Section:{}, skip this.", section.pItem);
                        continue;
                    }
                    auto it = customSearchList.find(trigger);
                    if (it == customSearchList.end())
                    {
                        std::map<uint32_t, CustomAction, std::greater<uint32_t>> tmp;
                        tmp.insert(std::make_pair(priority,
                                                  CustomAction{modifier, downEvent, upEvent, downContent, upContent}));
                        customSearchList.insert(std::make_pair(trigger, tmp));
                    }
                    else
                        it->second.insert(std::make_pair(
                            priority, CustomAction{modifier, downEvent, upEvent, downContent, upContent}));
                }
            }
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        logger::error("Error:{}", e.what());
    }
}

void doCustomAction(CustomEvent event, std::string content)
{
}
} // namespace Custom