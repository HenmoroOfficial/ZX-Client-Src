----description:This script file is for shortcuts.
----author: wangdongliang


------实时跟踪界面中某些控件用于表现快捷键信息的悬浮提示和文本信息
------逻辑快捷键ID   指向的界面  指向的控件	是否悬浮 是否文本  悬浮前缀 文本前缀
------lk（逻辑快捷键ID）唯一，指向一个特定的功能，具体的逻辑快捷键ID，可参考c++代码中的枚举定义，此处的lk值应与代码中的枚举值一一对应
------dlg（指向的界面）通常为界面的名称，control（指向的控件）通常为控件的名称。
------hint——是否跟踪悬浮提示，text——是否跟踪文本信息
------prehint——悬浮前缀，pretext——文本前缀
------此处对快捷键的悬浮和文本有统一的格式要求，即快捷键的信息格式为"xxxxxxx(ctrl+A)yyyyyyy"，
------"xxxxxxx"为prehint或pretext的值，可于此脚本中配置，"yyyyyyy"为posthint或posttext的值
------"(ctrl+A)"为快捷键的信息，快捷键可于程序中取得，其中括号的颜色和快捷键的颜色如无特殊需求，暂时在程序中写死


ShorcutsUIInfo = {}
 
ShorcutsUIInfo['包裹'] = {lk = 60, dlg = "Win_Main", control = "Btn_Inventory", hint = true, text = false, prehint = "^ffcb4a包裹装备", posthint = "", pretext = "", posttext = ""}
ShorcutsUIInfo['人物'] = {lk = 50, dlg = "Win_Main", control = "Btn_Character", hint = true, text = false, prehint = "^ffcb4a人物属性", posthint = "", pretext = "", posttext = ""}
ShorcutsUIInfo['技能'] = {lk = 61, dlg = "Win_Main", control = "Btn_Skill", hint = true, text = false, prehint = "^ffcb4a技能", posthint = "", pretext = "", posttext = ""}
ShorcutsUIInfo['任务'] = {lk = 62, dlg = "Win_Main", control = "Btn_Quest", hint = true, text = false, prehint = "^ffcb4a任务", posthint = "", pretext = "", posttext = ""}
ShorcutsUIInfo['天帝宝库'] = {lk = 67, dlg = "Win_QuickBar8H_1", control = "Btn_Qshop", hint = true, text = false, prehint = "^ffcb4a天帝宝库", posthint = "", pretext = "", posttext = ""}

 
function ShorcutsUIInfo:GetSelf()
	return self;
end
