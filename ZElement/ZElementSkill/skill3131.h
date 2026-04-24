#ifndef __CPPGEN_GNET_SKILL3131
#define __CPPGEN_GNET_SKILL3131
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3131:public Skill
    {
      public:
        enum
        { SKILL_ID = 3131 };
          Skill3131 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3131Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes1 ());
                skill->SetRatio (1 + 0.03 * skill->GetLevel () + 0.05 * skill->GetT1 ());
                skill->SetCrit (0.01 * skill->GetLevel ());
                skill->SetPlus (skill->GetPlayer ()->GetRes1 () * (0.1 * skill->GetLevel () + norm (skill->GetLevel () / 19.9)) +
                                35 * skill->GetLevel () * skill->GetT0 () + 860 + 45 * skill->GetLevel () +
                                2 * skill->GetLevel () * skill->GetLevel ());
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes1 ());
                skill->SetRatio (1 + 0.03 * skill->GetLevel () + 0.05 * skill->GetT1 ());
                skill->SetCrit (0.01 * skill->GetLevel () + 0.01);
                skill->SetPlus (skill->GetPlayer ()->GetRes1 () * (0.1 * skill->GetLevel () + norm (skill->GetLevel () / 19.9)) +
                                35 * skill->GetLevel () * skill->GetT0 () + 860 + 45 * skill->GetLevel () +
                                2 * skill->GetLevel () * skill->GetLevel ());
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes1 ());
                skill->SetRatio (1 + 0.03 * skill->GetLevel () + 0.05 * skill->GetT1 ());
                skill->SetCrit (0.01 * skill->GetLevel () + 0.02);
                skill->SetPlus (skill->GetPlayer ()->GetRes1 () * (0.1 * skill->GetLevel () + norm (skill->GetLevel () / 19.9)) +
                                35 * skill->GetLevel () * skill->GetT0 () + 860 + 45 * skill->GetLevel () +
                                2 * skill->GetLevel () * skill->GetLevel ());
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes1 ());
                skill->SetRatio (1 + 0.03 * skill->GetLevel () + 0.05 * skill->GetT1 ());
                skill->SetCrit (0.01 * skill->GetLevel () + 0.03);
                skill->SetCrithurt (1);
                skill->SetPlus (skill->GetPlayer ()->GetRes1 () * (0.1 * skill->GetLevel () + norm (skill->GetLevel () / 19.9)) +
                                35 * skill->GetLevel () * skill->GetT0 () + 860 + 45 * skill->GetLevel () +
                                2 * skill->GetLevel () * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill3131Stub ():SkillStub (3131)
        {
            occupation = 169;
            name = L"Éñ¹íÂÒÎè<ìø>";
            nativename = "Éñ¹íÂÒÎè<ìø>";
            icon = "·ğ¡¤Éñ¹íÂÒÎè.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 2;
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
            talent[0] = 1166;
            talent[1] = 1172;
            talent_size = 2;
            action[0] = "Éñ¹íÂÒÎè";
            action[1] = "Éñ¹íÂÒÎè";
            action[2] = "Éñ¹íÂÒÎè";
            action[3] = "Éñ¹íÂÒÎè";
            action[4] = "Éñ¹íÂÒÎè";
            action[5] = "Éñ¹íÂÒÎè";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "Éñ¹íÂÒÎè";
            action[16] = "Éñ¹íÂÒÎè";
            action[17] = "Éñ¹íÂÒÎè";
            action[18] = "Éñ¹íÂÒÎè";
            action[19] = "Éñ¹íÂÒÎè";
            action[20] = "Éñ¹íÂÒÎè";
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
        virtual ~ Skill3131Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (170 + 42 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1133 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 4 };
            return array[index];
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 200, 500, 500, 600, 900, 1900, 3200, 4500, 8600, 10800, 20700, 32200, 51000, 65200, 84800, 159900, 260100, 427500, 743600,
 1450600 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 1, 3, 5, 7, 9, 11, 14, 17, 21, 25, 29, 34, 39, 44, 49, 55, 61, 67, 73, 79 };
            return array[level - 1];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (700 + 25 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 11000 - 500 * skill->GetLevel ();
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
            return (float) (2.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (700 + 25 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel ()),
                               170 + 42 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel (),
                               11 - 0.5 * skill->GetLevel (),
                               3 * skill->GetLevel (),
                               860 + 45 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel (),
                               skill->GetLevel (),
                               skill->GetLevel () + 1,
                               skill->GetLevel () + 2,
                               skill->GetLevel () + 3,
                               5 + skill->GetLevel (),
                               4 + skill->GetLevel (),
                               5 + skill->GetLevel (),
                               skill->GetLevel (), 25, skill->GetLevel (), 15, 5 * skill->GetLevel (), 0.1 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (15));
            skill->GetVictim ()->SetTime (12000 + 100);
            skill->GetVictim ()->SetRatio (0.04 + 0.01001 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDeccritrate (1);
            skill->GetVictim ()->SetProbability (1.0 * (15));
            skill->GetVictim ()->SetTime (12000 + 100);
            skill->GetVictim ()->SetRatio (0.05 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecdodge (1);
            skill->GetVictim ()->SetProbability (1.0 * (5 + skill->GetLevel ()));
            skill->GetVictim ()->SetTime (12000 + 100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0.25 * skill->GetPlayer ()->GetMaxmp () + 1);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetVar1 () * skill->GetLevel ());
            skill->GetVictim ()->SetSubmp (1);
            skill->GetVictim ()->SetProbability (1.0 * (norm (zrand (100) / (100 - 2 * skill->GetLevel ())) * 650));
            skill->GetVictim ()->SetTime (12000 + 100);
            skill->GetVictim ()->SetSilent (1);
            return true;
        }
#endif
    };
}
#endif
