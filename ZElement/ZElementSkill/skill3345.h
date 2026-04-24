#ifndef __CPPGEN_GNET_SKILL3345
#define __CPPGEN_GNET_SKILL3345
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3345:public Skill
    {
      public:
        enum
        { SKILL_ID = 3345 };
          Skill3345 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3345Stub:public SkillStub
    {
      public:
        Skill3345Stub ():SkillStub (3345)
        {
            occupation = 128;
            name = L"·ÙÏã°ÔÐ°Õó·¨ÖÐ×ª";
            nativename = "·ÙÏã°ÔÐ°Õó·¨ÖÐ×ª";
            icon = "";
            maxlevel = 99;
            maxlearn = 10;
            type = 2;
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
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
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
            talent[0] = 3399;
            talent_size = 1;
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
            doenchant = true;
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
        virtual ~ Skill3345Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetRatio (skill->GetPlayer ()->GetHasbuff (4504) == 1 ? 20 : 220);
            skill->GetVictim ()->SetAmount (skill->GetPlayer ()->GetHasbuff (4524) == 1 ? 22 : 220);
            skill->GetVictim ()->SetValue (220);
            skill->GetVictim ()->SetDispel (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetRatio (skill->GetPlayer ()->GetHasbuff (4514) == 1 ? 21 : 220);
            skill->GetVictim ()->SetAmount (skill->GetPlayer ()->GetHasbuff (4494) == 1 ? 19 : 220);
            skill->GetVictim ()->SetValue (220);
            skill->GetVictim ()->SetDispel (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (1);
            skill->GetVictim ()->SetRatio (skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetValue (3605);
            skill->GetVictim ()->SetDelaycast (1);
            return true;
        }
#endif
    };
}
#endif
