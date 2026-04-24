#ifndef __CPPGEN_GNET_SKILL3017
#define __CPPGEN_GNET_SKILL3017
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3017:public Skill
    {
      public:
        enum
        { SKILL_ID = 3017 };
          Skill3017 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3017Stub:public SkillStub
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
                skill->SetRatio (1.1 + 0.05 * skill->GetT3 () + 0.05 * skill->GetLevel () +
                                 norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15));
                skill->SetPlus ((7.2 * skill->GetLevel () * skill->GetLevel () - 6.4 * skill->GetLevel () + 694) *
                                ((100 + (zrand (10 + 15 * skill->GetT1 ()) - 10) * norm (skill->GetT1 () + 0.1)) / 100.0));
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
                skill->GetPlayer ()->SetVar1 (1);
                skill->SetRatio (1.1 + 0.05 * skill->GetT3 () + 0.05 * skill->GetLevel () +
                                 norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15));
                skill->SetPlus ((7.2 * skill->GetLevel () * skill->GetLevel () - 6.4 * skill->GetLevel () + 694) *
                                ((100 + (zrand (10 + 15 * skill->GetT1 ()) - 10) * norm (skill->GetT1 () + 0.1)) / 100.0));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill3017Stub ():SkillStub (3017)
        {
            occupation = 167;
            name = L"雷光遁龙诀<玄>";
            nativename = "雷光遁龙诀<玄>";
            icon = "仙·雷光遁龙诀.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            clearmask = 1;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 2;
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
            preskillnum = 1;
            talent[0] = 582;
            talent[1] = 606;
            talent[2] = 610;
            talent[3] = 617;
            talent_size = 4;
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
        virtual ~ Skill3017Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (200 + 34 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 783 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 6 };
            return array[index];
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 400, 500, 500, 700, 1500, 3100, 3900, 6500, 9100, 10800, 20700, 26500, 39600, 52900, 84300, 119800, 229700, 389600, 525000,
 1237600 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 2, 4, 6, 8, 10, 13, 16, 19, 22, 25, 29, 33, 37, 42, 47, 53, 59, 65, 71, 77 };
            return array[level - 1];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (24);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (11.5 * skill->GetLevel () * skill->GetLevel () - 2.8 * skill->GetLevel () + 1630);
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
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
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
            return _snwprintf (buffer, length, format,
                               INT (11.5 * skill->GetLevel () * skill->GetLevel () - 2.8 * skill->GetLevel () + 1630),
                               200 + 34 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel (),
                               3,
                               10 + skill->GetLevel (),
                               20,
                               INT (10 + 5 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 3 * (skill->GetLevel () - 15)),
                               INT (7.2 * skill->GetLevel () * skill->GetLevel () - 6.4 * skill->GetLevel () + 694),
                               225 * skill->GetLevel (), 12 + skill->GetLevel (), 8, 4 + 0.5 * skill->GetLevel (), 48 + 24 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (12 + skill->GetLevel ()));
            skill->GetVictim ()->SetTime (8000 * (1 + skill->GetT0 ()) + 100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (225 * skill->GetLevel ());
            skill->GetVictim ()->SetSubattack (1);
            skill->GetVictim ()->SetTime (2050 + 500 * skill->GetLevel ());
            skill->GetVictim ()->SetAmount (skill->GetLevel ());
            skill->GetVictim ()->SetValue (48 + 24 * skill->GetLevel ());
            skill->GetVictim ()->SetCursed (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (10 + skill->GetLevel ());
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
