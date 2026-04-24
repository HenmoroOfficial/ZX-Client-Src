#ifndef __CPPGEN_GNET_SKILL4752
#define __CPPGEN_GNET_SKILL4752
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4752:public Skill
    {
      public:
        enum
        { SKILL_ID = 4752 };
          Skill4752 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4752Stub:public SkillStub
    {
      public:
        Skill4752Stub ():SkillStub (4752)
        {
            occupation = 130;
            name = L"伤心奇花";
            nativename = "伤心奇花";
            icon = "伤心奇花.dds";
            maxlevel = 5;
            maxlearn = 5;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = -2;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 0;
            action[0] = "";
            action[1] = "";
            action[2] = "";
            action[3] = "";
            action[4] = "";
            action[5] = "";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "";
            action[16] = "";
            action[17] = "";
            action[18] = "";
            action[19] = "";
            action[20] = "";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "";
            rangetype = 0;
            doenchant = false;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
#endif
        }
        virtual ~ Skill4752Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               705 - 55 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel (),
                               100 + 3 * skill->GetLevel (),
                               1000 * skill->GetLevel (),
                               skill->GetLevel () * 20,
                               2 * skill->GetLevel (),
                               skill->GetLevel () * 15,
                               10 + 4 * skill->GetLevel (),
                               6.3 - 0.3 * skill->GetLevel (),
                               8 - skill->GetLevel (),
                               skill->GetLevel () * 6,
                               4 + skill->GetLevel (),
                               2 * skill->GetLevel (), 3 * skill->GetLevel (), 3 * skill->GetLevel (), (60 + 10 * skill->GetLevel ()));

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
#endif
    };
}
#endif
