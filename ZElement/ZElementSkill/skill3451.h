#ifndef __CPPGEN_GNET_SKILL3451
#define __CPPGEN_GNET_SKILL3451
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3451:public Skill
    {
      public:
        enum
        { SKILL_ID = 3451 };
          Skill3451 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3451Stub:public SkillStub
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
                return 300;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (1 + 0.03 * skill->GetLevel () + 0.04 * skill->GetT0 ());
                skill->SetPlus (375 + 21 * skill->GetLevel ());
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
                return 700;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (1 + 0.03 * skill->GetLevel () + 0.04 * skill->GetT0 ());
                skill->SetPlus (375 + 21 * skill->GetLevel ());
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
                return 700;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (1 + 0.03 * skill->GetLevel () + 0.04 * skill->GetT0 ());
                skill->SetPlus (375 + 21 * skill->GetLevel ());
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
                return 700;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (1 + 0.03 * skill->GetLevel () + 0.04 * skill->GetT0 ());
                skill->SetPlus (375 + 21 * skill->GetLevel ());
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
                return 300;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (1 + 0.03 * skill->GetLevel () + 0.04 * skill->GetT0 ());
                skill->SetPlus (375 + 21 * skill->GetLevel ());
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
                return 300;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (1 + 0.03 * skill->GetLevel () + 0.04 * skill->GetT0 ());
                skill->SetPlus (375 + 21 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State8:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 500;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (1 + 0.03 * skill->GetLevel () + 0.04 * skill->GetT0 ());
                skill->SetPlus (375 + 21 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State9:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 500;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (1 + 0.03 * skill->GetLevel () + 0.04 * skill->GetT0 ());
                skill->SetPlus (375 + 21 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State10:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 500;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (1 + 0.03 * skill->GetLevel () + 0.04 * skill->GetT0 ());
                skill->SetPlus (375 + 21 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill3451Stub ():SkillStub (3451)
        {
            occupation = 68;
            name = L"ÄÏÎ×Ìì»ð";
            nativename = "ÄÏÎ×Ìì»ð";
            icon = "ÄÏÎ×Ìì»ð.dds";
            maxlevel = 7;
            maxlearn = 6;
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
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 9;
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
            talent[0] = 3381;
            talent[1] = 3384;
            talent[2] = 3389;
            talent_size = 3;
            action[0] = "·ÙÏã¹È_ÕÙ»½ÔÉÊ¯";
            action[1] = "·ÙÏã¹È_ÕÙ»½ÔÉÊ¯";
            action[2] = "·ÙÏã¹È_ÕÙ»½ÔÉÊ¯";
            action[3] = "·ÙÏã¹È_ÕÙ»½ÔÉÊ¯";
            action[4] = "·ÙÏã¹È_ÕÙ»½ÔÉÊ¯";
            action[5] = "·ÙÏã¹È_ÕÙ»½ÔÉÊ¯";
            action[6] = "·ÙÏã¹È_ÕÙ»½ÔÉÊ¯";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "·ÙÏã¹È_Æï³Ë_ÕÙ»½ÔÉÊ¯";
            action[16] = "·ÙÏã¹È_Æï³Ë_ÕÙ»½ÔÉÊ¯";
            action[17] = "·ÙÏã¹È_Æï³Ë_ÕÙ»½ÔÉÊ¯";
            action[18] = "·ÙÏã¹È_Æï³Ë_ÕÙ»½ÔÉÊ¯";
            action[19] = "·ÙÏã¹È_Æï³Ë_ÕÙ»½ÔÉÊ¯";
            action[20] = "·ÙÏã¹È_Æï³Ë_ÕÙ»½ÔÉÊ¯";
            action[21] = "·ÙÏã¹È_Æï³Ë_ÕÙ»½ÔÉÊ¯";
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
            statestub.push_back (new State6 ());
            statestub.push_back (new State7 ());
            statestub.push_back (new State8 ());
            statestub.push_back (new State9 ());
            statestub.push_back (new State10 ());
#endif
        }
        virtual ~ Skill3451Stub ()
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
            return (float) (3000 - 20 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 5000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 16000 - 3000 * skill->GetT1 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[6] = { 135, 135, 135, 135, 136, 137 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (18 + skill->GetLevel ());
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (22);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (20);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               3000 - 20 * skill->GetLevel () * skill->GetLevel (),
                               20, 3 * skill->GetLevel (), (375 + 21 * skill->GetLevel ()), (20 + skill->GetLevel ()), 15);

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
            skill->GetVictim ()->SetProbability (1.0 * (15 + 5 * skill->GetT0 () - 13 * (skill->GetPlayer ()->GetHasbuff (4120) == 1 ? 1 : 0)));
            skill->GetVictim ()->SetTime (14100);
            skill->GetVictim ()->SetRatio (8);
            skill->GetVictim ()->SetBuffid (skill->GetPlayer ()->GetHasbuff (4120) == 1 ? (zrand (2) + 1) : (zrand (5) + 1));
            skill->GetVictim ()->SetAmount (9);
            skill->GetVictim ()->SetValue (0.05 * skill->GetPlayer ()->GetVar1 ());
            skill->GetVictim ()->SetFiring (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (20 + 4 * skill->GetT2 ());
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
