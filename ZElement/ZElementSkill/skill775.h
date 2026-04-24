#ifndef __CPPGEN_GNET_SKILL775
#define __CPPGEN_GNET_SKILL775
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill775:public Skill
    {
      public:
        enum
        { SKILL_ID = 775 };
          Skill775 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill775Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 400;
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
                skill->SetRatio (1.2 + 0.01 * skill->GetT3 ());
                skill->SetPlus ((9.9 * skill->GetLevel () * skill->GetLevel () - 8.6 * skill->GetLevel () + 858) * (1 + 0.07 * skill->GetT2 ()));
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1.2 + 0.02 * skill->GetT3 ());
                skill->SetPlus ((9.9 * skill->GetLevel () * skill->GetLevel () - 8.6 * skill->GetLevel () + 858) * (1 + 0.07 * skill->GetT2 ()));
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1.2 + 0.03 * skill->GetT3 ());
                skill->SetPlus ((9.9 * skill->GetLevel () * skill->GetLevel () - 8.6 * skill->GetLevel () + 858) * (1 + 0.07 * skill->GetT2 ()));
                skill->GetPlayer ()->SetPerform (0);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State5:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1.2 + 0.04 * skill->GetT3 ());
                skill->SetPlus ((9.9 * skill->GetLevel () * skill->GetLevel () - 8.6 * skill->GetLevel () + 858) * (1 + 0.07 * skill->GetT2 ()));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill775Stub ():SkillStub (775)
        {
            occupation = 14;
            name = L"Ä§»êÌìÖä";
            nativename = "Ä§»êÌìÖä";
            icon = "Ä§»êÌìÖä.dds";
            maxlevel = 7;
            maxlearn = 6;
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
            skill_class = 0;
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
            talent[0] = 640;
            talent[1] = 644;
            talent[2] = 661;
            talent[3] = 663;
            talent_size = 4;
            action[0] = "¹íÍõ×Ú_Ä§»êÌìÖä";
            action[1] = "¹íÍõ×Ú_Ä§»êÌìÖä";
            action[2] = "¹íÍõ×Ú_Ä§»êÌìÖä";
            action[3] = "¹íÍõ×Ú_Ä§»êÌìÖä";
            action[4] = "¹íÍõ×Ú_Ä§»êÌìÖä";
            action[5] = "¹íÍõ×Ú_Ä§»êÌìÖä";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_Ä§»êÌìÖä";
            action[16] = "¹íÍõ×Ú_Æï³Ë_Ä§»êÌìÖä";
            action[17] = "¹íÍõ×Ú_Æï³Ë_Ä§»êÌìÖä";
            action[18] = "¹íÍõ×Ú_Æï³Ë_Ä§»êÌìÖä";
            action[19] = "¹íÍõ×Ú_Æï³Ë_Ä§»êÌìÖä";
            action[20] = "¹íÍõ×Ú_Æï³Ë_Ä§»êÌìÖä";
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
            statestub.push_back (new State3 ());
            statestub.push_back (new State4 ());
            statestub.push_back (new State5 ());
#endif
        }
        virtual ~ Skill775Stub ()
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
            return (float) (14);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (425 + 5 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 9000 - 1350 * skill->GetT3 () - 1350 * skill->GetT2 ();
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
        float GetPraydistance (Skill * skill) const
        {
            return (float) (3);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (425 + 5 * skill->GetLevel ()),
                               20,
                               INT (9.9 * skill->GetLevel () * skill->GetLevel () - 8.6 * skill->GetLevel () + 858), 160 + 40 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (30 + 6 * skill->GetT0 ()));
            skill->GetVictim ()->SetTime (11000 + 100 + 3000 * skill->GetT0 ());
            skill->GetVictim ()->SetRatio ((0.11 + 0.03 * skill->GetT1 ()) * (INT (zrand (100) / (100 - 6 * skill->GetT1 ())) + 1));
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetTiansha (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (2000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (160 + 40 * skill->GetLevel ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            return true;
        }
#endif
    };
}
#endif
