#pragma once

#include <RE/Skyrim.h>
#include <REL/Relocation.h>
#include <SKSE/SKSE.h>

#include <algorithm>
#include <any>
#include <d3d11.h>
#include <deque>
#include <dxgi.h>
#include <filesystem>
#include <format>
#include <iostream>
#include <map>
#include <random>
#include <shared_mutex>
#include <string>
#include <vector>
#include <version>

#undef cdecl // Workaround for Clang 14 CMake configure error.

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>

// Compatible declarations with other sample projects.
#define DLLEXPORT __declspec(dllexport)

using namespace std::literals;
using namespace REL::literals;
namespace logger = SKSE::log;
