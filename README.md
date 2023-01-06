# ImGui Custom HotKeys / KeyBinds
Better function for ImGui HotKeys (ImGui Keybinds)<br>
Recommended version of ImGui: 1.83 WIP

<img src="https://cdn.discordapp.com/attachments/439961603744464899/1060746182399361024/image.png"></img>

### Usage
First, add the files to your project, then add the code from the example to your main file with the ImGui loop<br>

Please star and fork this repo!

#### Example
```cpp
#include "InputUtil.h" // including custom hotkey files

bool activate = 0; // variable for action (for example activation)
KeyBindToggle activate_bind = KeyBindToggle(KeyBind::KeyCode::INSERT); // key variable, selected default hotkey (insert)

// in main gui while to check hotkeys
if (GetKeyState(activate_bind.toInt()) & 0x8000) { if (activate == 0) activate = 1; else activate = 0; Beep(1000, 150); Sleep(200); }

// in main gui     ("label", variable_keybind, margin)
ImGuiCustom::hotkey("Keybind:", activate_bind, 100.0f);
```

## Me
in **Discord**: xvorost#5394<br>
My BTC address: bc1qa2gj7vqpaaeajx47y5guwdr9usv6kh253j4v34
