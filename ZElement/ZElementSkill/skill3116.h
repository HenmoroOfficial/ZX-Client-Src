#ifndef __CPPGEN_GNET_SKILL3116
#define __CPPGEN_GNET_SKILL3116
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3116:public Skill
    {
      public:
        enum
        { SKILL_ID = 3116 };
          Skill3116 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3116Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
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
                return 900;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes3 ());
                skill->SetRatio (1 + 0.05 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) +
                                 (0.05 + 0.05 * int (skill->GetPlayer ()->GetSkilllevel (234) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (242) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (543) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (785) / 3.9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (786) / 1.9) +
                                  0.05 * norm (skill->GetPlayer ()->GetSkilllevel (3115) / 0.9)) *skill->GetT2 ());
                skill->SetPlus ((800 + 12 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ()) *
                                ((100 + (zrand (10 + 15 * skill->GetT1 ()) - 10) * norm (skill->GetT1 () + 0.1)) / 100.0));
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
                return 900;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes3 ());
                skill->SetRatio (1 + 0.05 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) +
                                 (0.05 + 0.05 * int (skill->GetPlayer ()->GetSkilllevel (234) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (242) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (543) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (785) / 3.9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (786) / 1.9) +
                                  0.05 * norm (skill->GetPlayer ()->GetSkilllevel (3115) / 0.9)) *skill->GetT2 ());
                skill->SetPlus ((800 + 12 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ()) *
                                ((100 + (zrand (10 + 15 * skill->GetT1 ()) - 10) * norm (skill->GetT1 () + 0.1)) / 100.0));
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
                return 900;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes3 ());
                skill->SetRatio (1 + 0.05 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) +
                                 (0.05 + 0.05 * int (skill->GetPlayer ()->GetSkilllevel (234) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (242) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (543) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (785) / 3.9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (786) / 1.9) +
                                  0.05 * norm (skill->GetPlayer ()->GetSkilllevel (3115) / 0.9)) *skill->GetT2 ());
                skill->SetPlus ((800 + 12 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ()) *
                                ((100 + (zrand (10 + 15 * skill->GetT1 ()) - 10) * norm (skill->GetT1 () + 0.1)) / 100.0));
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
                return 900;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes3 ());
                skill->SetRatio (1 + 0.05 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) +
                                 (0.05 + 0.05 * int (skill->GetPlayer ()->GetSkilllevel (234) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (242) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (543) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (785) / 3.9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (786) / 1.9) +
                                  0.05 * norm (skill->GetPlayer ()->GetSkilllevel (3115) / 0.9)) *skill->GetT2 ());
                skill->SetPlus ((800 + 12 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ()) *
                                ((100 + (zrand (10 + 15 * skill->GetT1 ()) - 10) * norm (skill->GetT1 () + 0.1)) / 100.0));
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
                return 900;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes3 ());
                skill->SetRatio (1 + 0.05 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) +
                                 (0.05 + 0.05 * int (skill->GetPlayer ()->GetSkilllevel (234) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (242) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (543) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (785) / 3.9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (786) / 1.9) +
                                  0.05 * norm (skill->GetPlayer ()->GetSkilllevel (3115) / 0.9)) *skill->GetT2 ());
                skill->SetPlus ((800 + 12 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ()) *
                                ((100 + (zrand (10 + 15 * skill->GetT1 ()) - 10) * norm (skill->GetT1 () + 0.1)) / 100.0));
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
                return 900;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes3 ());
                skill->SetRatio (1 + 0.05 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) +
                                 (0.05 + 0.05 * int (skill->GetPlayer ()->GetSkilllevel (234) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (242) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (543) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (785) / 3.9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (786) / 1.9) +
                                  0.05 * norm (skill->GetPlayer ()->GetSkilllevel (3115) / 0.9)) *skill->GetT2 ());
                skill->SetPlus ((INT (zrand (99 + 5 * skill->GetLevel ()) / 100) + 1) *
                                (800 + 12 * skill->GetLevel () +
                                 3 * skill->GetLevel () * skill->GetLevel ()) * ((100 +
                                                                                  (zrand (10 + 15 * skill->GetT1 ()) - 10) * norm (skill->GetT1 () +
                                                                                                                                   0.1)) / 100.0));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill3116Stub ():SkillStub (3116)
        {
            occupation = 169;
            name = L"神剑御雷真诀<禅>";
            nativename = "神剑御雷真诀<禅>";
            icon = "佛·神剑御雷真诀.dds";
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
            skill_class = 2;
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
            talent[0] = 593;
            talent[1] = 607;
            talent[2] = 617;
            talent_size = 3;
            action[0] = "青云门_神剑御雷真诀";
            action[1] = "青云门_神剑御雷真诀";
            action[2] = "青云门_神剑御雷真诀";
            action[3] = "青云门_神剑御雷真诀";
            action[4] = "青云门_神剑御雷真诀";
            action[5] = "青云门_神剑御雷真诀";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "青云门_骑乘_神剑御雷真诀";
            action[16] = "青云门_骑乘_神剑御雷真诀";
            action[17] = "青云门_骑乘_神剑御雷真诀";
            action[18] = "青云门_骑乘_神剑御雷真诀";
            action[19] = "青云门_骑乘_神剑御雷真诀";
            action[20] = "青云门_骑乘_神剑御雷真诀";
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
            rangetype = 2;
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
        virtual ~ Skill3116Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (200 + 34 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 786 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 200, 500, 500, 600, 900, 2400, 3500, 5600, 8000, 9100, 21700, 36400, 51500, 77500, 79800, 179300, 290600, 437200, 872700, 1620700 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 1, 3, 5, 7, 9, 12, 15, 18, 22, 26, 30, 35, 40, 45, 50, 56, 62, 68, 74, 80 };
            return array[level - 1];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (30 * skill->GetLevel () * skill->GetLevel () + 2560);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 6000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 16000;
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
        float GetRadius (Skill * skill) const
        {
            return (float) (26);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (18);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (30 * skill->GetLevel () * skill->GetLevel () + 2560),
                               200 + 34 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel (),
                               16,
                               10 + skill->GetLevel (),
                               25,
                               INT (5 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 3 * (skill->GetLevel () - 15)),
                               INT (800 + 12 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ()),
                               2 + 2 * skill->GetLevel (),
                               4 * skill->GetLevel (),
                               2 * skill->GetLevel (),
                               6,
                               550 + 10 * skill->GetLevel (),
                               6, 2 * skill->GetLevel (), 12 + 4 * skill->GetLevel (), 0.2 * skill->GetLevel (), 40 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (zrand (100) < 2 * skill->GetLevel ()? 550 + 10 * skill->GetLevel () : 0));
            skill->GetVictim ()->SetTime (6000 + 100);
            skill->GetVictim ()->SetSilent (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () + 2 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (6000 + 100 + 500 * skill->GetT0 ());
            skill->GetVictim ()->SetWrap (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetRatio (220 + INT (zrand (100 + 2 * skill->GetLevel ()) / 100) * 2896);
            skill->GetVictim ()->SetAmount (220);
            skill->GetVictim ()->SetValue (220);
            skill->GetVictim ()->SetClearcooldown (1);
            skill->GetVictim ()->SetTime (6000 + 100);
            skill->GetVictim ()->SetImmuneweak (1);
            skill->GetVictim ()->SetProbability (1.0 * (15 + 4 * skill->GetLevel ()));
            skill->GetVictim ()->SetRatio (0.002 * skill->GetLevel ());
            skill->GetVictim ()->SetValue (40 * skill->GetLevel ());
            skill->GetVictim ()->SetScopedamage (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (10 + skill->GetLevel ());
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
