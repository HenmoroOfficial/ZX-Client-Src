#include "skill.h"

namespace GNET
{

int norm(float f) { return (int)(f+0.00001)>=1?1:0; }
int norm(double f) { return (int)(f+0.00001)>=1?1:0; }
int norm(int n) { return n>=1?1:0; }

Rank Rank::instance;
Rank::Rank()    
{       
	basemap[0]  = 0;
	basemap[1]  = 0;
	basemap[2]  = 1;
	basemap[3]  = 2;
	basemap[4]  = 0;
	basemap[5]  = 4;
	basemap[6]  = 5;
	basemap[7]  = 0;
	basemap[8]  = 7;
	basemap[9]  = 8;
	basemap[10] = 0;
	basemap[11] = 10;
	basemap[12] = 11;
	basemap[13] = 3;
	basemap[14] = 13;
	basemap[16] = 6;
	basemap[17] = 16;
	basemap[19] = 9;
	basemap[20] = 19;
	basemap[22] = 12;
	basemap[23] = 22;
	basemap[25] = 0;
	basemap[26] = 25;
	basemap[27] = 26;
	basemap[28] = 27;
	basemap[29] = 28;
    basemap[33] = -1;
    basemap[34] = 33;
    basemap[35] = 34;
    basemap[36] = 35;
    basemap[37] = 36;
    basemap[39] = -1;
    basemap[40] = 39;
    basemap[41] = 40;
    basemap[42] = 41;
    basemap[43] = 42;
	basemap[45] = -1;
	basemap[46] = 45;
	basemap[47] = 46;
	basemap[48] = 47;
	basemap[49] = 48;
	basemap[51] = -1;
	basemap[52] = 51;
	basemap[53] = 52;
	basemap[54] = 53;
	basemap[55] = 54;

	basemap[56] = -1;
	basemap[57] = 56;
	basemap[58] = 57;
	basemap[59] = 58;
	basemap[60] = 59;


	basemap[64] = 0;
    basemap[65] = 64;
    basemap[66] = 65;
    basemap[67] = 66;
    basemap[68] = 67;

    basemap[96] = -1;
    basemap[97] = 96;
    basemap[98] = 97;
    basemap[99] = 98;
    basemap[100] = 99;

	basemap[102] = -1;
	basemap[103] = 102;
	basemap[104] = 103;
	basemap[105] = 104;
	basemap[106] = 105;

	basemap[108] = -1;
	basemap[109] = 108;
	basemap[110] = 109;
	basemap[111] = 110;
	basemap[112] = 111;
}
int Rank::GetBase(int rank)
{       
	return instance.basemap[rank];
}
bool Rank::SameBranch(int rank, int root)
{
	if(root<0)
		return false;
	while(rank>=root)
	{
		if(rank==root)
			return true;
		rank = instance.basemap[rank];
	}
	return false;
}

TeamStateImp::Map     TeamStateImp::stubmap;
VisibleStateImp::Map  VisibleStateImp::stubmap;

VisibleStateImp _stub_vstate1(1, L"保护盾","保护盾.gfx","");
VisibleStateImp _stub_vstate2(2, L"保护盾2","保护盾2.gfx","");
VisibleStateImp _stub_vstate3(3, L"冰冻","冰冻.gfx","HH_Spine");
VisibleStateImp _stub_vstate4(4, L"定身","定身.gfx","");
VisibleStateImp _stub_vstate5(5, L"神佑","神佑.gfx","");
VisibleStateImp _stub_vstate6(6, L"昏睡","昏睡.gfx","HH_Head");
VisibleStateImp _stub_vstate7(7, L"媚惑","媚惑.gfx","HH_Head");
VisibleStateImp _stub_vstate8(8, L"气血持续损失","气血持续损失.gfx","HH_Spine");
VisibleStateImp _stub_vstate9(9, L"气血逐渐回复","气血逐渐回复.gfx","");
VisibleStateImp _stub_vstate10(10, L"无敌","无敌2.gfx","");
VisibleStateImp _stub_vstate11(11, L"虚弱","虚弱.gfx","HH_Spine");
VisibleStateImp _stub_vstate12(12, L"真气持续损失","真气持续损失.gfx","HH_Spine");
VisibleStateImp _stub_vstate13(13, L"真气逐渐回复","真气逐渐回复.gfx","");
VisibleStateImp _stub_vstate14(14, L"击晕","击晕.gfx","HH_Head");
VisibleStateImp _stub_vstate15(15, L"任务光效","任务光效.gfx","");
VisibleStateImp _stub_vstate16(16, L"刷任务光效","刷任务标志.gfx","HH_Head");
VisibleStateImp _stub_vstate17(17, L"天玄冰","天玄冰.gfx","HH_Spine");
VisibleStateImp _stub_vstate18(18, L"忘情","风月.gfx","");
VisibleStateImp _stub_vstate19(19, L"天机印","天机印.gfx","");
VisibleStateImp _stub_vstate20(20, L"苍劫阵","苍劫阵.gfx","");
VisibleStateImp _stub_vstate21(21, L"慈航法愿","慈航法愿.gfx","");
VisibleStateImp _stub_vstate22(22, L"巫毒斗瘴","巫毒斗瘴.gfx","");
VisibleStateImp _stub_vstate23(23, L"宠物通灵","宠物通灵.gfx","HH_Spine");
VisibleStateImp _stub_vstate24(24, L"宠物御宝","宠物御宝.gfx","HH_Spine");
VisibleStateImp _stub_vstate25(25, L"捕捉","捕捉.gfx","");
VisibleStateImp _stub_vstate26(26, L"恐惧","恐惧.gfx","");
VisibleStateImp _stub_vstate27(27, L"阳春白雪","阳春白雪.gfx","");
VisibleStateImp _stub_vstate28(28, L"苏武牧羊","苏武牧羊.gfx","");
VisibleStateImp _stub_vstate29(29, L"悬崖飞瀑","悬崖飞瀑.gfx","");
VisibleStateImp _stub_vstate30(30, L"十面埋伏","十面埋伏.gfx","");
VisibleStateImp _stub_vstate31(31, L"空山鸟语","空山鸟语.gfx","");
VisibleStateImp _stub_vstate32(32, L"平沙落雁","平沙落雁.gfx","");

VisibleStateImp _stub_vstate33(33, L"仙_星辰爆发","仙_星辰爆发.gfx","");
VisibleStateImp _stub_vstate34(34, L"佛_神圣暴发","佛_神圣暴发.gfx","");
VisibleStateImp _stub_vstate35(35, L"魔_紫芒爆发","魔_紫芒爆发.gfx","");
VisibleStateImp _stub_vstate36(36, L"魔_燃烧黑","魔_燃烧黑.gfx","");
VisibleStateImp _stub_vstate37(37, L"魔_燃烧","魔_燃烧.gfx","");
VisibleStateImp _stub_vstate38(38, L"仙_冻结冰刺","仙_冻结冰刺.gfx","");
VisibleStateImp _stub_vstate39(39, L"仙_冻伤","仙_冻伤.gfx","");
VisibleStateImp _stub_vstate40(40, L"仙_冻伤红","仙_冻伤红.gfx","");
VisibleStateImp _stub_vstate41(41, L"逆转正面效果","逆转正面效果.gfx","HH_Spine");
VisibleStateImp _stub_vstate42(42, L"逆转负面效果","逆转负面效果.gfx","HH_Spine");
VisibleStateImp _stub_vstate43(43, L"焚香灵力精研_绿色","焚香灵力精研_绿色.gfx","HH_Spine");
VisibleStateImp _stub_vstate44(44, L"焚香灵力精研_红色","焚香灵力精研_红色.gfx","HH_Spine");
VisibleStateImp _stub_vstate45(45, L"焚香灵力精研_紫色","焚香灵力精研_紫色.gfx","HH_Spine");
VisibleStateImp _stub_vstate46(46, L"焚香灵力精研_橙色","焚香灵力精研_橙色.gfx","HH_Spine");
VisibleStateImp _stub_vstate47(47, L"焚香灵力精研_蓝色","焚香灵力精研_蓝色.gfx","HH_Spine");
VisibleStateImp _stub_vstate48(48, L"焚香灼烧效果","焚香灼烧效果.gfx","");
VisibleStateImp _stub_vstate49(49, L"火狩护主","火狩护主.gfx","");
VisibleStateImp _stub_vstate50(50, L"保护盾","护盾气罩效果01.gfx","");
VisibleStateImp _stub_vstate51(51, L"高等暗使","暗翅膀.gfx","HH_chibang");
VisibleStateImp _stub_vstate52(52, L"高等光使","光翅膀.gfx","HH_chibang");
//VisibleStateImp _stub_vstate53(53, L"镜像","轩辕_幻象法阵效果.gfx","");
VisibleStateImp _stub_vstate55(55, L"","玫瑰雨buff循环.gfx","");


TeamStateImp _stub_tstate1( 1,L"眩晕", "眩晕.dds");
TeamStateImp _stub_tstate2( 2,L"昏睡", "昏睡.dds");
TeamStateImp _stub_tstate3( 3,L"虚弱", "虚弱.dds");
TeamStateImp _stub_tstate4( 4,L"魅惑", "魅惑.dds");
TeamStateImp _stub_tstate5( 5,L"定身", "定身.dds");

TeamStateImp _stub_tstate6( 6,L"羁绊", "羁绊.dds");
TeamStateImp _stub_tstate7( 7,L"重斩", "重斩.dds");
TeamStateImp _stub_tstate8( 8,L"坚强", "坚强.dds");
TeamStateImp _stub_tstate9( 9,L"猛击", "猛击.dds");
TeamStateImp _stub_tstate10( 10,L"坚韧", "坚韧.dds");

TeamStateImp _stub_tstate11( 11,L"眩晕抗性上升", "眩晕抗性上升.dds");
TeamStateImp _stub_tstate12( 12,L"昏睡抗性上升", "昏睡抗性上升.dds");
TeamStateImp _stub_tstate13( 13,L"虚弱抗性上升", "虚弱抗性上升.dds");
TeamStateImp _stub_tstate14( 14,L"魅惑抗性上升", "魅惑抗性上升.dds");
TeamStateImp _stub_tstate15( 15,L"定身抗性上升", "定身抗性上升.dds");

TeamStateImp _stub_tstate16( 16,L"眩晕抗性下降", "眩晕抗性下降.dds");
TeamStateImp _stub_tstate17( 17,L"昏睡抗性下降", "昏睡抗性下降.dds");
TeamStateImp _stub_tstate18( 18,L"虚弱抗性下降", "虚弱抗性下降.dds");
TeamStateImp _stub_tstate19( 19,L"魅惑抗性下降", "魅惑抗性下降.dds");
TeamStateImp _stub_tstate20( 20,L"定身抗性下降", "定身抗性下降.dds");

TeamStateImp _stub_tstate21( 21,L"气血满盈", "气血满盈.dds");
TeamStateImp _stub_tstate22( 22,L"真气满盈", "真气满盈.dds");
TeamStateImp _stub_tstate23( 23,L"气血充沛", "气血充沛.dds");
TeamStateImp _stub_tstate24( 24,L"真气充沛", "真气充沛.dds");
TeamStateImp _stub_tstate25( 25,L"迅疾", "迅疾.dds");

TeamStateImp _stub_tstate26( 26,L"迥避", "迥避.dds");
TeamStateImp _stub_tstate27( 27,L"活血", "活血.dds");
TeamStateImp _stub_tstate28( 28,L"狂热", "狂热.dds");
TeamStateImp _stub_tstate29( 29,L"气血流失", "气血流失.dds");
TeamStateImp _stub_tstate30( 30,L"真气涣散", "真气涣散.dds");

TeamStateImp _stub_tstate31( 31,L"气血回复", "气血回复.dds");
TeamStateImp _stub_tstate32( 32,L"真气聚合", "真气聚合.dds");
TeamStateImp _stub_tstate33( 33,L"聚神", "聚神.dds");
TeamStateImp _stub_tstate34( 34,L"压制", "压制.dds");
TeamStateImp _stub_tstate35( 35,L"击破", "击破.dds");

TeamStateImp _stub_tstate36( 36,L"免疫眩晕", "免疫眩晕.dds");
TeamStateImp _stub_tstate37( 37,L"免疫昏睡", "免疫昏睡.dds");
TeamStateImp _stub_tstate38( 38,L"免疫魅惑", "免疫魅惑.dds");
TeamStateImp _stub_tstate39( 39,L"免疫虚弱", "免疫虚弱.dds");
TeamStateImp _stub_tstate40( 40,L"免疫定身", "免疫定身.dds");

TeamStateImp _stub_tstate41( 41,L"免疫羁绊", "免疫羁绊.dds");
TeamStateImp _stub_tstate42( 42,L"创伤", "创伤.dds");
TeamStateImp _stub_tstate43( 43,L"固守", "固守.dds");
TeamStateImp _stub_tstate44( 44,L"顽强", "顽强.dds");
TeamStateImp _stub_tstate45( 45,L"神佑", "神佑.dds");

TeamStateImp _stub_tstate46( 46,L"坚守", "坚守.dds");
TeamStateImp _stub_tstate47( 47,L"无敌", "无敌.dds");
TeamStateImp _stub_tstate48( 48,L"醉酒", "醉酒.dds");
TeamStateImp _stub_tstate49( 49,L"辣", "辣.dds");

TeamStateImp _stub_tstate50( 50,L"气血凝滞", "气血凝滞.dds");
TeamStateImp _stub_tstate51( 51,L"真气凝滞", "真气凝滞.dds");
TeamStateImp _stub_tstate52( 52,L"力量摧残", "力量摧残.dds");
TeamStateImp _stub_tstate53( 53,L"迷惘诅咒", "迷惘诅咒.dds");
TeamStateImp _stub_tstate54( 54,L"迟钝诅咒", "迟钝诅咒.dds");
TeamStateImp _stub_tstate55( 55,L"狂暴", "狂暴.dds");
TeamStateImp _stub_tstate56( 56,L"混乱诅咒", "混乱诅咒.dds");
TeamStateImp _stub_tstate57( 57,L"蓄势", "蓄势.dds");
TeamStateImp _stub_tstate58( 58,L"抗性强化", "抗性强化.dds");
TeamStateImp _stub_tstate59( 59,L"抗性增加", "抗性增加.dds");
TeamStateImp _stub_tstate60( 60,L"抗性削弱", "抗性削弱.dds");
TeamStateImp _stub_tstate61( 61,L"抗性下降", "抗性下降.dds");
TeamStateImp _stub_tstate62( 62,L"专注", "专注.dds");
TeamStateImp _stub_tstate63( 63,L"信念诅咒", "信念诅咒.dds");
TeamStateImp _stub_tstate64( 64,L"反射", "反射.dds");
TeamStateImp _stub_tstate65( 65,L"粉碎", "粉碎.dds");
TeamStateImp _stub_tstate66( 66,L"噬血", "噬血.dds");
TeamStateImp _stub_tstate67( 67,L"魔心", "魔心.dds");
TeamStateImp _stub_tstate68( 68,L"庇护", "庇护.dds");
TeamStateImp _stub_tstate69( 69,L"破甲诅咒", "破甲诅咒.dds");
TeamStateImp _stub_tstate70( 70,L"疲劳诅咒", "疲劳诅咒.dds");
TeamStateImp _stub_tstate71( 71,L"腐蚀诅咒", "腐蚀诅咒.dds");
TeamStateImp _stub_tstate72( 72,L"衰老诅咒", "衰老诅咒.dds");
TeamStateImp _stub_tstate73( 73,L"同生共死", "同生共死.dds");
TeamStateImp _stub_tstate74( 74,L"众志成城", "众志成城.dds");
TeamStateImp _stub_tstate75( 75,L"群策群力", "群策群力.dds");
TeamStateImp _stub_tstate76( 76,L"偏斜诅咒", "偏斜诅咒.dds");
TeamStateImp _stub_tstate77( 77,L"意志摧残", "意志摧残.dds");
TeamStateImp _stub_tstate78( 78,L"冰壳", "玄冰.dds");
TeamStateImp _stub_tstate79( 79,L"抗性汇聚", "抗性汇聚.dds");
TeamStateImp _stub_tstate80( 80,L"抗性分散", "抗性分散.dds");
TeamStateImp _stub_tstate81( 81,L"空手", "空手.dds");
TeamStateImp _stub_tstate82( 82,L"魔法盾", "魔法盾.dds");
TeamStateImp _stub_tstate83( 83,L"保护罩", "保护罩.dds");
TeamStateImp _stub_tstate84( 84,L"极限防护", "随机减伤害百分比.dds");
TeamStateImp _stub_tstate85( 85,L"禁食", "禁食.dds");
TeamStateImp _stub_tstate86( 86,L"增加爆击百分比", "增加爆击百分比.dds");
TeamStateImp _stub_tstate87( 87,L"怨念羁縻", "怨念羁縻.dds");
TeamStateImp _stub_tstate88( 88,L"孤寂", "孤寂.dds");
TeamStateImp _stub_tstate89( 89,L"折磨", "折磨.dds");
TeamStateImp _stub_tstate90( 90,L"迷茫", "迷茫.dds");
TeamStateImp _stub_tstate91( 91,L"困顿", "困顿.dds");
TeamStateImp _stub_tstate92( 92,L"魅心", "魅心.dds");
TeamStateImp _stub_tstate93( 93,L"昏厥", "昏厥.dds");
TeamStateImp _stub_tstate94( 94,L"蛊痛", "蛊痛.dds");
TeamStateImp _stub_tstate95( 95,L"蛊伤", "蛊伤.dds");
TeamStateImp _stub_tstate96( 96,L"蛊毒", "蛊毒.dds");
TeamStateImp _stub_tstate97( 97,L"魔魂姿态", "魔魂姿态.dds");
TeamStateImp _stub_tstate98( 98,L"蛊王姿态", "蛊王姿态.dds");
TeamStateImp _stub_tstate99( 99,L"闪避秘言", "闪避秘言.dds");
TeamStateImp _stub_tstate100( 100,L"回春秘言", "回春秘言.dds");
TeamStateImp _stub_tstate101( 101,L"命运祝福", "命运祝福.dds");
TeamStateImp _stub_tstate102( 102,L"荆棘护体", "荆棘护体.dds");
TeamStateImp _stub_tstate103( 103,L"死亡诅咒", "死亡诅咒.dds");
TeamStateImp _stub_tstate104( 104,L"会心一击", "会心一击.dds");
TeamStateImp _stub_tstate105( 105,L"破魔诅咒", "破魔诅咒.dds");
TeamStateImp _stub_tstate106( 106,L"轻盈护体", "轻盈护体.dds");
TeamStateImp _stub_tstate107( 107,L"迟钝诅咒", "迟钝诅咒.dds");
TeamStateImp _stub_tstate108( 108,L"厄运诅咒", "厄运诅咒.dds");
TeamStateImp _stub_tstate109( 109,L"幸运祝福", "幸运祝福.dds");
TeamStateImp _stub_tstate110( 110,L"雷霆诅咒", "雷霆诅咒.dds");
TeamStateImp _stub_tstate111( 111,L"天佑祝福", "天佑祝福.dds");
TeamStateImp _stub_tstate112( 112,L"众望所归", "众望所归.dds");
TeamStateImp _stub_tstate113( 113,L"先知先觉", "先知先觉.dds");
TeamStateImp _stub_tstate114( 114,L"反制庇佑", "反制庇佑.dds");
TeamStateImp _stub_tstate115( 115,L"勇气", "勇气.dds");
TeamStateImp _stub_tstate116( 116,L"烟雨", "烟雨.dds");
TeamStateImp _stub_tstate117( 117,L"火焰灼烧", "火焰灼烧.dds");
TeamStateImp _stub_tstate118( 118,L"鬼王力量", "鬼王力量.dds");
TeamStateImp _stub_tstate119( 119,L"咒怨之怒", "咒怨之怒.dds");
TeamStateImp _stub_tstate120( 120,L"妄念祝福", "妄念祝福.dds");
TeamStateImp _stub_tstate121( 121,L"倾情", "倾情.dds");
TeamStateImp _stub_tstate122( 122,L"祈福", "祈福.dds");
TeamStateImp _stub_tstate123( 123,L"相思之苦", "相思之苦.dds");
TeamStateImp _stub_tstate124( 124,L"五气朝元", "五气朝元.dds");
TeamStateImp _stub_tstate125( 125,L"天尊法身", "天尊法身.dds");
TeamStateImp _stub_tstate126( 126,L"慈悲之心", "慈悲之心.dds");
TeamStateImp _stub_tstate127( 127,L"天音普渡", "天音普渡.dds");
TeamStateImp _stub_tstate128( 128,L"空即是色", "空即是色.dds");
TeamStateImp _stub_tstate129( 129,L"天鼓祝福", "天鼓祝福.dds");
TeamStateImp _stub_tstate130( 130,L"大梵般若", "大梵般若.dds");
TeamStateImp _stub_tstate131( 131,L"怨鬼心魔", "怨鬼心魔.dds");
TeamStateImp _stub_tstate132( 132,L"邪神傲世", "邪神傲世.dds");
TeamStateImp _stub_tstate133( 133,L"虎魄冰晶", "虎魄冰晶.dds");
TeamStateImp _stub_tstate134( 134,L"龟灵冰晶", "龟灵冰晶.dds");
TeamStateImp _stub_tstate135( 135,L"天罡战意", "天罡战意.dds");
TeamStateImp _stub_tstate136( 136,L"天罡云体", "天罡云体.dds");
TeamStateImp _stub_tstate137( 137,L"天罡步尘", "天罡步尘.dds");
TeamStateImp _stub_tstate138( 138,L"永结同心", "永结同心.dds");
TeamStateImp _stub_tstate139( 139,L"虚空一斩", "虚空一斩.dds");
TeamStateImp _stub_tstate140( 140,L"众望所归(网吧奖励)", "众望所归网吧版.dds");
TeamStateImp _stub_tstate141( 141,L"气血回复(网吧奖励)", "气血回复网吧版.dds");
TeamStateImp _stub_tstate142( 142,L"真气聚合(网吧奖励)", "真气聚合网吧版.dds");
TeamStateImp _stub_tstate143( 143,L"敏捷(网吧奖励)", "敏捷网吧版.dds");
TeamStateImp _stub_tstate144( 144,L"虎魄冰晶(VIP奖励)", "VIP虎魄冰晶.dds");
TeamStateImp _stub_tstate145( 145,L"龟灵冰晶(VIP奖励)", "VIP龟灵冰晶.dds");
TeamStateImp _stub_tstate146( 146,L"恐惧", "恐惧.dds");
TeamStateImp _stub_tstate147( 147,L"龙骧", "龙骧.dds");
TeamStateImp _stub_tstate148( 148,L"虎踞", "虎踞.dds");
TeamStateImp _stub_tstate149( 149,L"斗魂", "斗魂.dds");
TeamStateImp _stub_tstate150( 150,L"钢胆", "钢胆.dds");
TeamStateImp _stub_tstate151( 151,L"疾如风", "疾如风.dds");
TeamStateImp _stub_tstate152( 152,L"徐如林", "徐如林.dds");
TeamStateImp _stub_tstate153( 153,L"侵如火", "侵如火.dds");
TeamStateImp _stub_tstate154( 154,L"安如山", "定如山.dds");
TeamStateImp _stub_tstate155( 155,L"暴杀", "暴杀.dds");
TeamStateImp _stub_tstate156( 156,L"狐影术", "狐影术.dds");
TeamStateImp _stub_tstate157( 157,L"幻化", "幻化.dds");
TeamStateImp _stub_tstate158( 158,L"蚩魂", "蚩魂.dds");
TeamStateImp _stub_tstate159( 159,L"诅咒法阵", "诅咒法阵.dds");
TeamStateImp _stub_tstate160( 160,L"祝福法阵", "祝福法阵.dds");
TeamStateImp _stub_tstate161( 161,L"变形术", "变身技能.dds");
TeamStateImp _stub_tstate162( 162,L"变身", "变身任务.dds");
TeamStateImp _stub_tstate163( 163,L"免疫变身", "免疫变身.dds");
TeamStateImp _stub_tstate164( 164,L"变身抗性增加", "变身抗性增加.dds");
TeamStateImp _stub_tstate165( 165,L"变身抗性减弱", "变身抗性减弱.dds");

TeamStateImp _stub_tstate166( 166,L"迷踪", "迷踪.dds");
TeamStateImp _stub_tstate167( 167,L"潜形", "潜形.dds");
TeamStateImp _stub_tstate168( 168,L"一闪", "一闪.dds");
TeamStateImp _stub_tstate169( 169,L"当身", "当身.dds");
TeamStateImp _stub_tstate170( 170,L"受身", "受身.dds");
TeamStateImp _stub_tstate171( 171,L"背刺", "背刺.dds");
TeamStateImp _stub_tstate172( 172,L"分筋", "分筋.dds");
TeamStateImp _stub_tstate173( 173,L"吃药效果增强", "吃药效果增强.dds");
TeamStateImp _stub_tstate174( 174,L"吃药效果减弱", "吃药效果减弱.dds");
TeamStateImp _stub_tstate175( 175,L"效果反弹", "效果反弹.dds");
TeamStateImp _stub_tstate176( 176,L"延命", "延命.dds");
TeamStateImp _stub_tstate177( 177,L"领域", "领域.dds");
TeamStateImp _stub_tstate178( 178,L"反隐能力提升", "反隐能力提升.dds");
TeamStateImp _stub_tstate179( 179,L"反隐能力下降", "反隐能力下降.dds");
TeamStateImp _stub_tstate180( 180,L"隐身能力提升", "隐身能力提升.dds");
TeamStateImp _stub_tstate181( 181,L"隐身能力下降", "隐身能力下降.dds");
TeamStateImp _stub_tstate182( 182,L"五行遁", "狐影术.dds");
TeamStateImp _stub_tstate183( 183,L"爆发", "元气爆发-魔.dds");
TeamStateImp _stub_tstate184( 184,L"爆发", "元气爆发-魔.dds");
TeamStateImp _stub_tstate185( 185,L"爆发", "元气爆发-魔.dds");
TeamStateImp _stub_tstate186( 186,L"业障", "黑暗术.dds");
TeamStateImp _stub_tstate187( 187,L"血咒", "邪恶禁锢.dds");
TeamStateImp _stub_tstate188( 188,L"反射", "技能反射.dds");
TeamStateImp _stub_tstate189( 189,L"元神震荡", "神圣之源.dds");
TeamStateImp _stub_tstate190( 190,L"元神聚合", "神圣护体.dds");
TeamStateImp _stub_tstate191( 191,L"刺骨", "极度寒冷.dds");
TeamStateImp _stub_tstate192( 192,L"冻结", "冰封术.dds");
TeamStateImp _stub_tstate193( 193,L"轮回", "时光倒流.dds");
TeamStateImp _stub_tstate196( 196,L"刀山地狱", "位移割裂.dds");
TeamStateImp _stub_tstate197( 197,L"天劫", "人体炸弹.dds");

TeamStateImp _stub_tstate199( 199,L"缚魂", "缚魂.dds");
TeamStateImp _stub_tstate200( 200,L"灼烧", "灼烧.dds");
TeamStateImp _stub_tstate201( 201,L"天命", "天命.dds");
TeamStateImp _stub_tstate202( 202,L"转阴易阳", "转阴易阳.dds");
TeamStateImp _stub_tstate203( 203,L"转阳易阴", "转阳易阴.dds");
TeamStateImp _stub_tstate204( 204,L"虚灵法元", "虚灵法元.dds");
TeamStateImp _stub_tstate205( 205,L"定灵法元", "定灵法元.dds");
TeamStateImp _stub_tstate206( 206,L"魅灵法元", "魅灵法元.dds");
TeamStateImp _stub_tstate207( 207,L"眩灵法元", "缓灵法元.dds");
TeamStateImp _stub_tstate208( 208,L"麻痹", "麻痹.dds");
TeamStateImp _stub_tstate209( 209,L"迅疾", "神速.dds");
TeamStateImp _stub_tstate210( 210,L"触发技能", "触发技能.dds");
TeamStateImp _stub_tstate211( 211,L"激活", "激活.dds");
TeamStateImp _stub_tstate212( 212,L"残废", "残废.dds");
TeamStateImp _stub_tstate213( 213,L"地煞·形", "归神1.dds");
TeamStateImp _stub_tstate214( 214,L"天罡·形", "脱灵1.dds");
TeamStateImp _stub_tstate215( 215,L"拉扯", "拉扯.dds");
TeamStateImp _stub_tstate216( 216,L"嗜血", "嗜血.dds");
TeamStateImp _stub_tstate217( 217,L"阵法", "阵法.dds");
TeamStateImp _stub_tstate218( 218,L"附加", "免疫变身.dds");
TeamStateImp _stub_tstate219( 219,L"盈升", "技能增幅.dds");
TeamStateImp _stub_tstate220( 220,L"定止", "潜力增幅.dds");
TeamStateImp _stub_tstate221( 221,L"骑乘加速", "骑乘加速.dds");
TeamStateImp _stub_tstate222( 222,L"地煞·神", "归神2.dds");
TeamStateImp _stub_tstate223( 223,L"天罡·神", "脱灵2.dds");

TeamStateImp _stub_tstate224( 224,L"囚禁", "囚禁.dds");
TeamStateImp _stub_tstate225( 225,L"阻止", "阻止.dds");
TeamStateImp _stub_tstate226( 226,L"拉扯", "拉扯.dds");
TeamStateImp _stub_tstate227( 227,L"原灵术", "原灵术.dds");

TeamStateImp _stub_tstate228( 228,L"霸元·夕", "霸元·夕.dds");
TeamStateImp _stub_tstate229( 229,L"天元·晖", "天元·晖.dds");
TeamStateImp _stub_tstate230( 230,L"霸元·介", "霸元·介.dds");
TeamStateImp _stub_tstate231( 231,L"天元·由", "天元·由.dds");
TeamStateImp _stub_tstate232( 232,L"元·无间", "元·无间.dds");
TeamStateImp _stub_tstate233( 233,L"元·无间", "元·无间.dds");
TeamStateImp _stub_tstate234( 234,L"里界·霁云", "里界·霁云.dds");
TeamStateImp _stub_tstate235( 235,L"莲华·疾", "莲华·疾.dds");
TeamStateImp _stub_tstate236( 236,L"莲华·翔", "莲华·翔.dds");
TeamStateImp _stub_tstate237( 237,L"皇极·冥", "皇极·冥.dds");
TeamStateImp _stub_tstate238( 238,L"界·天演式·和合", "界·天演式·和合.dds");

TeamStateImp _stub_tstate239( 239,L"无视", "定如山.dds");
TeamStateImp _stub_tstate240( 240,L"悲剧", "定身.dds");
TeamStateImp _stub_tstate241( 241,L"抓狂", "恐惧.dds");
TeamStateImp _stub_tstate242( 242,L"猥琐", "迷踪.dds");
TeamStateImp _stub_tstate243( 243,L"战旗", "帮派战旗祝福BUFF图标.dds");
TeamStateImp _stub_tstate244( 244,L"自动复活", "队伍战旗祝福BUFF图标.dds");
TeamStateImp _stub_tstate247( 247,L"自动复活", "GT攻击防御buff.dds");
TeamStateImp _stub_tstate248( 248,L"增花", "赠花用的BUFF图标.dds");
TeamStateImp _stub_tstate249( 249,L"自动复活", "赠花用的BUFF图标.dds");
TeamStateImp _stub_tstate250( 250,L"国王双倍经验", "0.5倍经验.dds");

TeamStateImp _stub_tstate251( 251,L"固定移动速度", "固定移动速度.dds");
TeamStateImp _stub_tstate252( 252,L"傀儡姿态", "傀儡姿态.dds");
TeamStateImp _stub_tstate253( 253,L"傀儡特技", "傀儡特技.dds");
TeamStateImp _stub_tstate254( 254,L"眩晕抗性下降叠加", "眩晕抗性下降叠加.dds");
TeamStateImp _stub_tstate255( 255,L"睡眠抗性下降叠加", "睡眠抗性下降叠加.dds");
TeamStateImp _stub_tstate256( 256,L"虚弱抗性下降叠加", "虚弱抗性下降叠加.dds");
TeamStateImp _stub_tstate257( 257,L"沉默抗性下降叠加", "沉默抗性下降叠加.dds");
TeamStateImp _stub_tstate258( 258,L"缠绕抗性下降叠加", "缠绕抗性下降叠加.dds");
TeamStateImp _stub_tstate259( 259,L"缓慢抗性下降叠加", "缓慢抗性下降叠加.dds");
TeamStateImp _stub_tstate260( 260,L"气血上限持续下降叠加", "气血上限持续下降叠加.dds");
TeamStateImp _stub_tstate261( 261,L"真气上限持续下降叠加", "真气上限持续下降叠加.dds");
TeamStateImp _stub_tstate262( 262,L"种子叠加", "种子叠加.dds");
TeamStateImp _stub_tstate263( 263,L"麒麟形态", "麒麟化身.dds");
TeamStateImp _stub_tstate264( 264,L"人马人形态", "人形态.dds");
TeamStateImp _stub_tstate265( 265,L"邪恶光环2", "纵横天下.dds");
TeamStateImp _stub_tstate266( 266,L"不朽", "霸王无双.dds");
TeamStateImp _stub_tstate267( 267,L"挑衅2", "混乱诅咒.dds");
TeamStateImp _stub_tstate268( 268,L"被攻击增加怒气", "山岚战法.dds");
TeamStateImp _stub_tstate269( 269,L"攻击增加怒气", "怒涛战法.dds");
TeamStateImp _stub_tstate270( 270,L"与召唤兽生命共享", "当身.dds");
TeamStateImp _stub_tstate271( 271,L"宠物通灵", "宠物通灵.dds");
TeamStateImp _stub_tstate272( 272,L"宠物御宝", "宠物御宝.dds");
TeamStateImp _stub_tstate273( 273,L"累积伤害释放", "累计伤害输出BUFF.dds");
TeamStateImp _stub_tstate274( 274,L"血槽", "血池BUFF.dds");
TeamStateImp _stub_tstate275( 275,L"八凶阵法连线冷却", "阻止阵法连线BUFF.dds");
TeamStateImp _stub_tstate276( 276,L"鬼王叠层增加HP上限百分比", "气血叠层BUFF.dds");
TeamStateImp _stub_tstate277( 277,L"鬼王叠层增加攻击力百分比", "攻击叠层BUFF.dds");
TeamStateImp _stub_tstate278( 278,L"鬼王叠层增加韧性", "韧性叠层BUFF.dds");
TeamStateImp _stub_tstate279( 279,L"鬼王叠层增加技能命中", "技能命中叠层BUFF.dds");
TeamStateImp _stub_tstate280( 280,L"鬼王叠层增加减免致命一击率", "减免被爆击叠层BUFF.dds");
TeamStateImp _stub_tstate281( 281,L"深度睡眠", "深度睡眠BUFF.dds");


TeamStateImp _stub_tstate1000( 1000,L"血魂法身（增加气血值）", "天人和一生命.dds");
TeamStateImp _stub_tstate1001( 1001,L"元魂法身（增加真气值）", "天人和一真气.dds");
TeamStateImp _stub_tstate1002( 1002,L"破魂法身（增加攻击力）", "天人和一攻击.dds");
TeamStateImp _stub_tstate1003( 1003,L"御魂法身（增加防御力）", "天人和一防御.dds");
TeamStateImp _stub_tstate1004( 1004,L"定魂法身（增加命中）", "天人和一命中.dds");
TeamStateImp _stub_tstate1005( 1005,L"幻魂法身（增加躲闪）", "天人和一躲闪.dds");
TeamStateImp _stub_tstate1006( 1006,L"行气运功(不断获得修为提升)", "行功.dds");

int Int(int d)    { return d; }
int Int(float f)  { return (int)f; }
int Int(double d) { return (int)d; }

int INT(int d)    { return d; }
int INT(float f)  { return (int)f; }
int INT(double d) { return (int)d; }

SkillStub::Map& SkillStub::GetMap()
{ 
	static Map map; 
	return map; 
}

SkillStub::~SkillStub()
{
}


int Skill::Condition(UseRequirement& info)
{
	if(info.iProfession >= 108 && info.iProfession <= 112)
	{
		if(info.hp < GetMpCost() + GetHpcost())
			return 17;
	}
	else
	{
		if(info.mp < GetMpCost())
			return 2;
		if(info.hp < GetHpcost())
			return 16;
	}
	if(info.dp < GetDpcost())
		return 12;
	if(info.ink < GetInkcost())
		return 15;
	int allowform = GetAllowForm();
	if(allowform && ((1<<info.form)&allowform)==0)
		return 1;
	if(info.iProfession >= 102 && info.iProfession <= 106 && info.form == 6 && allowform == 0 && 
		stub->id != 4783 && stub->id != 4784 && stub->id != 4785 && stub->id != 4786 && stub->id != 4787)
		return 1;
	if(stub->occupation==OCCUPATION_TAOIST)
	{
	   	if((info.cultivation&0x01)==0) return 3;
	}
	else if(stub->occupation==OCCUPATION_EVIL)
	{
	   	if((info.cultivation&0x02)==0) return 3;
	}
	else if(stub->occupation==OCCUPATION_BUDDHA)
	{
	   	if((info.cultivation&0x04)==0) return 3;
	}
	else if(stub->occupation==OCCUPATION_DEITY_TAOIST)
	{
		if((info.cultivation&0x01)==0) return 11;
	}
	else if(stub->occupation==OCCUPATION_DEITY_EVIL)
	{
		if((info.cultivation&0x02)==0) return 11;
	}
	else if(stub->occupation==OCCUPATION_DEITY_BUDDHA)
	{
		if((info.cultivation&0x04)==0) return 11;
	}
	if (stub->weapon_limit!=-1 && stub->weapon_limit!=info.weapon_type)
		return 4;
	if (!(info.skill_limit & PLAYER_IN_BATTLE) && IsBattleLimitSkill())
		return 5;
	if ((info.skill_limit & PLAYER_IS_FIGHTING) && IsCombatLimitSkill())
		return 6;
	if (!(info.skill_limit & PLAYER_IS_FIGHTING) && IsNonCombatLimitSkill())
		return 7;
	if (!(info.skill_limit & PLAYER_HAS_CLONE) && IsNoSummonPetLimitSkill())
		return 8;
	if ((info.skill_limit & PLAYER_IN_MOUNT) && IsMountLimitSkill())
		return 9;
	if((info.skill_limit & PLAYER_IN_FLYING) && IsFlyLimitSkill())
		return 10;
	if(stub->castInPreSkill && stub->preCastSkillId && stub->preCastSkillId != info.curr_skill_id)
		return 12;
	if(!stub->CheckDarkLightValue(this))
	{
		return 14;
	}
	return 0;
}

const VisibleState* VisibleStateImp::Query(int prof, int id)
{
	if(id>MAX_COMMON_VSTATE)
		id += 100*(prof+1);
	Map::iterator it = stubmap.find(id);
	return it == stubmap.end() ? NULL : it->second;
}

const TeamState* TeamStateImp::Query(int id)
{
	Map::iterator it = stubmap.find(id);
	return it == stubmap.end() ? NULL : it->second;
}


}
