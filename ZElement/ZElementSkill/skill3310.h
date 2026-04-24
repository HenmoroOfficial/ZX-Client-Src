#ifndef __CPPGEN_GNET_SKILL3310
#define __CPPGEN_GNET_SKILL3310
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3310:public Skill
    {
      public:
        enum
        { SKILL_ID = 3310 };
          Skill3310 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3310Stub:public SkillStub
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
                skill->SetRatio (1 + 0.05 * 3);
                skill->SetCrit (0.01 * 4);
                skill->SetPlus (35 * 4 * 3 + 860 + 45 * 4 + 2 * 4 * 4);
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
                skill->SetRatio (1 + 0.05 * 3);
                skill->SetCrit (0.01 * 4 + 0.01);
                skill->SetPlus (35 * 4 * 3 + 860 + 45 * 4 + 2 * 4 * 4);
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
                skill->SetRatio (1 + 0.05 * 3);
                skill->SetCrit (0.01 * 4 + 0.02);
                skill->SetPlus (35 * 4 * 3 + 860 + 45 * 4 + 2 * 4 * 4);
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
                skill->SetRatio (1 + 0.05 * 3);
                skill->SetCrit (0.01 * 4 + 0.03);
                skill->SetCrithurt (1);
                skill->SetPlus (35 * 4 * 3 + 860 + 45 * 4 + 2 * 4 * 4);
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill3310Stub ():SkillStub (3310)
        {
            occupation = 128;
            name = L"Éñ¹íÂÒÎè";
            nativename = "Éñ¹íÂÒÎè";
            icon = "Éñ¹íÂÒÎè.dds";
            maxlevel = 1;
            maxlearn = 1;
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
        virtual ~ Skill3310Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (15);
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
            return 11000 - 1000 * skill->GetLevel ();
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
                               1000, 860 + 45 * 4 + 2 * 4 * 4, 4, 4 + 1, 4 + 2, 4 + 3, 11 - 4, 15, 2 * 4, 15, 1000 * 4, 25, 2 * 4, 15, 5 * 4);

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
            skill->GetVictim ()->SetProbability (1.0 * (15));
            skill->GetVictim ()->SetTime (12000 + 100);
            skill->GetVictim ()->SetRatio (0.02001 * 4);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDeccritrate (1);
            skill->GetVictim ()->SetProbability (1.0 * (15));
            skill->GetVictim ()->SetTime (12000 + 100);
            skill->GetVictim ()->SetRatio (0.05 * 4);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecdodge (1);
            skill->GetVictim ()->SetProbability (1.0 * (15));
            skill->GetVictim ()->SetTime (12000 + 100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0.25 * skill->GetPlayer ()->GetMaxmp () + 1);
            skill->GetVictim ()->SetValue (1000 * 4);
            skill->GetVictim ()->SetSubmp (1);
            skill->GetVictim ()->SetProbability (1.0 * (norm (zrand (100) / (100 - 2 * 4)) * 600));
            skill->GetVictim ()->SetTime (12000 + 100);
            skill->GetVictim ()->SetSilent (1);
            return true;
        }
#endif
    };
}
#endif
