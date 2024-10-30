#include "lib/hook.h"

using namespace RE::BSScript;
using namespace SKSE;
using namespace SKSE::log;
using namespace SKSE::stl;

namespace
{
void InitializeLogging()
{
    auto logsFolder = SKSE::log::log_directory();
    if (!logsFolder)
        SKSE::stl::report_and_fail("SKSE log_directory not provided, logs disabled.");
    auto pluginName = SKSE::PluginDeclaration::GetSingleton()->GetName();
    auto logFilePath = *logsFolder / std::format("{}.log", pluginName);
    auto fileLoggerPtr = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath.string(), true);
    auto loggerPtr = std::make_shared<spdlog::logger>("log", std::move(fileLoggerPtr));
    spdlog::set_default_logger(std::move(loggerPtr));
    spdlog::set_level(spdlog::level::trace);
    spdlog::flush_on(spdlog::level::trace);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%t] [%s:%#] %v");
}

void InitializePapyrus()
{
    log::trace("Initializing Papyrus binding...");
}

void InitializeHooking()
{
    VarUtils::init();
    Compatibility::init();
    Config::loadInI();
    Hook::Hook();
}

void InitializeMessaging()
{
    if (!GetMessagingInterface()->RegisterListener([](MessagingInterface::Message *message) {
            switch (message->type)
            {
            case MessagingInterface::kPostLoad:
                break;
            case MessagingInterface::kPostPostLoad:
                break;
            case MessagingInterface::kInputLoaded:
                break;
            case MessagingInterface::kDataLoaded:
                InitializeHooking();
                break;

            case MessagingInterface::kNewGame:
                Hook::AnimationGraphEventSink::Install();
                if (Stances::enableStances)
                    Stances::init();
                break;
            case MessagingInterface::kPreLoadGame:
                break;
            case MessagingInterface::kPostLoadGame:
                Hook::AnimationGraphEventSink::Install();
                if (Stances::enableStances)
                    Stances::init();
                break;
            case MessagingInterface::kSaveGame:
                break;
            case MessagingInterface::kDeleteGame:
                break;
            }
        }))
    {
        stl::report_and_fail("Unable to register message listener.");
    }
}
} // namespace

SKSEPluginLoad(const LoadInterface *skse)
{
    InitializeLogging();

    auto *plugin = PluginDeclaration::GetSingleton();
    auto version = plugin->GetVersion();
    log::info("{} is loading...", plugin->GetName());

    Init(skse);
    InitializeMessaging();
    InitializePapyrus();

    log::info("{} has finished loading.", plugin->GetName());
    return true;
}
