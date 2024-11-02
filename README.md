# KeyLogicExpansion
A SKSE plugin that makes key input smooth and modern.

---

## For Mod Users

### What Can KLE Do?

- **Separate Left Attack and Right Attack Keys** into three keys: Normal Attack, Power Attack, and Block. This won't affect equipping items or spells in the menu.
  - To enable this, please ensure that the Normal Attack, Power Attack, and Block keys are not set to zero. The default keys are Mouse Left, Mouse Right, and Tab, similar to Nioh.

- **Separate Open TweenMenu Key** and its function to close the menu.
  - To enable this, please ensure that the AltTweenMenu key is not set to zero.

- **Separate Toggle POV Key** and its function to mark an item as favorite.
  - To enable this, please ensure that the AltTogglePOV key is not set to zero.

- **Separate Zoom In and Zoom Out** and their function for scrolling in the menu.
  - To enable this, please ensure that the AltZoomIn or AltZoomOut key is not set to zero.

- Change sprint or sneak by holding the sprint or sneak key.
  - To enable this, please ensure that EnableHoldSprint or EnableHoldSneak is set to true.

- Allow you to press sprint when you stop moving, and it will start when you begin moving again. Vanilla Skyrim cannot do this.

- Allow you to reverse horse attacks. In the vanilla game, pressing Left Attack will attack right, and pressing Right Attack will attack left. Enable this to make pressing Left Attack attack left and pressing Right Attack attack right.

- Compatible with MCO, BFCO, and Elden Parry for Normal Attack, Power Attack, Block, and Block Bash. Also compatible with Special Attack, Combo Attack, Sprint, and Jump Attack (not tested; if successful, please let me know) for BFCO.

- Add a pre-input where the target action can be changed and regretted.

- Add a special control mod when the left hand is a staff/magic and the right hand is a weapon/fist. In this situation, you can only use magic/staff by pressing the Magic Modifier key and Power Attack.

- Add a modifier to every separate vanilla key. If you set a modifier for a key, you can only activate it by pressing the modifier first. You can set it to a very high number to disable the vanilla key, such as 10000.

- Load custom input from other mod authors.


## For Mod Authors

### Trigger, Modifier, and Priority

- **Trigger** means that when this key is pressed, KLE will check for custom input related to it.
- **Modifier** means that if it is not zero, KLE will check whether the modifier key is pressed first.
- **Priority** means that when a trigger has many conditions and events, it determines which event will be processed first (when the condition is true). In other words, KLE will process the event with the highest priority whose condition is true.

### How to Add a Custom Input
You can write an INI file. Here is an example located in "SKSE/Plugins/KeyLogicExpansion". You can copy and modify it as needed.

> **Attention:** Custom input is case-sensitive.

### Input Modes
KLE supports three input modes:
- Click: When you press this key, KLE will try to perform an action based on the condition.
- Double Click: When you double-click this key within a set time frame specified in the KLE Config File.
- Hold: When you press and hold this key for a duration specified in the KLE Config File.

### Condition
(TODO: Add conditions section)

### Event
(TODO: Add events section)

---
