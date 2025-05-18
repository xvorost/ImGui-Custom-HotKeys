#include "../ImGui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../ImGui/imgui_internal.h"
#include <Windows.h>
#include "Hotkey.h"
#include "InputUtil.h"
#include <unordered_map>

ImGuiKey KeyCodeToImGuiKey_(int code);

static bool old_key_state = false;
static bool is_first_press = true;
static int last_key = -1;

bool ImGuiCustom::CheckBindKey(KeyBindToggle key_toggle, bool& func) {
    int current_key = key_toggle.toInt();
    bool current_key_state = (GetKeyState(current_key) & 0x8000) != 0;

    if (current_key_state && !old_key_state) {
        if (current_key != last_key) {
            is_first_press = true;
            last_key = current_key;
        }

        if (!is_first_press) {
            func = !func;
        }
        else {
            is_first_press = false;
        }
    }

    old_key_state = current_key_state;
    Sleep(10);
    return func;
}

bool ImGuiCustom::CheckBindKey(KeyBindToggle key_toggle) {
    static bool old_key_state = false;
    static bool is_first_press = true;
    static int last_key = -1;

    int current_key = key_toggle.toInt();
    bool current_key_state = (GetKeyState(current_key) & 0x8000) != 0;

    bool should_toggle = false;

    if (current_key_state && !old_key_state) {
        if (current_key != last_key) {
            is_first_press = true;  // Новая клавиша — ждём первого нажатия
            last_key = current_key;
        }

        if (!is_first_press) {
            should_toggle = true;  // Второе нажатие — можно переключать
        }
        else {
            is_first_press = false; // Первое нажатие — игнорируем, но запоминаем
        }
    }

    old_key_state = current_key_state;
    Sleep(10);

    return should_toggle;
}

bool ImGuiCustom::CheckHoldBindKey(KeyBindToggle key_toggle) {
    static int last_key = -1;

    int current_key = key_toggle.toInt();
    bool current_key_state = (GetKeyState(current_key) & 0x8000) != 0;

    // Обновляем последнюю клавишу
    if (current_key_state)
        last_key = current_key;

    // Если текущая клавиша нажата (и это та же клавиша, что была нажата последней)
    if (current_key_state && current_key == last_key)
        return true;

    // Если клавиша отжата (и это та же клавиша, что была нажата последней)
    if (!current_key_state && current_key == last_key) {
        last_key = -1; // Сбрасываем последнюю клавишу
        return false;
    }

    return false;
}

bool toggle_state_ = false;       // Текущее состояние (вкл/выкл)
bool ImGuiCustom::CheckStateBindKey(KeyBindToggle key_toggle) {
    static bool key_was_pressed = false;    // Защита от повторного срабатывания

    int current_key = key_toggle.toInt();
    bool is_key_pressed = (GetKeyState(current_key) & 0x8000) != 0;

    // Если клавиша нажата и ранее не была зажата
    if (is_key_pressed && !key_was_pressed) {
        toggle_state_ = !toggle_state_;       // Переключаем состояние
        key_was_pressed = true;            // Блокируем повторный триггер

    }
    // Если клавиша отжата — снимаем блокировку
    else if (!is_key_pressed) {
        key_was_pressed = false;
    }

    return toggle_state_;
}

struct KeyState {
    ImGuiKey key = ImGuiKey_None;
    bool waitingForKey = false;
    bool* boundBool = nullptr;
};

static std::unordered_map<std::string, KeyState> keyBindStates;

int once_btn = 0;

void ImGuiCustom::hotkey(const char* id, KeyBind& keyy, bool* boundBool, float samelineOffset, const ImVec2& size) noexcept
{
    KeyState& state = keyBindStates[id];

    if (once_btn < 2) {
        state.key = static_cast<ImGuiKey>(KeyCodeToImGuiKey_(keyy.toInt()));
        once_btn++;
    }

    std::string buttonLabel = (state.waitingForKey) ? "Press a key..." : ImGui::GetKeyName(state.key);

    if (ImGui::Button(buttonLabel.c_str(), size)) {
        state.waitingForKey = true;
    }

    if (state.waitingForKey) {
        for (int i = ImGuiKey_NamedKey_BEGIN; i < ImGuiKey_NamedKey_END; i++) {
            ImGuiKey key = (ImGuiKey)i;

            if (key == ImGuiKey_MouseLeft ||
                key == ImGuiKey_MouseRight ||
                key == ImGuiKey_MouseMiddle) {
                continue;
            }

            if (ImGui::IsKeyPressed(key)) {
                if (key == ImGuiKey_Escape) {
                    state.waitingForKey = false;
                    break;
                }
                keyy.setToPressedKey();
                state.key = key;
                state.boundBool = boundBool;
                if (boundBool) *boundBool = true;
                state.waitingForKey = false;
                break;
            }
        }
    }
}

ImGuiKey KeyCodeToImGuiKey_(int code) {
    switch (code) {
    case VK_ESCAPE: return ImGuiKey_Escape;
    case VK_RETURN: return ImGuiKey_Enter;
    case VK_TAB: return ImGuiKey_Tab;
    case VK_BACK: return ImGuiKey_Backspace;
    case VK_SPACE: return ImGuiKey_Space;

    case VK_LEFT: return ImGuiKey_LeftArrow;
    case VK_RIGHT: return ImGuiKey_RightArrow;
    case VK_UP: return ImGuiKey_UpArrow;
    case VK_DOWN: return ImGuiKey_DownArrow;

    case VK_PRIOR: return ImGuiKey_PageUp;
    case VK_NEXT: return ImGuiKey_PageDown;
    case VK_HOME: return ImGuiKey_Home;
    case VK_END: return ImGuiKey_End;
    case VK_INSERT: return ImGuiKey_Insert;
    case VK_DELETE: return ImGuiKey_Delete;

    case VK_CAPITAL: return ImGuiKey_CapsLock;
    case VK_LSHIFT: return ImGuiKey_LeftShift;
    case VK_RSHIFT: return ImGuiKey_RightShift;
    case VK_LCONTROL: return ImGuiKey_LeftCtrl;
    case VK_RCONTROL: return ImGuiKey_RightCtrl;
    case VK_LMENU: return ImGuiKey_LeftAlt;
    case VK_RMENU: return ImGuiKey_RightAlt;
    case VK_LWIN: return ImGuiKey_LeftSuper;
    case VK_RWIN: return ImGuiKey_RightSuper;

    case VK_OEM_1: return ImGuiKey_Semicolon;       // ;
    case VK_OEM_2: return ImGuiKey_Slash;           // /
    case VK_OEM_3: return ImGuiKey_GraveAccent;     // `
    case VK_OEM_4: return ImGuiKey_LeftBracket;     // [
    case VK_OEM_5: return ImGuiKey_Backslash;       // \ //
    case VK_OEM_6: return ImGuiKey_RightBracket;    // ]
    case VK_OEM_7: return ImGuiKey_Apostrophe;      // '
    case VK_OEM_COMMA: return ImGuiKey_Comma;       // ,
    case VK_OEM_MINUS: return ImGuiKey_Minus;       // -
    case VK_OEM_PERIOD: return ImGuiKey_Period;     // .
    case VK_OEM_PLUS: return ImGuiKey_Equal;        // =

    case VK_F1: return ImGuiKey_F1;
    case VK_F2: return ImGuiKey_F2;
    case VK_F3: return ImGuiKey_F3;
    case VK_F4: return ImGuiKey_F4;
    case VK_F5: return ImGuiKey_F5;
    case VK_F6: return ImGuiKey_F6;
    case VK_F7: return ImGuiKey_F7;
    case VK_F8: return ImGuiKey_F8;
    case VK_F9: return ImGuiKey_F9;
    case VK_F10: return ImGuiKey_F10;
    case VK_F11: return ImGuiKey_F11;
    case VK_F12: return ImGuiKey_F12;
    case VK_F13: return ImGuiKey_F13;
    case VK_F14: return ImGuiKey_F14;
    case VK_F15: return ImGuiKey_F15;
    case VK_F16: return ImGuiKey_F16;
    case VK_F17: return ImGuiKey_F17;
    case VK_F18: return ImGuiKey_F18;
    case VK_F19: return ImGuiKey_F19;
    case VK_F20: return ImGuiKey_F20;
    case VK_F21: return ImGuiKey_F21;
    case VK_F22: return ImGuiKey_F22;
    case VK_F23: return ImGuiKey_F23;
    case VK_F24: return ImGuiKey_F24;

    case VK_NUMLOCK: return ImGuiKey_NumLock;
    case VK_SCROLL: return ImGuiKey_ScrollLock;
    case VK_ADD: return ImGuiKey_KeypadAdd;
    case VK_SUBTRACT: return ImGuiKey_KeypadSubtract;
    case VK_MULTIPLY: return ImGuiKey_KeypadMultiply;
    case VK_DIVIDE: return ImGuiKey_KeypadDivide;
    case VK_DECIMAL: return ImGuiKey_KeypadDecimal;
    }

    if (code >= 'A' && code <= 'Z') {
        return static_cast<ImGuiKey>(ImGuiKey_A + (code - 'A'));
    }

    if (code >= '0' && code <= '9') {
        return static_cast<ImGuiKey>(ImGuiKey_0 + (code - '0'));
    }

    if (code >= VK_NUMPAD0 && code <= VK_NUMPAD9) {
        return static_cast<ImGuiKey>(ImGuiKey_Keypad0 + (code - VK_NUMPAD0));
    }

    if (code == 0) return ImGuiKey_MouseLeft;    // MOUSE1
    if (code == 1) return ImGuiKey_MouseRight;   // MOUSE2
    if (code == 2) return ImGuiKey_MouseMiddle;  // MOUSE3
    if (code == VK_XBUTTON1) return ImGuiKey_MouseX1;
    if (code == VK_XBUTTON2) return ImGuiKey_MouseX2;

    return ImGuiKey_None;
}
