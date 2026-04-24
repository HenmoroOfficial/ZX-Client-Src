#ifndef __CPPGEN_GNET_SKILL3117
#define __CPPGEN_GNET_SKILL3117
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3117:public Skill
    {
      public:
        enum
        { SKILL_ID = 3117 };
          Skill3117 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3117Stub:public SkillStub
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
                skill->SetRatio (1.1 + 0.05 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) +
                                 0.01 * skill->GetT3 ());
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
                skill->SetRatio (1.1 + 0.05 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) +
                                 0.02 * skill->GetT3 ());
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
                skill->SetRatio (1.1 + 0.05 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) +
                                 0.03 * skill->GetT3 ());
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
                skill->SetRatio (1.1 + 0.05 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) +
                                 0.04 * skill->GetT3 ());
                skill->SetPlus ((9.9 * skill->GetLevel () * skill->GetLevel () - 8.6 * skill->GetLevel () + 858) * (1 + 0.07 * skill->GetT2 ()));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill3117Stub ():SkillStub (3117)
        {
            occupation = 169;
            name = L"Ä§»êÌìÖä<ìø>";
            nativename = "Ä§»êÌìÖä<ìø>";
            icon = "·ğ¡¤Ä§»êÌìÖä.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 0;
            credittype = 0;
            clearmask = 4;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
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
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_Ä§»êÌìÖä";
            action[16] = "¹íÍõ×Ú_Æï³Ë_Ä§»êÌìÖä";
            action[17] = "¹íÍõ×Ú_Æï³Ë_Ä§»êÌìÖä";
            action[18] = "¹íÍõ×Ú_Æï³Ë_Ä§»êÌìÖä";
            action[19] = "¹íÍõ×Ú_Æï³Ë_Ä§»êÌìÖä";
            action[20] = "¹íÍõ×Ú_Æï³Ë_Ä§»êÌìÖä";
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
        virtual ~ Skill3117Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (170 + 42 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 775 };
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
            return (float) (14);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (850 + 10 * skill->GetLevel ());
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
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (9);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (4.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (850 + 10 * skill->GetLevel ()),
                               170 + 42 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel (),
                               9,
                               INT (10 + INT (5 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 3 * (skill->GetLevel () - 15))),
                               INT (9.9 * skill->GetLevel () * skill->GetLevel () - 8.6 * skill->GetLevel () + 858),
                               3 + 0.5 * skill->GetLevel (), 56 * skill->GetLevel (), 2 + skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (3050 + 500 * skill->GetLevel ());
            skill->GetVictim ()->SetAmount (2 + skill->GetLevel ());
            skill->GetVictim ()->SetValue (56 * skill->GetLevel ());
            skill->GetVictim ()->SetCursed (1);
            return true;
        }
#endif
    };
}
#endif
