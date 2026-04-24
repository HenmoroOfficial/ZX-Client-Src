#ifndef __CPPGEN_GNET_SKILL308
#define __CPPGEN_GNET_SKILL308
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill308:public Skill
    {
      public:
        enum
        { SKILL_ID = 308 };
          Skill308 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill308Stub:public SkillStub
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
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrit (0.01 * skill->GetT3 ());
                skill->SetCrithurt (0.1 * skill->GetT3 ());
                skill->SetRatio (1 + 0.05 * skill->GetT1 ());
                skill->SetPlus (INT (1.42 - 0.05 * skill->GetLevel ()) * (54 + 6 * skill->GetLevel ()) +
                                INT (0.05 * skill->GetLevel () + 0.55) * (0.38 * skill->GetLevel () * skill->GetLevel () + 4.6 * skill->GetLevel () +
                                                                          54));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill308Stub ():SkillStub (308)
        {
            occupation = 4;
            name = L"ÆÆÔÆ";
            nativename = "ÆÆÔÆ";
            icon = "ÆÆÔÆ.dds";
            maxlevel = 20;
            maxlearn = 9;
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
            skill_class = 1;
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
            talent[0] = 739;
            talent[1] = 742;
            talent[2] = 749;
            talent[3] = 753;
            talent_size = 4;
            action[0] = "ºÏ»¶ÅÉ_ÆÆÔÆ";
            action[1] = "ºÏ»¶ÅÉ_ÆÆÔÆ";
            action[2] = "ºÏ»¶ÅÉ_ÆÆÔÆ";
            action[3] = "ºÏ»¶ÅÉ_ÆÆÔÆ";
            action[4] = "ºÏ»¶ÅÉ_ÆÆÔÆ";
            action[5] = "ºÏ»¶ÅÉ_ÆÆÔÆ";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_ÆÆÔÆ";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_ÆÆÔÆ";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_ÆÆÔÆ";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_ÆÆÔÆ";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_ÆÆÔÆ";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_ÆÆÔÆ";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "¼¼ÄÜ¹¥»÷1";
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
        virtual ~ Skill308Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 300 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 6 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0.18 * skill->GetLevel () * skill->GetLevel () + 0.013 * skill->GetLevel () + 30);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 4000 - 800 * skill->GetT2 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 40, 42, 44, 46, 48, 50, 52, 54, 56 };
            return array[level - 1];
        }
        float GetAttackdistance (Skill * skill) const
        {
            return (float) (5.5 + 0.5 * skill->GetLevel ());
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (7);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (0.18 * skill->GetLevel () * skill->GetLevel () + 0.013 * skill->GetLevel () + 30),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) * (54 + 6 * skill->GetLevel ()) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (0.38 * skill->GetLevel () * skill->GetLevel () +
                                                                              4.6 * skill->GetLevel () + 54)), 240 + 48 * skill->GetLevel (),
                               480 + 96 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (8000);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount ((240 + 48 * skill->GetLevel ()) * (1 + 0.3 * skill->GetT0 ()) +
                                            (120 + 24 * skill->GetLevel ()) * (1 +
                                                                               0.3 * skill->GetT0 ()) * INT (zrand (100) / (100 -
                                                                                                                            6 * skill->GetT0 ())));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (100 * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetTime (8000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount ((480 + 96 * skill->GetLevel ()) * (1 + 0.3 * skill->GetT0 ()));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetMpleak (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (8);
        }
#endif
    };
}
#endif
