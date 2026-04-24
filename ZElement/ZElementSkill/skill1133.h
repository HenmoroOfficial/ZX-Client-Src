#ifndef __CPPGEN_GNET_SKILL1133
#define __CPPGEN_GNET_SKILL1133
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1133:public Skill
    {
      public:
        enum
        { SKILL_ID = 1133 };
          Skill1133 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1133Stub:public SkillStub
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
                skill->SetRatio (1 + 0.05 * skill->GetT1 ());
                skill->SetCrit (0.01 * skill->GetLevel ());
                skill->SetPlus (35 * skill->GetLevel () * skill->GetT0 () + 860 + 45 * skill->GetLevel () +
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
                skill->SetRatio (1 + 0.05 * skill->GetT1 ());
                skill->SetCrit (0.01 * skill->GetLevel () + 0.01);
                skill->SetPlus (35 * skill->GetLevel () * skill->GetT0 () + 860 + 45 * skill->GetLevel () +
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
                skill->SetRatio (1 + 0.05 * skill->GetT1 ());
                skill->SetCrit (0.01 * skill->GetLevel () + 0.02);
                skill->SetPlus (35 * skill->GetLevel () * skill->GetT0 () + 860 + 45 * skill->GetLevel () +
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
                skill->SetRatio (1 + 0.05 * skill->GetT1 ());
                skill->SetCrit (0.01 * skill->GetLevel () + 0.03);
                skill->SetCrithurt (1);
                skill->SetPlus (35 * skill->GetLevel () * skill->GetT0 () + 860 + 45 * skill->GetLevel () +
                                2 * skill->GetLevel () * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill1133Stub ():SkillStub (1133)
        {
            occupation = 29;
            name = L"Éñ¹íÂÒÎè";
            nativename = "Éñ¹íÂÒÎè";
            icon = "Éñ¹íÂÒÎè.dds";
            maxlevel = 5;
            maxlearn = 4;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 2;
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
            talent[0] = 1166;
            talent[1] = 1172;
            talent_size = 2;
            action[0] = "Éñ¹íÂÒÎè";
            action[1] = "Éñ¹íÂÒÎè";
            action[2] = "Éñ¹íÂÒÎè";
            action[3] = "Éñ¹íÂÒÎè";
            action[4] = "Éñ¹íÂÒÎè";
            action[5] = "Éñ¹íÂÒÎè";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "Éñ¹íÂÒÎè";
            action[16] = "Éñ¹íÂÒÎè";
            action[17] = "Éñ¹íÂÒÎè";
            action[18] = "Éñ¹íÂÒÎè";
            action[19] = "Éñ¹íÂÒÎè";
            action[20] = "Éñ¹íÂÒÎè";
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
        virtual ~ Skill1133Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1132 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
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
            return 11000 - 1000 * skill->GetLevel ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[4] = { 135, 135, 136, 138 };
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
                               860 + 45 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel (),
                               skill->GetLevel (),
                               skill->GetLevel () + 1,
                               skill->GetLevel () + 2,
                               skill->GetLevel () + 3,
                               11 - skill->GetLevel (),
                               15, 2 * skill->GetLevel (), 15, 1000 * skill->GetLevel (), 25, 2 * skill->GetLevel (), 15, 5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetRatio (0.02001 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDeccritrate (1);
            skill->GetVictim ()->SetProbability (1.0 * (15));
            skill->GetVictim ()->SetTime (12000 + 100);
            skill->GetVictim ()->SetRatio (0.05 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecdodge (1);
            skill->GetVictim ()->SetProbability (1.0 * (15));
            skill->GetVictim ()->SetTime (12000 + 100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0.25 * skill->GetPlayer ()->GetMaxmp () + 1);
            skill->GetVictim ()->SetValue (1000 * skill->GetLevel ());
            skill->GetVictim ()->SetSubmp (1);
            skill->GetVictim ()->SetProbability (1.0 * (norm (zrand (100) / (100 - 2 * skill->GetLevel ())) * 600));
            skill->GetVictim ()->SetTime (12000 + 100);
            skill->GetVictim ()->SetSilent (1);
            return true;
        }
#endif
    };
}
#endif
