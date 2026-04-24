#ifndef __CPPGEN_GNET_SKILL3193
#define __CPPGEN_GNET_SKILL3193
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3193:public Skill
    {
      public:
        enum
        { SKILL_ID = 3193 };
          Skill3193 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3193Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 50;
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
                return 450;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3193Stub ():SkillStub (3193)
        {
            occupation = 128;
            name = L"Ìì½ç-Í¼ÌÚ¹¥·ÀÑª";
            nativename = "Ìì½ç-Í¼ÌÚ¹¥·ÀÑª";
            icon = "";
            maxlevel = 10;
            maxlearn = 10;
            type = 3;
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
            action[30] = "¼¼ÄÜ¹¥»÷2";
            rangetype = 2;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 16;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill3193Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 500;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 0;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (17);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (16);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (15);
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
            skill->GetVictim ()->SetProbability (1.0 * ((skill->GetLevel () == 1 && skill->GetPlayer ()->GetTmplid () == 42271) ? 100 : 0));
            skill->GetVictim ()->SetTime (12000);
            skill->GetVictim ()->SetRatio (0.6);
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetIncattack (1);
            skill->GetVictim ()->SetProbability (1.0 * ((skill->GetLevel () == 2 && skill->GetPlayer ()->GetTmplid () == 42271) ? 100 : 0));
            skill->GetVictim ()->SetTime (12000);
            skill->GetVictim ()->SetRatio (0.6);
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetIncdefence (1);
            skill->GetVictim ()->SetProbability (1.0 * ((skill->GetLevel () == 3 && skill->GetPlayer ()->GetTmplid () != 42271) ? 100 : 0));
            skill->GetVictim ()->SetTime (500);
            skill->GetVictim ()->SetBuffid (6);
            skill->GetVictim ()->SetAmount (skill->GetPlayer ()->GetMaxhp () * (0.6 + 0.01 * zrand (10)));
            skill->GetVictim ()->SetValue (1);
            skill->GetVictim ()->SetHpleak (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (99);
        }
#endif
    };
}
#endif
