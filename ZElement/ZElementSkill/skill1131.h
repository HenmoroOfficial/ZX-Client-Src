#ifndef __CPPGEN_GNET_SKILL1131
#define __CPPGEN_GNET_SKILL1131
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1131:public Skill
    {
      public:
        enum
        { SKILL_ID = 1131 };
          Skill1131 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1131Stub:public SkillStub
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
                skill->SetRatio (1 + 0.06 * skill->GetT1 () + 0.02 * skill->GetT4 () +
                                 0.06 * skill->GetT1 () * norm (zrand (100 + 5 * skill->GetT1 ()) / 100));
                skill->SetCrit (0.01 * skill->GetT3 () + 0.03 * norm (skill->GetT4 () / 9.9));
                skill->SetCrithurt (0.10 * skill->GetLevel () + 0.10 * skill->GetT3 ());
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
                skill->SetRatio (1 + 0.06 * skill->GetT1 () + 0.02 * skill->GetT4 () +
                                 0.06 * skill->GetT1 () * norm (zrand (100 + 5 * skill->GetT1 ()) / 100));
                skill->SetCrit (0.01 * skill->GetT3 () + 0.03 * norm (skill->GetT4 () / 9.9));
                skill->SetCrithurt (0.10 * skill->GetLevel () + 0.10 * skill->GetT3 ());
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
                skill->SetRatio (1 + 0.02 * skill->GetT4 () + 3 * norm (zrand (108) / 100.0) + 0.06 * skill->GetT1 () +
                                 0.06 * skill->GetT1 () * norm (zrand (100 + 5 * skill->GetT1 ()) / 100));
                skill->SetCrit (0.01 * skill->GetT3 () + 0.03 * norm (skill->GetT4 () / 9.9));
                skill->SetCrithurt (0.10 * skill->GetLevel () + 0.10 * skill->GetT3 ());
                skill->SetPlus (750 + 50 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel () +
                                2 * skill->GetLevel () * skill->GetPlayer ()->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill1131Stub ():SkillStub (1131)
        {
            occupation = 29;
            name = L"Ãğ»ê¿ñì­";
            nativename = "Ãğ»ê¿ñì­";
            icon = "Ãğ»ê¿ñì­.dds";
            maxlevel = 7;
            maxlearn = 6;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 1;
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
            action[0] = "¹íµÀ_Ãğ»ê¿ñì­";
            action[1] = "¹íµÀ_Ãğ»ê¿ñì­";
            action[2] = "¹íµÀ_Ãğ»ê¿ñì­";
            action[3] = "¹íµÀ_Ãğ»ê¿ñì­";
            action[4] = "¹íµÀ_Ãğ»ê¿ñì­";
            action[5] = "¹íµÀ_Ãğ»ê¿ñì­";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íµÀ_Æï³Ë_Ãğ»ê¿ñì­";
            action[16] = "¹íµÀ_Æï³Ë_Ãğ»ê¿ñì­";
            action[17] = "¹íµÀ_Æï³Ë_Ãğ»ê¿ñì­";
            action[18] = "¹íµÀ_Æï³Ë_Ãğ»ê¿ñì­";
            action[19] = "¹íµÀ_Æï³Ë_Ãğ»ê¿ñì­";
            action[20] = "¹íµÀ_Æï³Ë_Ãğ»ê¿ñì­";
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
        virtual ~ Skill1131Stub ()
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
            return 13000 - 1950 * skill->GetT0 () - 650 * skill->GetT4 () - 1950 * 3 * norm (skill->GetT1 () / 1.9);
        }
        int GetRequiredLevel (int level) const
        {
            static int array[6] = { 135, 135, 135, 135, 136, 137 };
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
                               35,
                               INT (750 + 50 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel ()),
                               10 * skill->GetLevel (),
                               8,
                               15 * skill->GetLevel (),
                               12,
                               170 + 16 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel (),
                               3 * skill->GetLevel (), 2 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (100 * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetRatio (0.03 * skill->GetLevel ());
            skill->GetVictim ()->SetDrainmagic (1);
            skill->GetVictim ()->SetProbability (1.0 * (170 + 16 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (12000 + 100);
            skill->GetVictim ()->SetRatio (0.15);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetWeak (1);
            skill->GetVictim ()->SetProbability (1.0 * (norm (zrand (100) / (100 - 2 * skill->GetT2 ())) * 600));
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
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (35);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
