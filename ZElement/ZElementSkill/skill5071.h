#ifndef __CPPGEN_GNET_SKILL5071
#define __CPPGEN_GNET_SKILL5071
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill5071:public Skill
    {
      public:
        enum
        { SKILL_ID = 5071 };
          Skill5071 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill5071Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1000;
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
                return 1000;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (1.1 + 0.02 * 7 + 0.05 * 3);
                skill->SetPlus (375 + 21 * 7);
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
                return 1000;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (1.1 + 0.02 * 7 + 0.05 * 3);
                skill->SetPlus (375 + 21 * 7);
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
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (1.1 + 0.02 * 7 + 0.05 * 3);
                skill->SetPlus (375 + 21 * 7);
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
                return 500;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (1.1 + 0.02 * 7 + 0.05 * 3);
                skill->SetPlus (375 + 21 * 7);
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
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (1.1 + 0.02 * 7 + 0.05 * 3);
                skill->SetPlus (375 + 21 * 7);
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
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (1.1 + 0.02 * 7 + 0.05 * 3);
                skill->SetPlus (375 + 21 * 7);
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
                return 100;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (1.1 + 0.02 * 7 + 0.05 * 3);
                skill->SetPlus (375 + 21 * 7);
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
                return 100;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (1.1 + 0.02 * 7 + 0.05 * 3);
                skill->SetPlus (375 + 21 * 7);
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
                return 100;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (1.1 + 0.02 * 7 + 0.05 * 3);
                skill->SetPlus (375 + 21 * 7);
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill5071Stub ():SkillStub (5071)
        {
            occupation = 128;
            name = L"ÄÏÎ×Ìì»ğ";
            nativename = "ÄÏÎ×Ìì»ğ";
            icon = "ÄÏÎ×Ìì»ğ.dds";
            maxlevel = 1;
            maxlearn = 1;
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
            action[0] = "·ÙÏã¹È_ÕÙ»½ÔÉÊ¯";
            action[1] = "·ÙÏã¹È_ÕÙ»½ÔÉÊ¯";
            action[2] = "·ÙÏã¹È_ÕÙ»½ÔÉÊ¯";
            action[3] = "·ÙÏã¹È_ÕÙ»½ÔÉÊ¯";
            action[4] = "·ÙÏã¹È_ÕÙ»½ÔÉÊ¯";
            action[5] = "·ÙÏã¹È_ÕÙ»½ÔÉÊ¯";
            action[6] = "·ÙÏã¹È_ÕÙ»½ÔÉÊ¯";
            action[7] = "·ÙÏã¹È_ÕÙ»½ÔÉÊ¯";
            action[8] = "·ÙÏã¹È_ÕÙ»½ÔÉÊ¯";
            action[9] = "·ÙÏã¹È_ÕÙ»½ÔÉÊ¯";
            action[10] = "·ÙÏã¹È_ÕÙ»½ÔÉÊ¯";
            action[11] = "·ÙÏã¹È_ÕÙ»½ÔÉÊ¯";
            action[12] = "·ÙÏã¹È_ÕÙ»½ÔÉÊ¯";
            action[13] = "0";
            action[14] = "0";
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
            action[28] = "0";
            action[29] = "0";
            action[30] = "¼¼ÄÜ¹¥»÷1";
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
        virtual ~ Skill5071Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetHpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetInkcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1000);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 5000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 16000 - 3000 * 3;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (20 + 7);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 1000, 1, 15, (10 + 2 * 7), (375 + 21 * 7), (20 + 7), 8);

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
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetVar1 () - skill->GetPlayer ()->GetDef ());
            skill->GetVictim ()->SetDirecthurt (1);
            skill->GetVictim ()->SetProbability (1.0 * (8 + 2 * 3));
            skill->GetVictim ()->SetTime (14100);
            skill->GetVictim ()->SetRatio (8);
            skill->GetVictim ()->SetBuffid (zrand (4) + 1);
            skill->GetVictim ()->SetAmount (9);
            skill->GetVictim ()->SetValue ((0.15 + 0.02 * 3) * skill->GetPlayer ()->GetVar1 ());
            skill->GetVictim ()->SetFiring (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (zrand (15) + 1 + 12);
        }
#endif
    };
}
#endif
