#ifndef __CPPGEN_GNET_SKILL3299
#define __CPPGEN_GNET_SKILL3299
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3299:public Skill
    {
      public:
        enum
        { SKILL_ID = 3299 };
          Skill3299 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3299Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 300;
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
                return 450;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1.62 + 0.05 * 3 + 0.04 * 3);
                skill->SetPlus ((720 + 30 * 3 + 7 * 3 * 3) * (1 + 0.07 * 3));
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
                return 450;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1.62 + 0.04 * 3);
                skill->SetPlus ((720 + 30 * 3 + 7 * 3 * 3) * (1 + 0.07 * 3));
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
                return 450;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1.62 + 0.04 * 3);
                skill->SetPlus ((720 + 30 * 3 + 7 * 3 * 3) * (1 + 0.07 * 3));
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
                return 450;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1.62 + 0.04 * 3);
                skill->SetPlus ((720 + 30 * 3 + 7 * 3 * 3) * (1 + 0.07 * 3));
                skill->GetPlayer ()->SetPerform (0);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State6:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 450;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1.62 + 0.04 * 3);
                skill->SetPlus ((720 + 30 * 3 + 7 * 3 * 3) * (1 + 0.07 * 3));
                skill->GetPlayer ()->SetPerform (0);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State7:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 450;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1.62 + 0.15 + 0.04 * 3);
                skill->SetPlus ((720 + 30 * 3 + 7 * 3 * 3) * (1 + 0.07 * 3));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill3299Stub ():SkillStub (3299)
        {
            occupation = 128;
            name = L"Î´ÃûÕ¶";
            nativename = "Î´ÃûÕ¶";
            icon = "Î´ÃûÕ¶.dds";
            maxlevel = 1;
            maxlearn = 1;
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
            action[0] = "¹íÍõ×Ú_Î´ÃûÕ¶";
            action[1] = "¹íÍõ×Ú_Î´ÃûÕ¶";
            action[2] = "¹íÍõ×Ú_Î´ÃûÕ¶";
            action[3] = "¹íÍõ×Ú_Î´ÃûÕ¶";
            action[4] = "¹íÍõ×Ú_Î´ÃûÕ¶";
            action[5] = "¹íÍõ×Ú_Î´ÃûÕ¶";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_Î´ÃûÕ¶";
            action[16] = "¹íÍõ×Ú_Æï³Ë_Î´ÃûÕ¶";
            action[17] = "¹íÍõ×Ú_Æï³Ë_Î´ÃûÕ¶";
            action[18] = "¹íÍõ×Ú_Æï³Ë_Î´ÃûÕ¶";
            action[19] = "¹íÍõ×Ú_Æï³Ë_Î´ÃûÕ¶";
            action[20] = "¹íÍõ×Ú_Æï³Ë_Î´ÃûÕ¶";
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
            statestub.push_back (new State5 ());
            statestub.push_back (new State6 ());
            statestub.push_back (new State7 ());
#endif
        }
        virtual ~ Skill3299Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12 + 0.6 * 3);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (500);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 15000 - 2000 * 3 - 2250 * 3;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (9 + 0.6 * 3);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7 + 0.6 * 3);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 500, 62, INT (720 + 30 * 3 + 7 * 3 * 3), 8, 33, 30, 30, 6, 210 + 15 * 3, 6, 33, 10 * 3, 15);

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
            skill->GetVictim ()->SetProbability (1.0 * (210 + 15 * 3 + 9 * 3 * 3));
            skill->GetVictim ()->SetTime (6000 + 100);
            skill->GetVictim ()->SetRatio (0.3 + 0.05 * 3);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetWeak (1);
            skill->GetVictim ()->SetProbability (1.0 * (33 * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0.1 * 3 * skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetMpleak (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (33 + 15 * 3));
            skill->GetVictim ()->SetTime (8000 + 100);
            skill->GetVictim ()->SetRatio (0.3 + 0.02 * 3);
            skill->GetVictim ()->SetDecdamage (1);
            return true;
        }
#endif
    };
}
#endif
