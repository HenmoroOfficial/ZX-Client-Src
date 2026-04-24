#ifndef __CPPGEN_GNET_SKILL552
#define __CPPGEN_GNET_SKILL552
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill552:public Skill
    {
      public:
        enum
        { SKILL_ID = 552 };
          Skill552 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill552Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1500;
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
                skill->GetPlayer ()->SetVar1 (1);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetRes5 ());
                skill->SetRatio (1 + 0.07 * skill->GetT1 ());
                skill->SetCrit ((0.01 * skill->GetT0 () + 0.01) * int (skill->GetLevel () / 9));
                  skill->SetPlus (skill->GetPlayer ()->GetRes5 () * int (skill->GetLevel () / 9) * (skill->GetT0 () + 1) * norm (skill->GetT0 ()) +
                                  INT (1.42 - 0.05 * skill->GetLevel ()) * (0.089 * skill->GetLevel () * skill->GetLevel () +
                                                                            17 * skill->GetLevel () + 742) + INT (0.05 * skill->GetLevel () +
                                                                                                                  0.55) * (81 * skill->GetLevel () +
                                                                                                                           596 -
                                                                                                                           1.8 * skill->GetLevel () *
                                                                                                                           skill->GetLevel ()));
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
                skill->GetPlayer ()->SetVar1 (1);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetRes5 ());
                skill->SetRatio (1 + 0.07 * skill->GetT1 ());
                skill->SetCrit ((0.01 * skill->GetT0 () + 0.02) * int (skill->GetLevel () / 9));
                  skill->SetPlus (skill->GetPlayer ()->GetVar2 () * int (skill->GetLevel () / 9) * (skill->GetT0 () + 2) * norm (skill->GetT0 ()) +
                                  INT (1.42 - 0.05 * skill->GetLevel ()) * (0.089 * skill->GetLevel () * skill->GetLevel () +
                                                                            17 * skill->GetLevel () + 742) + INT (0.05 * skill->GetLevel () +
                                                                                                                  0.55) * (81 * skill->GetLevel () +
                                                                                                                           596 -
                                                                                                                           1.8 * skill->GetLevel () *
                                                                                                                           skill->GetLevel ()));
                  skill->GetPlayer ()->SetPerform (1);
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
                return 1000;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (1);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetRes5 ());
                skill->SetRatio (1 + 0.07 * skill->GetT1 ());
                skill->SetCrit ((0.01 * skill->GetT0 () + 0.03) * int (skill->GetLevel () / 9));
                  skill->SetPlus (skill->GetPlayer ()->GetVar2 () * int (skill->GetLevel () / 9) * (skill->GetT0 () + 3) * norm (skill->GetT0 ()) +
                                  INT (1.42 - 0.05 * skill->GetLevel ()) * (0.089 * skill->GetLevel () * skill->GetLevel () +
                                                                            17 * skill->GetLevel () + 742) + INT (0.05 * skill->GetLevel () +
                                                                                                                  0.55) * (81 * skill->GetLevel () +
                                                                                                                           596 -
                                                                                                                           1.8 * skill->GetLevel () *
                                                                                                                           skill->GetLevel ()));
                  skill->GetPlayer ()->SetPerform (1);
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
                skill->GetPlayer ()->SetVar1 (0);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetRes5 ());
                skill->SetRatio (1 + 0.07 * skill->GetT1 ());
                skill->SetCrit ((0.01 * skill->GetT0 () + 0.04) * int (skill->GetLevel () / 9));
                  skill->SetPlus (skill->GetPlayer ()->GetVar2 () * int (skill->GetLevel () / 9) * (skill->GetT0 () + 4) * norm (skill->GetT0 ()) +
                                  INT (1.42 - 0.05 * skill->GetLevel ()) * (0.089 * skill->GetLevel () * skill->GetLevel () +
                                                                            17 * skill->GetLevel () + 742) + INT (0.05 * skill->GetLevel () +
                                                                                                                  0.55) * (81 * skill->GetLevel () +
                                                                                                                           596 -
                                                                                                                           1.8 * skill->GetLevel () *
                                                                                                                           skill->GetLevel ()));
                  skill->GetPlayer ()->SetPerform (1);
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
                return 500;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (0);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetRes5 ());
                skill->SetRatio (1 + 0.07 * skill->GetT1 ());
                skill->SetCrit ((0.01 * skill->GetT0 () + 0.05) * int (skill->GetLevel () / 9));
                  skill->SetPlus (skill->GetPlayer ()->GetVar2 () * int (skill->GetLevel () / 9) * (skill->GetT0 () + 5) * norm (skill->GetT0 ()) +
                                  INT (1.42 - 0.05 * skill->GetLevel ()) * (0.089 * skill->GetLevel () * skill->GetLevel () +
                                                                            17 * skill->GetLevel () + 742) + INT (0.05 * skill->GetLevel () +
                                                                                                                  0.55) * (81 * skill->GetLevel () +
                                                                                                                           596 -
                                                                                                                           1.8 * skill->GetLevel () *
                                                                                                                           skill->GetLevel ()));
                  skill->GetPlayer ()->SetPerform (1);
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
                return 500;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (0);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetRes5 ());
                skill->SetRatio (1 + 0.07 * skill->GetT1 ());
                skill->SetCrit ((0.01 * skill->GetT0 () + 0.06) * int (skill->GetLevel () / 9));
                  skill->SetPlus (skill->GetPlayer ()->GetVar2 () * int (skill->GetLevel () / 9) * (skill->GetT0 () + 6) * norm (skill->GetT0 ()) +
                                  INT (1.42 - 0.05 * skill->GetLevel ()) * (0.089 * skill->GetLevel () * skill->GetLevel () +
                                                                            17 * skill->GetLevel () + 742) + INT (0.05 * skill->GetLevel () +
                                                                                                                  0.55) * (81 * skill->GetLevel () +
                                                                                                                           596 -
                                                                                                                           1.8 * skill->GetLevel () *
                                                                                                                           skill->GetLevel ()));
                  skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill552Stub ():SkillStub (552)
        {
            occupation = 22;
            name = L"ÁùµÀÂÖ»Ø";
            nativename = "ÁùµÀÂÖ»Ø";
            icon = "ÁùµÀÂÖ»Ø.dds";
            maxlevel = 20;
            maxlearn = 9;
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
            skill_class = 3;
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
            talent[0] = 699;
            talent[1] = 702;
            talent_size = 2;
            action[0] = "ÌìÒôËÂ_ÁùµÀÂÖ»Ø";
            action[1] = "ÌìÒôËÂ_ÁùµÀÂÖ»Ø";
            action[2] = "ÌìÒôËÂ_ÁùµÀÂÖ»Ø";
            action[3] = "ÌìÒôËÂ_ÁùµÀÂÖ»Ø";
            action[4] = "ÌìÒôËÂ_ÁùµÀÂÖ»Ø";
            action[5] = "ÌìÒôËÂ_ÁùµÀÂÖ»Ø";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_ÁùµÀÂÖ»Ø";
            action[16] = "ÌìÒôËÂ_Æï³Ë_ÁùµÀÂÖ»Ø";
            action[17] = "ÌìÒôËÂ_Æï³Ë_ÁùµÀÂÖ»Ø";
            action[18] = "ÌìÒôËÂ_Æï³Ë_ÁùµÀÂÖ»Ø";
            action[19] = "ÌìÒôËÂ_Æï³Ë_ÁùµÀÂÖ»Ø";
            action[20] = "ÌìÒôËÂ_Æï³Ë_ÁùµÀÂÖ»Ø";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "¼¼ÄÜ¹¥»÷1";
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
        virtual ~ Skill552Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 549 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (33);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (325 + 27 * skill->GetLevel () - 0.17 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 6000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 24000 - 3000 * skill->GetT0 () - 5 * 800;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 120, 122, 124, 125, 125, 125, 125, 125, 125 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (28);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10 + skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (325 + 27 * skill->GetLevel () - 0.175 * skill->GetLevel () * skill->GetLevel ()),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (0.089 * skill->GetLevel () * skill->GetLevel () + 17 * skill->GetLevel () + 742) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (596 + 81 * skill->GetLevel () -
                                                                              1.8 * skill->GetLevel () * skill->GetLevel ())), 8, 8);

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
            skill->GetVictim ()->SetProbability (1.0 * (6));
            skill->GetVictim ()->SetTime (2000 + 100);
            skill->GetVictim ()->SetRatio (0.1);
            skill->GetVictim ()->SetValue (0.1);
            skill->GetVictim ()->SetRandcurse (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () == 1 ? skill->GetPlayer ()->GetVar2 () : 0));
            skill->GetVictim ()->SetTime (2000);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetSleep (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (8));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetValue (0.080 * skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetHeal (1);
            return true;
        }
#endif
    };
}
#endif
