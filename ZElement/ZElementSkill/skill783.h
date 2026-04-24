#ifndef __CPPGEN_GNET_SKILL783
#define __CPPGEN_GNET_SKILL783
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill783:public Skill
    {
      public:
        enum
        { SKILL_ID = 783 };
          Skill783 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill783Stub:public SkillStub
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
                skill->SetRatio (1.1 + 0.05 * skill->GetT3 ());
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
                skill->SetRatio (1.1 + 0.05 * skill->GetT3 ());
                skill->SetPlus ((7.2 * skill->GetLevel () * skill->GetLevel () - 6.4 * skill->GetLevel () + 694) *
                                ((100 + (zrand (10 + 15 * skill->GetT1 ()) - 10) * norm (skill->GetT1 () + 0.1)) / 100.0));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill783Stub ():SkillStub (783)
        {
            occupation = 20;
            name = L"雷光遁龙诀";
            nativename = "雷光遁龙诀";
            icon = "雷光遁龙诀.dds";
            maxlevel = 7;
            maxlearn = 6;
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
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "青云门_骑乘_雷光遁龙决";
            action[16] = "青云门_骑乘_雷光遁龙决";
            action[17] = "青云门_骑乘_雷光遁龙决";
            action[18] = "青云门_骑乘_雷光遁龙决";
            action[19] = "青云门_骑乘_雷光遁龙决";
            action[20] = "青云门_骑乘_雷光遁龙决";
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
        virtual ~ Skill783Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 0 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 24 };
            return array[index];
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
            static int array[6] = { 135, 135, 135, 135, 136, 137 };
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
                               40,
                               20,
                               10,
                               INT (7.2 * skill->GetLevel () * skill->GetLevel () - 6.4 * skill->GetLevel () + 694),
                               25 * skill->GetLevel (), 35 + 5 * skill->GetLevel (), 8);

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
            skill->GetVictim ()->SetProbability (1.0 * (35 + 5 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (8000 * (1 + skill->GetT0 ()) + 100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (25 * skill->GetLevel () * (1 + skill->GetT0 ()));
            skill->GetVictim ()->SetSubattack (1);
            skill->GetVictim ()->SetProbability (1.0 * (5 * skill->GetT2 ()));
            skill->GetVictim ()->SetTime (3050 * skill->GetT2 ());
            skill->GetVictim ()->SetRatio (0.05 * skill->GetT2 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecdefence (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (40);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
