#ifndef __CPPGEN_GNET_SKILL3575
#define __CPPGEN_GNET_SKILL3575
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3575:public Skill
    {
      public:
        enum
        { SKILL_ID = 3575 };
          Skill3575 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3575Stub:public SkillStub
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
                skill->SetCrithurt (0.01 + 0.05 * int (skill->GetLevel () / 2) * (zrand (100) < (1 + 2 * int (skill->GetLevel () / 2)) ? 1 : 0));
                  skill->SetRatio (1 + 0.01 * (4 * skill->GetLevel () + int (skill->GetLevel () / 15) * 8 * (skill->GetLevel () - 15)) +0.04 *
                                   skill->GetT0 ());
                  skill->SetPlus (375 + 21 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
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
                skill->SetRatio (1 + 0.01 * (4 * skill->GetLevel () + int (skill->GetLevel () / 15) * 8 * (skill->GetLevel () - 15)) +0.04 *
                                 skill->GetT0 ());
                skill->SetPlus (375 + 21 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
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
                skill->SetRatio (1 + 0.01 * (4 * skill->GetLevel () + int (skill->GetLevel () / 15) * 8 * (skill->GetLevel () - 15)) +0.04 *
                                 skill->GetT0 ());
                skill->SetPlus (375 + 21 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
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
                skill->SetCrithurt (0.01 + 0.05 * int (skill->GetLevel () / 2) * (zrand (100) < (1 + 3 * int (skill->GetLevel () / 2)) ? 1 : 0));
                  skill->SetRatio (1 + 0.01 * (4 * skill->GetLevel () + int (skill->GetLevel () / 15) * 8 * (skill->GetLevel () - 15)) +0.04 *
                                   skill->GetT0 ());
                  skill->SetPlus (375 + 21 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
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
                skill->SetRatio (1 + 0.01 * (4 * skill->GetLevel () + int (skill->GetLevel () / 15) * 8 * (skill->GetLevel () - 15)) +0.04 *
                                 skill->GetT0 ());
                skill->SetPlus (375 + 21 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
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
                skill->SetRatio (1 + 0.01 * (4 * skill->GetLevel () + int (skill->GetLevel () / 15) * 8 * (skill->GetLevel () - 15)) +0.04 *
                                 skill->GetT0 ());
                skill->SetPlus (375 + 21 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
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
                skill->SetCrithurt (0.01 + 0.05 * int (skill->GetLevel () / 2) * (zrand (100) < (1 + 5 * int (skill->GetLevel () / 2)) ? 1 : 0));
                  skill->SetRatio (1 + 0.01 * (4 * skill->GetLevel () + int (skill->GetLevel () / 15) * 8 * (skill->GetLevel () - 15)) +0.04 *
                                   skill->GetT0 ());
                  skill->SetPlus (375 + 21 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
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
                skill->SetRatio (1 + 0.01 * (4 * skill->GetLevel () + int (skill->GetLevel () / 15) * 8 * (skill->GetLevel () - 15)) +0.04 *
                                 skill->GetT0 ());
                skill->SetPlus (375 + 21 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
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
                skill->SetRatio (1 + 0.01 * (4 * skill->GetLevel () + int (skill->GetLevel () / 15) * 8 * (skill->GetLevel () - 15)) +0.04 *
                                 skill->GetT0 ());
                skill->SetPlus (375 + 21 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill3575Stub ():SkillStub (3575)
        {
            occupation = 168;
            name = L"ÄÏÎ×Ìì»ğ<É·>";
            nativename = "ÄÏÎ×Ìì»ğ<É·>";
            icon = "Ä§¡¤ÄÏÎ×Ìì»ğ.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            clearmask = 2;
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
        virtual ~ Skill3575Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (200 + 42 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3451 };
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
        float GetMpcost (Skill * skill) const
        {
            return (float) (2374 + 26 * skill->GetLevel ());
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
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) ((20 + skill->GetLevel ()) > 26 ? 26 : (20 + skill->GetLevel ()));
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
            return _snwprintf (buffer, length, format,
                               2374 + 26 * skill->GetLevel (),
                               200 + 42 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel (),
                               16,
                               20 + int (skill->GetLevel () / 2),
                               4 * skill->GetLevel () + int (skill->GetLevel () / 15) * 8 * (skill->GetLevel () - 15),
                               (375 + 21 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ()),
                               (20 + skill->GetLevel ()) > 26 ? 26 : (20 + skill->GetLevel ()),
                               15,
                               1 + 2 * INT (skill->GetLevel () / 2),
                               1 + 5 * INT (skill->GetLevel () / 2),
                               1 + 3 * INT (skill->GetLevel () / 2),
                               1 + 5 * INT (skill->GetLevel () / 2), 1 + 5 * INT (skill->GetLevel () / 2), 1 + 5 * INT (skill->GetLevel () / 2));

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
            skill->GetVictim ()->SetProbability (1.0 * (15 + 5 * skill->GetT0 () - 10 * (skill->GetPlayer ()->GetHasbuff (4120) == 1 ? 1 : 0)));
            skill->GetVictim ()->SetTime (16100);
            skill->GetVictim ()->SetRatio (8);
            skill->GetVictim ()->SetBuffid (skill->GetPlayer ()->GetHasbuff (4120) == 1 ? (zrand (2) + 1) : (zrand (5) + 1));
            skill->GetVictim ()->SetAmount (9);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetVar1 () * 0.05);
            skill->GetVictim ()->SetFiring (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (20 + int (skill->GetLevel () / 2) + 4 * skill->GetT2 ());
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
