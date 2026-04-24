#ifndef __CPPGEN_GNET_SKILL304
#define __CPPGEN_GNET_SKILL304
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill304:public Skill
    {
      public:
        enum
        { SKILL_ID = 304 };
          Skill304 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill304Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1600;
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrit (0.01 * skill->GetT3 ());
                skill->SetCrithurt (0.1 * skill->GetT3 ());
                skill->SetRatio (1.01 + 0.01 * skill->GetLevel () + 0.05 * skill->GetT1 ());
                skill->SetPlus (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                (0.63 * skill->GetLevel () * skill->GetLevel () + 2.5 * skill->GetLevel () + 597) + INT (0.05 * skill->GetLevel () +
                                                                                                                         0.55) * (133 +
                                                                                                                                  79 *
                                                                                                                                  skill->GetLevel () -
                                                                                                                                  1.4 *
                                                                                                                                  skill->GetLevel () *
                                                                                                                                  skill->
                                                                                                                                  GetLevel ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill304Stub ():SkillStub (304)
        {
            occupation = 6;
            name = L"Çã³Ç";
            nativename = "Çã³Ç";
            icon = "Çã³Ç.dds";
            maxlevel = 20;
            maxlearn = 9;
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
            talent[0] = 743;
            talent[1] = 742;
            talent[2] = 749;
            talent[3] = 753;
            talent_size = 4;
            action[0] = "ºÏ»¶ÅÉ_Çã³Ç";
            action[1] = "ºÏ»¶ÅÉ_Çã³Ç";
            action[2] = "ºÏ»¶ÅÉ_Çã³Ç";
            action[3] = "ºÏ»¶ÅÉ_Çã³Ç";
            action[4] = "ºÏ»¶ÅÉ_Çã³Ç";
            action[5] = "ºÏ»¶ÅÉ_Çã³Ç";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_Çã³Ç";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_Çã³Ç";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_Çã³Ç";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_Çã³Ç";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_Çã³Ç";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_Çã³Ç";
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
        virtual ~ Skill304Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 306 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (13);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0.55 * skill->GetLevel () * skill->GetLevel () + 12 * skill->GetLevel () + 254);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 2000 - 400 * skill->GetT2 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 76, 77, 79, 81, 83, 85, 87, 89, 91 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (3);
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
                               INT (0.55 * skill->GetLevel () * skill->GetLevel () + 12 * skill->GetLevel () + 254),
                               1 + skill->GetLevel (),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (0.63 * skill->GetLevel () * skill->GetLevel () + 2.5 * skill->GetLevel () + 597) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (133 + 79 * skill->GetLevel () -
                                                                              1.4 * skill->GetLevel () * skill->GetLevel ())),
                               INT (39 + 4.1 * skill->GetLevel ()), 3 * skill->GetLevel () + 3);

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
            skill->GetVictim ()->SetTime (3000 * skill->GetLevel () + 3000 + 100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (39 + 4.1 * skill->GetLevel ());
            skill->GetVictim ()->SetSubantisilent (1);
            skill->GetVictim ()->SetProbability (1.0 * ((40 + 8 * skill->GetT0 ()) * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetRatio (0.24 * (INT (zrand (100) / (100 - 4 * skill->GetT0 ())) + 1));
            skill->GetVictim ()->SetXiaoxin (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (10);
        }
#endif
    };
}
#endif
