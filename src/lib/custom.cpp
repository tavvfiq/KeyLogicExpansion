#include "custom.h"

namespace Custom
{
const static std::string custom_dir = "Data/SKSE/Plugins/KeyLogicExpansion/";
CSimpleIniA ini;

// Custom Input
bool enableCustomInput;
std::unordered_map<uint32_t, std::map<uint32_t, std::vector<CustomAction>, std::greater<uint32_t>>> customSearchList;
std::deque<NewInput> inputQueue;

std::vector<std::string> SplitString(std::string str, char splitToken)
{
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;

    while (std::getline(iss, token, splitToken))
    {
        // Detect Some Useless Symbol
        size_t first = token.find_first_not_of(" \t\n\r\f\v");
        size_t last = token.find_last_not_of(" \t\n\r\f\v");

        // Clear First and Last Sapce
        if (first != std::string::npos && last != std::string::npos)
            token = token.substr(first, (last - first + 1));
        else
            token.clear();

        tokens.push_back(token);
    }

    return tokens;
}

bool DecodeInputMode(std::string inputMode_str, CustomInputMode &inputMode)
{
    if (inputMode_str == "Click")
    {
        inputMode = CustomInputMode::Click;
        return true;
    }
    else if (inputMode_str == "DoubleClick")
    {
        inputMode = CustomInputMode::DoubleClick;
        return true;
    }
    else if (inputMode_str == "Hold")
    {
        inputMode = CustomInputMode::Hold;
        return true;
    }
    else
        return false;
}

bool DecodeConditon(std::string condition_str, std::string content_str,
                    std::vector<std::vector<std::any>> &conditon_ouput)
{
    auto spit_conditon = SplitString(condition_str, ',');
    auto spit_content = SplitString(content_str, ',');
    for (int i = 0; i < spit_conditon.size(); i++)
    {
        if (spit_conditon[i] == "ModExist")
        {
            auto target = SplitString(spit_content[i], '|');
            if (target.size() < 2)
                return false;
            bool res = false;
            if (target[0] == "Mod")
                res = ModSupport::ModExist(target[1]);
            else if (target[0] == "DLL")
                res = ModSupport::DLLExist(target[1]);
            if (!res)
                return false;
        }
        else if (spit_conditon[i] == "PlayerStatus")
        {
        }
        else if (spit_conditon[i] == "ActorValueRange")
        {
        }
        else if (spit_conditon[i] == "ModValueRange")
        {
        }
    }
    return true;
}

bool DecodeEvent(std::string event_str, std::string content_str, std::vector<std::vector<std::any>> &event_ouput)
{
    auto spit_event = SplitString(event_str, ',');
    auto spit_content = SplitString(content_str, ',');
    for (int i = 0; i < spit_event.size(); i++)
    {
        if (spit_event[i] == "AddPerk")
        {
        }
        else if (spit_event[i] == "RemovePerk")
        {
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
        if (!std::filesystem::exists(custom_dir))
        {
            logger::warn("Can't find custom ini dir:{}, will generate a new one.", custom_dir.c_str());
            if (std::filesystem::create_directory(custom_dir))
                logger::trace("Generate dir success.");
            else
            {
                logger::error("Error: Can't generate dir.");
                return;
            }
        }
        for (const auto &entry : std::filesystem::directory_iterator(custom_dir))
            if (entry.path().extension() == ".ini")
            {
                if (entry.path().filename() == "example.ini")
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
                    std::string inputMode_str = ini.GetValue(section.pItem, "InputMode");
                    std::string condition_str = ini.GetValue(section.pItem, "Conditon");
                    std::string conditionContent_str = ini.GetValue(section.pItem, "ConditonContent");
                    std::string event_str = ini.GetValue(section.pItem, "Event");
                    std::string eventContent_str = ini.GetValue(section.pItem, "EventContent");

                    CustomInputMode inputMode;
                    std::vector<std::vector<std::any>> conditon;
                    std::vector<std::vector<std::any>> event;
                    if (!DecodeInputMode(inputMode_str, std::ref(inputMode)) ||
                        !DecodeConditon(condition_str, conditionContent_str, std::ref(conditon)) ||
                        !DecodeEvent(event_str, eventContent_str, std::ref(event)))
                    {
                        logger::error("Can't resolve Section:{}, skip this.", section.pItem);
                        continue;
                    }
                    auto node = customSearchList.find(trigger);
                    if (node == customSearchList.end())
                    {
                        std::map<uint32_t, std::vector<CustomAction>, std::greater<uint32_t>> tmp_map;
                        std::vector<CustomAction> tmp_vector;
                        tmp_vector.push_back(CustomAction{modifier, inputMode, conditon, event});
                        tmp_map.insert(std::make_pair(priority, tmp_vector));
                        customSearchList.insert(std::make_pair(trigger, tmp_map));
                    }
                    else
                    {
                        auto vector = node->second.find(priority);
                        if (vector == node->second.end())
                        {
                            std::vector<CustomAction> tmp_vector;
                            tmp_vector.push_back(CustomAction{modifier, inputMode, conditon, event});
                            node->second.insert(std::make_pair(priority, tmp_vector));
                        }
                        else
                            vector->second.push_back(CustomAction{modifier, inputMode, conditon, event});
                    }
                }
            }
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        logger::error("Error:{}", e.what());
    }
}

void CustomEventDecoder()
{
    logger::trace("Custom Event Decoder Start.");
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        if (inputQueue.empty())
            continue;
        auto input = inputQueue.front();
        inputQueue.pop_front();
    }
}
} // namespace Custom