#ifndef __CPPGEN_GNET_SKILL4165
#define __CPPGEN_GNET_SKILL4165
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4165:public Skill
    {
      public:
        enum
        { SKILL_ID = 4165 };
          Skill4165 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4165Stub:public SkillStub
    {
      public:
        Skill4165Stub ():SkillStub (4165)
        {
            occupation = 56;
            name = L"Ìì³ê";
            nativename = "Ìì³ê";
            icon = "Ìì³ê.dds";
            maxlevel = 4;
            maxlearn = 3;
            type = 11;
            eventflag = 1;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            talent_type = 1;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 11;
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
            action[0] = "0";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "0";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
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
        virtual ~ Skill4165Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetRequiredLevel (int level) const
        {
            static int array[3] = { 135, 135, 135 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 0.5 * skill->GetLevel (), 0.5 * skill->GetLevel (), 0.5 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool TakeEffect (Skill * skill) const
        {
            skill->GetPlayer ()->SetPasinccrit (0.005001 * skill->GetLevel () *
                                                ((skill->GetPlayer ()->GetSkilllevel (4117) ==
                                                  skill->GetPlayer ()->GetSkilllevel (4134) ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (4125) ==
                                                                                                        skill->GetPlayer ()->
                                                                                                        GetSkilllevel (4142) ? 1 : 0) +
                                                 (skill->GetPlayer ()->GetSkilllevel (4131) == skill->GetPlayer ()->GetSkilllevel (4148) ? 1 : 0)));
            return true;
        }
#endif
    };
}
#endif
