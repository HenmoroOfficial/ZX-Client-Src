#ifndef __CPPGEN_GNET_SKILL5061
#define __CPPGEN_GNET_SKILL5061
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill5061:public Skill
    {
      public:
        enum
        { SKILL_ID = 5061 };
          Skill5061 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill5061Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
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
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill5061Stub ():SkillStub (5061)
        {
            occupation = 128;
            name = L"Ï§´º";
            nativename = "Ï§´º";
            icon = "Ï§´º.dds";
            maxlevel = 1;
            maxlearn = 1;
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
            action[0] = "ºÏ»¶ÅÉ_Ï§´º";
            action[1] = "ºÏ»¶ÅÉ_Ï§´º";
            action[2] = "ºÏ»¶ÅÉ_Ï§´º";
            action[3] = "ºÏ»¶ÅÉ_Ï§´º";
            action[4] = "ºÏ»¶ÅÉ_Ï§´º";
            action[5] = "ºÏ»¶ÅÉ_Ï§´º";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "0";
            action[14] = "0";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_Ï§´º";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_Ï§´º";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_Ï§´º";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_Ï§´º";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_Ï§´º";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_Ï§´º";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "0";
            action[29] = "0";
            action[30] = "¼¼ÄÜ¹¥»÷2";
            rangetype = 5;
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
        virtual ~ Skill5061Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetHpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetInkcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (500);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 180000 - 14400 * 3;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 500, 8, 2 * 5, 30);

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
            skill->GetVictim ()->SetTime (8000 * (1 + 0.1 * 3) + 100);
            skill->GetVictim ()->SetValue (2 * 5 + 2 * 3);
            skill->GetVictim ()->SetBlessed (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (99);
            skill->GetVictim ()->SetValue (99);
            skill->GetVictim ()->SetExorcism (1);
            skill->GetVictim ()->SetProbability (1.0 * (30 * 3));
            skill->GetVictim ()->SetClearslow (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (8000 * (1 + 0.1 * 3) + 100);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetAmount (30);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetMaxhp () * 0.3);
            skill->GetVictim ()->SetHpregain (1);
            return true;
        }
#endif
    };
}
#endif
