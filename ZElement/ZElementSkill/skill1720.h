#ifndef __CPPGEN_GNET_SKILL1720
#define __CPPGEN_GNET_SKILL1720
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1720:public Skill
    {
      public:
        enum
        { SKILL_ID = 1720 };
          Skill1720 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1720Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 5600;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (0);
                skill->SetPlus (1);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1720Stub ():SkillStub (1720)
        {
            occupation = 129;
            name = L"南明离火";
            nativename = "南明离火";
            icon = "邪剑星羽.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
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
            action[0] = "青云门_天地不仁";
            action[1] = "青云门_天地不仁";
            action[2] = "青云门_天地不仁";
            action[3] = "青云门_天地不仁";
            action[4] = "青云门_天地不仁";
            action[5] = "青云门_天地不仁";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "青云门_骑乘_天地不仁";
            action[16] = "青云门_骑乘_天地不仁";
            action[17] = "青云门_骑乘_天地不仁";
            action[18] = "青云门_骑乘_天地不仁";
            action[19] = "青云门_骑乘_天地不仁";
            action[20] = "青云门_骑乘_天地不仁";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
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
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill1720Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 20);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1000);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 6000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 180000;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 10);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
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
            skill->GetVictim ()->SetRatio (skill->GetPlayer ()->GetTmplid () == 19687 ? 299999 : 0);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetTmplid () == 19687 ? 1 : 0);
            skill->GetVictim ()->SetSecondattack (1);
            return true;
        }
#endif
    };
}
#endif
