#ifndef __CPPGEN_GNET_SKILL3033
#define __CPPGEN_GNET_SKILL3033
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3033:public Skill
    {
      public:
        enum
        { SKILL_ID = 3033 };
          Skill3033 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3033Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 500;
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
                return 500;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.05 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) +
                                 0.06 * skill->GetT1 () + 0.02 * skill->GetT4 () +
                                 0.06 * skill->GetT1 () * norm (zrand (100 + 5 * skill->GetT1 ()) / 100));
                skill->SetCrit (0.01 * skill->GetT3 () + 0.03 * norm (skill->GetT4 () / 9.9));
                skill->SetCrithurt (0.05 * skill->GetLevel () + 0.10 * skill->GetT3 ());
                skill->SetPlus (750 + 50 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel ());
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
                return 500;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.05 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) +
                                 0.06 * skill->GetT1 () + 0.02 * skill->GetT4 () +
                                 0.06 * skill->GetT1 () * norm (zrand (100 + 5 * skill->GetT1 ()) / 100));
                skill->SetCrit (0.01 * skill->GetT3 () + 0.03 * norm (skill->GetT4 () / 9.9));
                skill->SetCrithurt (0.05 * skill->GetLevel () + 0.10 * skill->GetT3 ());
                skill->SetPlus (750 + 50 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State4:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 500;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.05 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) +
                                 0.02 * skill->GetT4 () + 3 * norm (zrand (108) / 100.0) + 0.06 * skill->GetT1 () +
                                 0.06 * skill->GetT1 () * norm (zrand (100 + 5 * skill->GetT1 ()) / 100));
                skill->SetCrit (0.01 * skill->GetT3 () + 0.03 * norm (skill->GetT4 () / 9.9));
                skill->SetCrithurt (0.05 * skill->GetLevel () + 0.10 * skill->GetT3 ());
                skill->SetPlus (750 + 50 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel () +
                                2 * skill->GetLevel () * skill->GetPlayer ()->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill3033Stub ():SkillStub (3033)
        {
            occupation = 167;
            name = L"Ãð»ê¿ñì­<Ðþ>";
            nativename = "Ãð»ê¿ñì­<Ðþ>";
            icon = "ÏÉ¡¤Ãð»ê¿ñì­.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 1;
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
            skill_class = 4;
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
            talent[0] = 1151;
            talent[1] = 1153;
            talent[2] = 1154;
            talent[3] = 1155;
            talent[4] = 1543;
            talent_size = 5;
            action[0] = "¹íµÀ_Ãð»ê¿ñì­";
            action[1] = "¹íµÀ_Ãð»ê¿ñì­";
            action[2] = "¹íµÀ_Ãð»ê¿ñì­";
            action[3] = "¹íµÀ_Ãð»ê¿ñì­";
            action[4] = "¹íµÀ_Ãð»ê¿ñì­";
            action[5] = "¹íµÀ_Ãð»ê¿ñì­";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íµÀ_Æï³Ë_Ãð»ê¿ñì­";
            action[16] = "¹íµÀ_Æï³Ë_Ãð»ê¿ñì­";
            action[17] = "¹íµÀ_Æï³Ë_Ãð»ê¿ñì­";
            action[18] = "¹íµÀ_Æï³Ë_Ãð»ê¿ñì­";
            action[19] = "¹íµÀ_Æï³Ë_Ãð»ê¿ñì­";
            action[20] = "¹íµÀ_Æï³Ë_Ãð»ê¿ñì­";
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
            rangetype = 2;
            doenchant = true;
            dobless = true;
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
            statestub.push_back (new State4 ());
#endif
        }
        virtual ~ Skill3033Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (200 + 34 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1131 };
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
        float GetMpcost (Skill * skill) const
        {
            return (float) (80 * skill->GetLevel () + 800 + 5 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 13000 - 1950 * skill->GetT0 () - 650 * skill->GetT4 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (20);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (15);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (80 * skill->GetLevel () + 800 + 5 * skill->GetLevel () * skill->GetLevel ()),
                               200 + 34 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel (),
                               13,
                               INT (6 + INT (skill->GetLevel () / 2)),
                               INT (5 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 3 * (skill->GetLevel () - 15)),
                               INT (750 + 50 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel ()),
                               5 * skill->GetLevel (),
                               6,
                               15 * skill->GetLevel (),
                               12,
                               170 + 6 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel (),
                               skill->GetLevel (), 2 * skill->GetLevel (), 2 + 0.5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetRatio (0.01 * skill->GetLevel ());
            skill->GetVictim ()->SetDrainmagic (1);
            skill->GetVictim ()->SetProbability (1.0 * (170 + 6 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (12000 + 100);
            skill->GetVictim ()->SetRatio (0.15);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetWeak (1);
            skill->GetVictim ()->SetProbability (1.0 * (norm (zrand (100) / (100 - 2 * skill->GetT2 ())) * 650));
            skill->GetVictim ()->SetTime (2050 * skill->GetT2 ());
            skill->GetVictim ()->SetDizzy (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (6000 + 100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (15 * skill->GetLevel ());
            skill->GetVictim ()->SetAdddodge (1);
            skill->GetVictim ()->SetTime (2050 + 500 * skill->GetLevel ());
            skill->GetVictim ()->SetImmunesilent (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (6 + INT (skill->GetLevel () / 2));
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
