#ifndef __CPPGEN_GNET_SKILL3292
#define __CPPGEN_GNET_SKILL3292
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3292:public Skill
    {
      public:
        enum
        { SKILL_ID = 3292 };
          Skill3292 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3292Stub:public SkillStub
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
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1.1 + 0.05 * 3);
                skill->SetPlus ((7.2 * 6 * 6 - 6.4 * 6 + 694) * ((100 + (zrand (10 + 15 * 3) - 10) * norm (3 + 0.1)) / 100.0));
                skill->GetPlayer ()->SetPerform (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State3:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1.1 + 0.05 * 3);
                skill->SetPlus ((7.2 * 6 * 6 - 6.4 * 6 + 694) * ((100 + (zrand (10 + 15 * 3) - 10) * norm (3 + 0.1)) / 100.0));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill3292Stub ():SkillStub (3292)
        {
            occupation = 128;
            name = L"雷光遁龙诀";
            nativename = "雷光遁龙诀";
            icon = "雷光遁龙诀.dds";
            maxlevel = 1;
            maxlearn = 1;
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
            action[0] = "青云门_雷光遁龙决";
            action[1] = "青云门_雷光遁龙决";
            action[2] = "青云门_雷光遁龙决";
            action[3] = "青云门_雷光遁龙决";
            action[4] = "青云门_雷光遁龙决";
            action[5] = "青云门_雷光遁龙决";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "青云门_骑乘_雷光遁龙决";
            action[16] = "青云门_骑乘_雷光遁龙决";
            action[17] = "青云门_骑乘_雷光遁龙决";
            action[18] = "青云门_骑乘_雷光遁龙决";
            action[19] = "青云门_骑乘_雷光遁龙决";
            action[20] = "青云门_骑乘_雷光遁龙决";
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
            rangetype = 3;
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
            statestub.push_back (new State3 ());
#endif
        }
        virtual ~ Skill3292Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (24);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1000);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 3000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (21);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (21);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (16);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 1000, 15, 20, 10, INT (7.2 * 6 * 6 - 6.4 * 6 + 694), 25 * 6, 35 + 5 * 6, 8);

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
            skill->GetVictim ()->SetProbability (1.0 * (35 + 5 * 6));
            skill->GetVictim ()->SetTime (8000 * (1 + 3) + 100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (25 * 6 * (1 + 3));
            skill->GetVictim ()->SetSubattack (1);
            skill->GetVictim ()->SetProbability (1.0 * (5 * 3));
            skill->GetVictim ()->SetTime (3050 * 3);
            skill->GetVictim ()->SetRatio (0.05 * 3);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecdefence (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (15);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
