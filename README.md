# KeyLogicExpansion
A SKSE plugin which make key input smooth and modern.

---

## For Mod User
### What Can KLE do
Separeate leftAttack, RightAttack Key To Three Key: NormalAttack PowerAttack, Block. And this Won't affect euqip item or spell in Menu.
To Enable this Please Make Sure NormalAttack, PowerAttack, Block Key NOT zero.Default MouseLeft, MouseRight, Tab, Same as Nioh.

Separate Open TweenMenu Key and It's function of Close Menu.
To Enable this Please Make Sure AltTweenMenu Key NOT zero.

Separate TogglePOV Key and It's function of Mark As Favorite.
To Enable this Please Make Sure AltTogglePOV Key NOT zero.

Seprate ZoomIn and ZoomOut and their function Of Scroll in Menu.
To Enable this Please Make Sure AltZoomIn or AltZoomOut Key NOT zero.

Change sprint or sneak by holding sprint or sneak Key.
To Enable this Please Make Sure EnableHoldSprint or EnableHoldSneak True.

Allow you press sprint when you stop moving and spring will start when you start moving, Vanilla Can't do that.

Allow you Reverse HorseAttack.When Riding in Vanilla Game, Press LeftAttack Will Attack Right, Press RightAttack Will Attack Left, enable this will make Press LeftAttack Will Attack Left, Press RightAttack Will Attack Right.

Compatible with MCO, BFCO, Elden Parry about NormalAttack, PowerAttack, Block, BlockBash. And SpecialAttack ComboAttack Sprint and jumpAttack(Not Test, if succeed please tell me) for BFCO.
Add a Pre Input where the target action can be changed and regretted.
Add a Special Control Mod When lefthand is staff/magic and righthand is weapen/fist. In this Situation You can only use magic/staff by pressing MagicModifier Key and PowerAttack.

Add a Modifier to Every(Actually Not, If you find a modifer not work please tell me) Separate Vanilla Key.If you set A Key's Modifier , you can only activate it by pressing Modifier first.
You can Set it a very big number to disable Vanilla key, such as 10000.

Load Custom Input from other Mod Author.

## For Mod Author

### Trigger, Modifier and Priority 
Trigger means when press this key, KLE will check Custom Input about it.
Modifier not zero means KLE will cheak whether Modifier pressed first.
Priority means when A trigger has many conditions and events, which event will be process(When condition is true).
Another Word, KLE will process the event which condition result is True and has Biggest Priority.

### How to Add a Custom Input
You can write a ini file, and this is a example in "SKSE/Plugins/KeyLogicExpansion".You can copy something from it.

ATTENTION: LowerCase and UpperCase Sensitive in Custom Input.

### Input Mode
There is 3 Input Mode Supported by KLE.
Click, DoubleClick, Hold.
Click means when you press this key, KLE will try to do something according conditon,
DoubleClick means when you press this key double in a time setted by KLE Config File.
Hold means when you press this key at least for a time setted by KLE Config File.

### Condition

### Event