# ImGui Custom HotKeys / KeyBinds
Better function for ImGui HotKeys (ImGui Keybinds)<br>
Recommended version of ImGui: 1.83 WIP

![image](https://github.com/user-attachments/assets/77c7f751-0412-41dc-8bbf-bc2fa9aceeea)

### Usage
First, add the files to your project, then add the code from the example to your main file with the ImGui / Function loop<br>

### Please star and fork this repo!

#### Example
```cpp
#include "InputUtil.h" // including custom hotkey files
#include "Hotkey.h" // including custom hotkey files

bool activate = false; // variable for action (for example activation)
bool activate_state = false; // variable for action_state (for example activation state)
KeyBindToggle activate_bind = KeyBindToggle(KeyBind::KeyCode::INSERT); // key variable, selected default hotkey (Insert)
KeyBindToggle activate_key = KeyBindToggle(KeyBind::KeyCode::CAPSLOCK); // key variable, selected default hotkey (CapsLock)

// in main gui while to check hotkeys true:false
ImGuiCustom::CheckBindKey(activate_bind, activate);

// in function while to check hotkeys
if (ImGuiCustom::CheckBindKey(activate_key)) activate_state = !activate_state;

// in function while to check hotkeys HOLD
if (ImGuiCustom::CheckHoldBindKey(activate_key)) activate_state = !activate_state;

// in function while to check hotkeys STATE
if (ImGuiCustom::CheckStateBindKey(activate_key)) activate_state = !activate_state;

// in main gui     ("label", variable_keybind, state, margin, size)
ImGui::Text("KeyBind:"); ImGui::SameLine();
ImGuiCustom::hotkey("##keybind_1", activate_bind, activate_state, 100.0f, { 100.0f, 20.0f });
```

## Me
in **Discord**: xvorost#5394<br>
in **Telegram**: X_vorost
My BTC address: bc1qa2gj7vqpaaeajx47y5guwdr9usv6kh253j4v34
