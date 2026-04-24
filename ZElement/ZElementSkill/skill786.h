#ifndef __CPPGEN_GNET_SKILL786
#define __CPPGEN_GNET_SKILL786
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill786:public Skill
    {
      public:
        enum
        { SKILL_ID = 786 };
          Skill786 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill786Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (1);
                skill->SetRatio (1 + 0.04 * skill->GetLevel () +
                                 (0.05 + 0.05 * int (skill->GetPlayer ()->GetSkilllevel (234) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (242) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (543) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (785) / 3.9)) *skill->GetT3 () + 0.02 * (skill->GetT4 () >=
                                                                                                                          skill->GetT5 ()? (skill->
                                                                                                                                            GetT4 ()
                                                                                                                                            >=
                                                                                                                                            skill->
                                                                                                                                            GetT6 ()?
                                                                                                                                            skill->
                                                                                                                                            GetT4 () :
                                                                                                                                            skill->
                                                                                                                                            GetT6 ())
                                                                                                                          : (skill->GetT5 () >=
                                                                                                                             skill->GetT6 ()? skill->
                                                                                                                             GetT5 () : skill->
                                                                                                                             GetT6 ())));
                skill->SetPlus ((800 + 50 * skill->GetLevel () + 15 * skill->GetLevel () * skill->GetLevel ()) *
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
                skill->GetPlayer ()->SetVar1 (2);
                skill->SetRatio (1 + 0.04 * skill->GetLevel () +
                                 (0.05 + 0.05 * int (skill->GetPlayer ()->GetSkilllevel (234) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (242) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (543) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (785) / 3.9)) *skill->GetT3 () + 0.02 * (skill->GetT4 () >=
                                                                                                                          skill->GetT5 ()? (skill->
                                                                                                                                            GetT4 ()
                                                                                                                                            >=
                                                                                                                                            skill->
                                                                                                                                            GetT6 ()?
                                                                                                                                            skill->
                                                                                                                                            GetT4 () :
                                                                                                                                            skill->
                                                                                                                                            GetT6 ())
                                                                                                                          : (skill->GetT5 () >=
                                                                                                                             skill->GetT6 ()? skill->
                                                                                                                             GetT5 () : skill->
                                                                                                                             GetT6 ())));
                skill->SetPlus ((800 + 50 * skill->GetLevel () + 15 * skill->GetLevel () * skill->GetLevel ()) *
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
                skill->GetPlayer ()->SetVar1 (2);
                skill->SetRatio (1 + 0.04 * skill->GetLevel () +
                                 (0.05 + 0.05 * int (skill->GetPlayer ()->GetSkilllevel (234) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (242) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (543) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (785) / 3.9)) *skill->GetT3 () + 0.02 * (skill->GetT4 () >=
                                                                                                                          skill->GetT5 ()? (skill->
                                                                                                                                            GetT4 ()
                                                                                                                                            >=
                                                                                                                                            skill->
                                                                                                                                            GetT6 ()?
                                                                                                                                            skill->
                                                                                                                                            GetT4 () :
                                                                                                                                            skill->
                                                                                                                                            GetT6 ())
                                                                                                                          : (skill->GetT5 () >=
                                                                                                                             skill->GetT6 ()? skill->
                                                                                                                             GetT5 () : skill->
                                                                                                                             GetT6 ())));
                skill->SetPlus ((800 + 50 * skill->GetLevel () + 15 * skill->GetLevel () * skill->GetLevel ()) *
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
                skill->GetPlayer ()->SetVar1 (2);
                skill->SetRatio (1 + 0.04 * skill->GetLevel () +
                                 (0.05 + 0.05 * int (skill->GetPlayer ()->GetSkilllevel (234) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (242) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (543) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (785) / 3.9)) *skill->GetT3 () + 0.02 * (skill->GetT4 () >=
                                                                                                                          skill->GetT5 ()? (skill->
                                                                                                                                            GetT4 ()
                                                                                                                                            >=
                                                                                                                                            skill->
                                                                                                                                            GetT6 ()?
                                                                                                                                            skill->
                                                                                                                                            GetT4 () :
                                                                                                                                            skill->
                                                                                                                                            GetT6 ())
                                                                                                                          : (skill->GetT5 () >=
                                                                                                                             skill->GetT6 ()? skill->
                                                                                                                             GetT5 () : skill->
                                                                                                                             GetT6 ())));
                skill->SetPlus ((800 + 50 * skill->GetLevel () + 15 * skill->GetLevel () * skill->GetLevel ()) *
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
                skill->GetPlayer ()->SetVar1 (2);
                skill->SetRatio (1 + 0.04 * skill->GetLevel () +
                                 (0.05 + 0.05 * int (skill->GetPlayer ()->GetSkilllevel (234) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (242) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (543) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (785) / 3.9)) *skill->GetT3 () + 0.02 * (skill->GetT4 () >=
                                                                                                                          skill->GetT5 ()? (skill->
                                                                                                                                            GetT4 ()
                                                                                                                                            >=
                                                                                                                                            skill->
                                                                                                                                            GetT6 ()?
                                                                                                                                            skill->
                                                                                                                                            GetT4 () :
                                                                                                                                            skill->
                                                                                                                                            GetT6 ())
                                                                                                                          : (skill->GetT5 () >=
                                                                                                                             skill->GetT6 ()? skill->
                                                                                                                             GetT5 () : skill->
                                                                                                                             GetT6 ())));
                skill->SetPlus ((800 + 50 * skill->GetLevel () + 15 * skill->GetLevel () * skill->GetLevel ()) *
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
                skill->GetPlayer ()->SetVar1 (3);
                skill->SetRatio (1 + 0.04 * skill->GetLevel () +
                                 (0.05 + 0.05 * int (skill->GetPlayer ()->GetSkilllevel (234) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (242) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (543) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (785) / 3.9)) *skill->GetT3 () + 0.02 * (skill->GetT4 () >=
                                                                                                                          skill->GetT5 ()? (skill->
                                                                                                                                            GetT4 ()
                                                                                                                                            >=
                                                                                                                                            skill->
                                                                                                                                            GetT6 ()?
                                                                                                                                            skill->
                                                                                                                                            GetT4 () :
                                                                                                                                            skill->
                                                                                                                                            GetT6 ())
                                                                                                                          : (skill->GetT5 () >=
                                                                                                                             skill->GetT6 ()? skill->
                                                                                                                             GetT5 () : skill->
                                                                                                                             GetT6 ())));
                skill->SetPlus ((INT (zrand (99 + 5 * skill->GetLevel ()) / 100) + 1) *
                                (800 + 50 * skill->GetLevel () +
                                 15 * skill->GetLevel () * skill->GetLevel ()) * ((100 +
                                                                                   (zrand (10 + 15 * skill->GetT1 ()) - 10) * norm (skill->GetT1 () +
                                                                                                                                    0.1)) / 100.0));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill786Stub ():SkillStub (786)
        {
            occupation = 20;
            name = L"Éñ½£ÓùÀ×Õæ¾÷";
            nativename = "Éñ½£ÓùÀ×Õæ¾÷";
            icon = "Éñ½£ÓùÀ×Õæ¾÷.dds";
            maxlevel = 3;
            maxlearn = 2;
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
            talent[2] = 611;
            talent[3] = 617;
            talent[4] = 2947;
            talent[5] = 4112;
            talent[6] = 4530;
            talent_size = 7;
            action[0] = "ÇàÔÆÃÅ_Éñ½£ÓùÀ×Õæ¾÷";
            action[1] = "ÇàÔÆÃÅ_Éñ½£ÓùÀ×Õæ¾÷";
            action[2] = "ÇàÔÆÃÅ_Éñ½£ÓùÀ×Õæ¾÷";
            action[3] = "ÇàÔÆÃÅ_Éñ½£ÓùÀ×Õæ¾÷";
            action[4] = "ÇàÔÆÃÅ_Éñ½£ÓùÀ×Õæ¾÷";
            action[5] = "ÇàÔÆÃÅ_Éñ½£ÓùÀ×Õæ¾÷";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_Éñ½£ÓùÀ×Õæ¾÷";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_Éñ½£ÓùÀ×Õæ¾÷";
            action[17] = "ÇàÔÆÃÅ_Æï³Ë_Éñ½£ÓùÀ×Õæ¾÷";
            action[18] = "ÇàÔÆÃÅ_Æï³Ë_Éñ½£ÓùÀ×Õæ¾÷";
            action[19] = "ÇàÔÆÃÅ_Æï³Ë_Éñ½£ÓùÀ×Õæ¾÷";
            action[20] = "ÇàÔÆÃÅ_Æï³Ë_Éñ½£ÓùÀ×Õæ¾÷";
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
        virtual ~ Skill786Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 783 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
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
            static int array[2] = { 139, 141 };
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
                               40,
                               25,
                               4 * skill->GetLevel (),
                               INT (800 + 50 * skill->GetLevel () + 15 * skill->GetLevel () * skill->GetLevel ()),
                               4 * skill->GetLevel (),
                               4 * skill->GetLevel (),
                               4 * skill->GetLevel (),
                               6,
                               6,
                               180 + 17 * skill->GetLevel (),
                               skill->GetT4 () > 0 || skill->GetPlayer ()->GetSkilllevel (4530) > 0
                               || skill->GetPlayer ()->GetSkilllevel (4112) > 0 ? table.Find (7863) : table.Find (7864));

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
            skill->GetVictim ()->SetProbability (1.0 * (INT (zrand (100) / (100 - 5 * skill->GetLevel ())) * 600));
            skill->GetVictim ()->SetTime (6000 + 100);
            skill->GetVictim ()->SetSilent (1);
            skill->GetVictim ()->SetProbability (1.0 * (180 + 17 * skill->GetLevel () + 6 * skill->GetT0 () * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (6000 + 100 + 500 * skill->GetT0 ());
            skill->GetVictim ()->SetWrap (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetRatio (220 + INT (zrand (99 + 5 * skill->GetLevel () + 5 * skill->GetT2 ()) / 100) * 566);
            skill->GetVictim ()->SetAmount (220);
            skill->GetVictim ()->SetValue (220);
            skill->GetVictim ()->SetClearcooldown (1);
            skill->GetVictim ()->SetTime (norm (skill->GetLevel () / 3) *
                                          (skill->GetPlayer ()->GetVar1 () == 1
                                           || skill->GetPlayer ()->GetVar1 () == 3 ? 1 : 0) * 1000 * (skill->GetT4 () >=
                                                                                                      skill->GetT5 ()? (skill->GetT4 () >=
                                                                                                                        skill->GetPlayer ()->
                                                                                                                        GetSkilllevel (4530) ? skill->
                                                                                                                        GetT4 () : skill->
                                                                                                                        GetPlayer ()->
                                                                                                                        GetSkilllevel (4530))
                                                                                                      : (skill->GetT5 () >=
                                                                                                         skill->GetPlayer ()->
                                                                                                         GetSkilllevel (4530) ? skill->
                                                                                                         GetT5 () : skill->GetPlayer ()->
                                                                                                         GetSkilllevel (4530))) *
                                          norm ((skill->GetT4 () >=
                                                 skill->GetT5 ()? (skill->GetT4 () >=
                                                                   skill->GetPlayer ()->GetSkilllevel (4530) ? skill->GetT4 () : skill->GetPlayer ()->
                                                                   GetSkilllevel (4530)) : (skill->GetT5 () >=
                                                                                            skill->GetPlayer ()->GetSkilllevel (4530) ? skill->
                                                                                            GetT5 () : skill->GetPlayer ()->GetSkilllevel (4530))) *
                                                8 / (zrand (100) + 0.01)));
            skill->GetVictim ()->
                SetValue ((skill->GetT4 () >=
                           skill->GetT5 ()? (skill->GetT4 () >=
                                             skill->GetPlayer ()->GetSkilllevel (4530) ? skill->GetT4 () : skill->GetPlayer ()->
                                             GetSkilllevel (4530)) : (skill->GetT5 () >=
                                                                      skill->GetPlayer ()->GetSkilllevel (4530) ? skill->GetT5 () : skill->
                                                                      GetPlayer ()->GetSkilllevel (4530))) + 5);
            skill->GetVictim ()->SetBlessed (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (40);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
