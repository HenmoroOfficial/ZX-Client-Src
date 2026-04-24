#ifndef __CPPGEN_GNET_SKILL3308
#define __CPPGEN_GNET_SKILL3308
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3308:public Skill
    {
      public:
        enum
        { SKILL_ID = 3308 };
          Skill3308 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3308Stub:public SkillStub
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
                skill->SetRatio (1 + 0.06 * 3 + 0.02 * 3 + 0.06 * 3 * norm (zrand (100 + 5 * 3) / 100));
                skill->SetCrit (0.01 * 3);
                skill->SetCrithurt (0.10 * 6 + 0.10 * 3);
                skill->SetPlus (750 + 50 * 6 + 2 * 6 * 6);
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
                skill->SetRatio (1 + 0.06 * 3 + 0.02 * 3 + 0.06 * 3 * norm (zrand (100 + 5 * 3) / 100));
                skill->SetCrit (0.01 * 3);
                skill->SetCrithurt (0.10 * 6 + 0.10 * 3);
                skill->SetPlus (750 + 50 * 6 + 2 * 6 * 6);
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
                skill->SetRatio (1 + 0.02 * 3 + 3 * norm (zrand (108) / 100.0) + 0.06 * 3 + 0.06 * 3 * norm (zrand (100 + 5 * 3) / 100));
                skill->SetCrit (0.01 * 3);
                skill->SetCrithurt (0.10 * 6 + 0.10 * 3);
                skill->SetPlus (750 + 50 * 6 + 2 * 6 * 6 + 2 * 6 * skill->GetPlayer ()->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill3308Stub ():SkillStub (3308)
        {
            occupation = 128;
            name = L"Ãğ»ê¿ñì­";
            nativename = "Ãğ»ê¿ñì­";
            icon = "Ãğ»ê¿ñì­.dds";
            maxlevel = 1;
            maxlearn = 1;
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
            action[0] = "¹íµÀ_Ãğ»ê¿ñì­";
            action[1] = "¹íµÀ_Ãğ»ê¿ñì­";
            action[2] = "¹íµÀ_Ãğ»ê¿ñì­";
            action[3] = "¹íµÀ_Ãğ»ê¿ñì­";
            action[4] = "¹íµÀ_Ãğ»ê¿ñì­";
            action[5] = "¹íµÀ_Ãğ»ê¿ñì­";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íµÀ_Æï³Ë_Ãğ»ê¿ñì­";
            action[16] = "¹íµÀ_Æï³Ë_Ãğ»ê¿ñì­";
            action[17] = "¹íµÀ_Æï³Ë_Ãğ»ê¿ñì­";
            action[18] = "¹íµÀ_Æï³Ë_Ãğ»ê¿ñì­";
            action[19] = "¹íµÀ_Æï³Ë_Ãğ»ê¿ñì­";
            action[20] = "¹íµÀ_Æï³Ë_Ãğ»ê¿ñì­";
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
        virtual ~ Skill3308Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
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
            return 13000 - 1950 * 3 - 650 * 3;
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
            return (float) (10);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               1000, 15, INT (750 + 50 * 6 + 2 * 6 * 6), 10 * 6, 8, 15 * 6, 12, 170 + 16 * 6 + 6 * 6, 3 * 6, 2 * 6);

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
            skill->GetVictim ()->SetProbability (1.0 * (100 * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetRatio (0.03 * 6);
            skill->GetVictim ()->SetDrainmagic (1);
            skill->GetVictim ()->SetProbability (1.0 * (170 + 16 * 6 + 6 * 6));
            skill->GetVictim ()->SetTime (12000 + 100);
            skill->GetVictim ()->SetRatio (0.15);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetWeak (1);
            skill->GetVictim ()->SetProbability (1.0 * (norm (zrand (100) / (100 - 2 * 3)) * 600));
            skill->GetVictim ()->SetTime (2050 * 3);
            skill->GetVictim ()->SetDizzy (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (6000 + 100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (15 * 6);
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
