# ImGui-Custom-HotKeys
Better function for ImGui HotKeys<be>

### Usage
<pre language="cpp">
#include "InputUtil.h" // including custom hotkey files

bool activate = 0;
KeyBindToggle activate_bind = KeyBindToggle(KeyBind::KeyCode::INSERT); // key variable, selected default hotkey (insert)

// in main gui while to check hotkeys
if (GetKeyState(activate_bind.toInt()) & 0x8000) { if (activate == 0) activate = 1; else activate = 0; Beep(1000, 150); Sleep(200); }

// in main gui
ImGuiCustom::hotkey("Keybind:", activate_bind, 100.0f);
</pre>

## Me
in **Discord**: xvorost#5394
