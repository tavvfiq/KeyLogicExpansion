#pragma once
#include "compatibility.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "imgui_internal.h"
#include <dxgi.h>

namespace GUI
{
extern std::deque<uint32_t> guiInputQueue;
extern uint8_t guiNums;

void init();
void showGUI();
} // namespace GUI
