#include <algorithm>
#include <array>
#include <string_view>

#ifdef _WIN32
#include <Windows.h>
#else
#include <SDL2/SDL.h>
#endif

#ifdef _WIN32

#define THISCALL_CONV __thiscall
#define FASTCALL_CONV __fastcall
#define CDECL_CONV __cdecl
#define STDCALL_CONV __stdcall

#else

#define THISCALL_CONV
#define FASTCALL_CONV
#define CDECL_CONV
#define STDCALL_CONV

#endif

#ifdef _WIN32

#define LINUX_ARGS(...)
#define RETURN_ADDRESS() std::uintptr_t(_ReturnAddress())
#define FRAME_ADDRESS() (std::uintptr_t(_AddressOfReturnAddress()) - sizeof(std::uintptr_t))
#define IS_WIN32() true
#define WIN32_LINUX(win32, linux) win32

#else

#define LINUX_ARGS(...) __VA_ARGS__
#define RETURN_ADDRESS() std::uintptr_t(__builtin_return_address(0))
#define FRAME_ADDRESS() std::uintptr_t(__builtin_frame_address(0))
#define IS_WIN32() false
#define WIN32_LINUX(win32, linux) linux

#endif

#include "../ImGui/imgui.h"
#include "InputUtil.h"

struct Key {
    constexpr Key(std::string_view name, int code) : name{ name }, code{ code } {}

    std::string_view name;
    int code;
};

// indices must match KeyBind::KeyCode enum
static constexpr auto keyMap = std::to_array<Key>({
    { "'", WIN32_LINUX(VK_OEM_7, SDL_SCANCODE_APOSTROPHE) },
    { ",", WIN32_LINUX(VK_OEM_COMMA, SDL_SCANCODE_COMMA) },
    { "-", WIN32_LINUX(VK_OEM_MINUS, SDL_SCANCODE_MINUS) },
    { ".", WIN32_LINUX(VK_OEM_PERIOD, SDL_SCANCODE_PERIOD) },
    { "/", WIN32_LINUX(VK_OEM_2, SDL_SCANCODE_SLASH) },
    { "0", WIN32_LINUX('0', SDL_SCANCODE_0) },
    { "1", WIN32_LINUX('1', SDL_SCANCODE_1) },
    { "2", WIN32_LINUX('2', SDL_SCANCODE_2) },
    { "3", WIN32_LINUX('3', SDL_SCANCODE_3) },
    { "4", WIN32_LINUX('4', SDL_SCANCODE_4) },
    { "5", WIN32_LINUX('5', SDL_SCANCODE_5) },
    { "6", WIN32_LINUX('6', SDL_SCANCODE_6) },
    { "7", WIN32_LINUX('7', SDL_SCANCODE_7) },
    { "8", WIN32_LINUX('8', SDL_SCANCODE_8) },
    { "9", WIN32_LINUX('9', SDL_SCANCODE_9) },
    { ";", WIN32_LINUX(VK_OEM_1, SDL_SCANCODE_SEMICOLON) },
    { "=", WIN32_LINUX(VK_OEM_PLUS, SDL_SCANCODE_EQUALS) },
    { "A", WIN32_LINUX('A', SDL_SCANCODE_A) },
    { "ADD", WIN32_LINUX(VK_ADD, SDL_SCANCODE_KP_PLUS) },
    { "B", WIN32_LINUX('B', SDL_SCANCODE_B) },
    { "BACKSPACE", WIN32_LINUX(VK_BACK, SDL_SCANCODE_BACKSPACE) },
    { "C", WIN32_LINUX('C', SDL_SCANCODE_C) },
    { "CAPSLOCK", WIN32_LINUX(VK_CAPITAL, SDL_SCANCODE_CAPSLOCK) },
    { "D", WIN32_LINUX('D', SDL_SCANCODE_D) },
    { "DECIMAL", WIN32_LINUX(VK_DECIMAL, SDL_SCANCODE_KP_DECIMAL) },
    { "DELETE", WIN32_LINUX(VK_DELETE, SDL_SCANCODE_DELETE) },
    { "DIVIDE", WIN32_LINUX(VK_DIVIDE, SDL_SCANCODE_KP_DIVIDE) },
    { "DOWN", WIN32_LINUX(VK_DOWN, SDL_SCANCODE_DOWN) },
    { "E", WIN32_LINUX('E', SDL_SCANCODE_E) },
    { "END", WIN32_LINUX(VK_END, SDL_SCANCODE_END) },
    { "ENTER", WIN32_LINUX(VK_RETURN, SDL_SCANCODE_RETURN) },
    { "F", WIN32_LINUX('F', SDL_SCANCODE_F) },
    { "F1", WIN32_LINUX(VK_F1, SDL_SCANCODE_F1) },
    { "F10", WIN32_LINUX(VK_F10, SDL_SCANCODE_F10) },
    { "F11", WIN32_LINUX(VK_F11, SDL_SCANCODE_F11) },
    { "F12", WIN32_LINUX(VK_F12, SDL_SCANCODE_F12) },
    { "F2", WIN32_LINUX(VK_F2, SDL_SCANCODE_F2) },
    { "F3", WIN32_LINUX(VK_F3, SDL_SCANCODE_F3) },
    { "F4", WIN32_LINUX(VK_F4, SDL_SCANCODE_F4) },
    { "F5", WIN32_LINUX(VK_F5, SDL_SCANCODE_F5) },
    { "F6", WIN32_LINUX(VK_F6, SDL_SCANCODE_F6) },
    { "F7", WIN32_LINUX(VK_F7, SDL_SCANCODE_F7) },
    { "F8", WIN32_LINUX(VK_F8, SDL_SCANCODE_F8) },
    { "F9", WIN32_LINUX(VK_F9, SDL_SCANCODE_F9) },
    { "G", WIN32_LINUX('G', SDL_SCANCODE_G) },
    { "H", WIN32_LINUX('H', SDL_SCANCODE_H) },
    { "HOME", WIN32_LINUX(VK_HOME, SDL_SCANCODE_HOME) },
    { "I", WIN32_LINUX('I', SDL_SCANCODE_I) },
    { "INSERT", WIN32_LINUX(VK_INSERT, SDL_SCANCODE_INSERT) },
    { "J", WIN32_LINUX('J', SDL_SCANCODE_J) },
    { "K", WIN32_LINUX('K', SDL_SCANCODE_K) },
    { "L", WIN32_LINUX('L', SDL_SCANCODE_L) },
    { "LALT", WIN32_LINUX(VK_LMENU, SDL_SCANCODE_LALT) },
    { "LCTRL", WIN32_LINUX(VK_LCONTROL, SDL_SCANCODE_LCTRL) },
    { "LEFT", WIN32_LINUX(VK_LEFT, SDL_SCANCODE_LEFT) },
    { "LSHIFT", WIN32_LINUX(VK_LSHIFT, SDL_SCANCODE_LSHIFT) },
    { "M", WIN32_LINUX('M', SDL_SCANCODE_M) },
    { "MOUSE1", 0 },
    { "MOUSE2", 1 },
    { "MOUSE3", 2 },
    { "MOUSE4", VK_XBUTTON1 },
    { "MOUSE5", VK_XBUTTON2 },
    { "MULTIPLY", WIN32_LINUX(VK_MULTIPLY, SDL_SCANCODE_KP_MULTIPLY) },
    { "MWHEEL_DOWN", 0 },
    { "MWHEEL_UP", 0 },
    { "N", WIN32_LINUX('N', SDL_SCANCODE_N) },
    { "NONE", 0 },
    { "NUMPAD_0", WIN32_LINUX(VK_NUMPAD0, SDL_SCANCODE_KP_0) },
    { "NUMPAD_1", WIN32_LINUX(VK_NUMPAD1, SDL_SCANCODE_KP_1) },
    { "NUMPAD_2", WIN32_LINUX(VK_NUMPAD2, SDL_SCANCODE_KP_2) },
    { "NUMPAD_3", WIN32_LINUX(VK_NUMPAD3, SDL_SCANCODE_KP_3) },
    { "NUMPAD_4", WIN32_LINUX(VK_NUMPAD4, SDL_SCANCODE_KP_4) },
    { "NUMPAD_5", WIN32_LINUX(VK_NUMPAD5, SDL_SCANCODE_KP_5) },
    { "NUMPAD_6", WIN32_LINUX(VK_NUMPAD6, SDL_SCANCODE_KP_6) },
    { "NUMPAD_7", WIN32_LINUX(VK_NUMPAD7, SDL_SCANCODE_KP_7) },
    { "NUMPAD_8", WIN32_LINUX(VK_NUMPAD8, SDL_SCANCODE_KP_8) },
    { "NUMPAD_9", WIN32_LINUX(VK_NUMPAD9, SDL_SCANCODE_KP_9) },
    { "O", WIN32_LINUX('O', SDL_SCANCODE_O) },
    { "P", WIN32_LINUX('P', SDL_SCANCODE_P) },
    { "PAGE_DOWN", WIN32_LINUX(VK_NEXT, SDL_SCANCODE_PAGEDOWN) },
    { "PAGE_UP", WIN32_LINUX(VK_PRIOR, SDL_SCANCODE_PAGEUP) },
    { "Q", WIN32_LINUX('Q', SDL_SCANCODE_Q) },
    { "R", WIN32_LINUX('R', SDL_SCANCODE_R) },
    { "RALT", WIN32_LINUX(VK_RMENU, SDL_SCANCODE_RALT) },
    { "RCTRL", WIN32_LINUX(VK_RCONTROL, SDL_SCANCODE_RCTRL) },
    { "RIGHT", WIN32_LINUX(VK_RIGHT, SDL_SCANCODE_RIGHT) },
    { "RSHIFT", WIN32_LINUX(VK_RSHIFT, SDL_SCANCODE_RSHIFT) },
    { "S", WIN32_LINUX('S', SDL_SCANCODE_S) },
    { "SPACE", WIN32_LINUX(VK_SPACE, SDL_SCANCODE_SPACE) },
    { "SUBTRACT", WIN32_LINUX(VK_SUBTRACT, SDL_SCANCODE_KP_MINUS) },
    { "T", WIN32_LINUX('T', SDL_SCANCODE_T) },
    { "TAB", WIN32_LINUX(VK_TAB, SDL_SCANCODE_TAB) },
    { "U", WIN32_LINUX('U', SDL_SCANCODE_U) },
    { "UP", WIN32_LINUX(VK_UP, SDL_SCANCODE_UP) },
    { "V", WIN32_LINUX('V', SDL_SCANCODE_V) },
    { "W", WIN32_LINUX('W', SDL_SCANCODE_W) },
    { "X", WIN32_LINUX('X', SDL_SCANCODE_X) },
    { "Y", WIN32_LINUX('Y', SDL_SCANCODE_Y) },
    { "Z", WIN32_LINUX('Z', SDL_SCANCODE_X) },
    { "[", WIN32_LINUX(VK_OEM_4, SDL_SCANCODE_LEFTBRACKET) },
    { "\\", WIN32_LINUX(VK_OEM_5, SDL_SCANCODE_BACKSLASH) },
    { "]", WIN32_LINUX(VK_OEM_6, SDL_SCANCODE_RIGHTBRACKET) },
    { "`", WIN32_LINUX(VK_OEM_3, SDL_SCANCODE_GRAVE) },
    });

ImGuiKey KeyCodeToImGuiKey(int code) {
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

static_assert(keyMap.size() == KeyBind::MAX);
static_assert(std::ranges::is_sorted(keyMap, {}, & Key::name));

KeyBind::KeyBind(KeyCode keyCode) noexcept : keyCode{ static_cast<std::size_t>(keyCode) < keyMap.size() ? keyCode : KeyCode::NONE } {}

KeyBind::KeyBind(const char* keyName) noexcept
{
    if (const auto it = std::ranges::lower_bound(keyMap, keyName, {}, &Key::name); it != keyMap.end() && it->name == keyName)
        keyCode = static_cast<KeyCode>(std::distance(keyMap.begin(), it));
    else
        keyCode = KeyCode::NONE;
}

const char* KeyBind::toString() const noexcept
{
    if (keyCode == KeyCode::NONE) return "None";
    if (static_cast<size_t>(keyCode) < keyMap.size()) {
        return keyMap[static_cast<size_t>(keyCode)].name.data();
    }
    return "Unknown";
}

const int KeyBind::toInt() const noexcept
{
    return keyMap[static_cast<std::size_t>(keyCode) < keyMap.size() ? keyCode : KeyCode::NONE].code; //Key::code
}

bool KeyBind::isPressed() const noexcept
{
    if (keyCode == KeyCode::NONE)
        return false;

    if (keyCode == KeyCode::MOUSEWHEEL_DOWN)
        return ImGui::GetIO().MouseWheel < 0.0f;

    if (keyCode == KeyCode::MOUSEWHEEL_UP)
        return ImGui::GetIO().MouseWheel > 0.0f;

    if (keyCode >= KeyCode::MOUSE1 && keyCode <= KeyCode::MOUSE5)
        return ImGui::IsMouseClicked(keyMap[keyCode].code);

    return static_cast<std::size_t>(keyCode) < keyMap.size() && ImGui::IsKeyPressed(KeyCodeToImGuiKey(keyMap[keyCode].code), false);
}

bool KeyBind::isDown() const noexcept
{
    if (keyCode == KeyCode::NONE)
        return false;

    if (keyCode == KeyCode::MOUSEWHEEL_DOWN)
        return ImGui::GetIO().MouseWheel < 0.0f;

    if (keyCode == KeyCode::MOUSEWHEEL_UP)
        return ImGui::GetIO().MouseWheel > 0.0f;

    if (keyCode >= KeyCode::MOUSE1 && keyCode <= KeyCode::MOUSE5)
        return ImGui::IsMouseDown(keyMap[keyCode].code);

    return static_cast<std::size_t>(keyCode) < keyMap.size() && ImGui::IsKeyDown(KeyCodeToImGuiKey(keyMap[keyCode].code));
}

bool KeyBind::setToPressedKey() noexcept
{
    // 1. Проверка Escape
    if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
        keyCode = KeyCode::NONE;
        return true;
    }

    // 2. Проверка колеса мыши
    const float wheel = ImGui::GetIO().MouseWheel;
    if (wheel < 0.0f) {
        keyCode = KeyCode::MOUSEWHEEL_DOWN;
        return true;
    }
    if (wheel > 0.0f) {
        keyCode = KeyCode::MOUSEWHEEL_UP;
        return true;
    }

    // 3. Проверка кнопок мыши (исправленная версия)
    constexpr std::pair<ImGuiMouseButton, KeyCode> mouseButtons[] = {
        {ImGuiMouseButton_Left,   KeyCode::MOUSE1},
        {ImGuiMouseButton_Right,  KeyCode::MOUSE2},
        {ImGuiMouseButton_Middle, KeyCode::MOUSE3},
        {3,                       KeyCode::MOUSE4},  // XBUTTON1
        {4,                       KeyCode::MOUSE5}   // XBUTTON2
    };

    for (const auto& [imguiBtn, ourBtn] : mouseButtons) {
        if (ImGui::IsMouseClicked(imguiBtn)) {
            keyCode = ourBtn;
            return true;
        }
    }

    // 4. Проверка специальных клавиш через GetKeyState
    auto checkKeyState = [this](KeyCode code) {
        if (static_cast<size_t>(code) < keyMap.size()) {
            return (GetKeyState(keyMap[static_cast<size_t>(code)].code) & 0x8000) != 0;
        }
        return false;
        };

    if (checkKeyState(KeyCode::LSHIFT)) {
        keyCode = KeyCode::LSHIFT;
        return true;
    }
    if (checkKeyState(KeyCode::LALT)) {
        keyCode = KeyCode::LALT;
        return true;
    }
    if (checkKeyState(KeyCode::LCTRL)) {
        keyCode = KeyCode::LCTRL;
        return true;
    }

    // 5. Проверка остальных клавиш
    for (int i = ImGuiKey_NamedKey_BEGIN; i < ImGuiKey_NamedKey_END; ++i) {
        const auto imguiKey = static_cast<ImGuiKey>(i);
        if (ImGui::IsKeyPressed(imguiKey, false)) {
            // Пропускаем модификаторы
            if (imguiKey == ImGuiKey_ModShift || imguiKey == ImGuiKey_ModCtrl ||
                imguiKey == ImGuiKey_ModAlt || imguiKey == ImGuiKey_ModSuper) {
                continue;
            }

            // Конвертация в наш KeyCode
            for (size_t j = 0; j < keyMap.size(); ++j) {
                if (KeyCodeToImGuiKey(keyMap[j].code) == imguiKey) {
                    keyCode = static_cast<KeyCode>(j);

                    // Обработка AltGr
                    if (keyCode == KeyCode::LCTRL && ImGui::IsKeyDown(ImGuiKey_RightAlt)) {
                        keyCode = KeyCode::RALT;
                    }
                    return true;
                }
            }
        }
    }

    return false;
}


void KeyBindToggle::handleToggle() noexcept
{
    if (isPressed())
        toggledOn = !toggledOn;
}
