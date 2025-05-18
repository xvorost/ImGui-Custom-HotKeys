#pragma once

#include "../ImGui/imgui.h"
#include "InputUtil.h"

class KeyBind;

namespace ImGuiCustom
{
    void hotkey(const char* label, KeyBind& keyy, bool* boundBool, float samelineOffset = 0.0f, const ImVec2& size = { 85.0f, 20.0f }) noexcept;
    bool CheckBindKey(KeyBindToggle key_toggle, bool& func);
    bool CheckBindKey(KeyBindToggle key_toggle);
    bool CheckHoldBindKey(KeyBindToggle key_toggle);
    bool CheckStateBindKey(KeyBindToggle key_toggle);
}
