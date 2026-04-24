#ifndef __CPPGEN_GNET_SKILL778
#define __CPPGEN_GNET_SKILL778
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill778:public Skill
    {
      public:
        enum
        { SKILL_ID = 778 };
          Skill778 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill778Stub:public SkillStub
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
                skill->SetRatio (1.22 + 0.05 * skill->GetT1 () +
                                 0.04 * (skill->GetT4 () >=
                                         skill->GetT5 ()? (skill->GetT4 () >=
                                                           skill->GetPlayer ()->GetSkilllevel (4530) ? skill->GetT4 () : skill->GetPlayer ()->
                                                           GetSkilllevel (4530)) : (skill->GetT5 () >=
                                                                                    skill->GetPlayer ()->GetSkilllevel (4530) ? skill->
                                                                                    GetT5 () : skill->GetPlayer ()->GetSkilllevel (4530))));
                skill->SetPlus ((720 + 30 * skill->GetLevel () + 7 * skill->GetLevel () * skill->GetLevel ()) * (1 + 0.07 * skill->GetT3 ()));
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
                skill->SetRatio (1.22 +
                                 0.04 * (skill->GetT4 () >=
                                         skill->GetT5 ()? (skill->GetT4 () >=
                                                           skill->GetPlayer ()->GetSkilllevel (4530) ? skill->GetT4 () : skill->GetPlayer ()->
                                                           GetSkilllevel (4530)) : (skill->GetT5 () >=
                                                                                    skill->GetPlayer ()->GetSkilllevel (4530) ? skill->
                                                                                    GetT5 () : skill->GetPlayer ()->GetSkilllevel (4530))));
                skill->SetPlus ((720 + 30 * skill->GetLevel () + 7 * skill->GetLevel () * skill->GetLevel ()) * (1 + 0.07 * skill->GetT3 ()));
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
                skill->SetRatio (1.22 +
                                 0.04 * (skill->GetT4 () >=
                                         skill->GetT5 ()? (skill->GetT4 () >=
                                                           skill->GetPlayer ()->GetSkilllevel (4530) ? skill->GetT4 () : skill->GetPlayer ()->
                                                           GetSkilllevel (4530)) : (skill->GetT5 () >=
                                                                                    skill->GetPlayer ()->GetSkilllevel (4530) ? skill->
                                                                                    GetT5 () : skill->GetPlayer ()->GetSkilllevel (4530))));
                skill->SetPlus ((720 + 30 * skill->GetLevel () + 7 * skill->GetLevel () * skill->GetLevel ()) * (1 + 0.07 * skill->GetT3 ()));
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
                skill->SetRatio (1.22 +
                                 0.04 * (skill->GetT4 () >=
                                         skill->GetT5 ()? (skill->GetT4 () >=
                                                           skill->GetPlayer ()->GetSkilllevel (4530) ? skill->GetT4 () : skill->GetPlayer ()->
                                                           GetSkilllevel (4530)) : (skill->GetT5 () >=
                                                                                    skill->GetPlayer ()->GetSkilllevel (4530) ? skill->
                                                                                    GetT5 () : skill->GetPlayer ()->GetSkilllevel (4530))));
                skill->SetPlus ((720 + 30 * skill->GetLevel () + 7 * skill->GetLevel () * skill->GetLevel ()) * (1 + 0.07 * skill->GetT3 ()));
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
                skill->SetRatio (1.22 +
                                 0.04 * (skill->GetT4 () >=
                                         skill->GetT5 ()? (skill->GetT4 () >=
                                                           skill->GetPlayer ()->GetSkilllevel (4530) ? skill->GetT4 () : skill->GetPlayer ()->
                                                           GetSkilllevel (4530)) : (skill->GetT5 () >=
                                                                                    skill->GetPlayer ()->GetSkilllevel (4530) ? skill->
                                                                                    GetT5 () : skill->GetPlayer ()->GetSkilllevel (4530))));
                skill->SetPlus ((720 + 30 * skill->GetLevel () + 7 * skill->GetLevel () * skill->GetLevel ()) * (1 + 0.07 * skill->GetT3 ()));
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
                skill->SetRatio (1.22 + 0.15 +
                                 0.04 * (skill->GetT4 () >=
                                         skill->GetT5 ()? (skill->GetT4 () >=
                                                           skill->GetPlayer ()->GetSkilllevel (4530) ? skill->GetT4 () : skill->GetPlayer ()->
                                                           GetSkilllevel (4530)) : (skill->GetT5 () >=
                                                                                    skill->GetPlayer ()->GetSkilllevel (4530) ? skill->
                                                                                    GetT5 () : skill->GetPlayer ()->GetSkilllevel (4530))));
                skill->SetPlus ((720 + 30 * skill->GetLevel () + 7 * skill->GetLevel () * skill->GetLevel ()) * (1 + 0.07 * skill->GetT3 ()));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill778Stub ():SkillStub (778)
        {
            occupation = 14;
            name = L"Î´ÃûÕ¶";
            nativename = "Î´ÃûÕ¶";
            icon = "Î´ÃûÕ¶.dds";
            maxlevel = 3;
            maxlearn = 2;
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
            skill_class = 0;
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
            talent[0] = 624;
            talent[1] = 629;
            talent[2] = 650;
            talent[3] = 661;
            talent[4] = 2947;
            talent[5] = 4112;
            talent[6] = 4530;
            talent_size = 7;
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
        virtual ~ Skill778Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 775 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12 +
                            0.6 * (skill->GetT4 () >=
                                   skill->GetT5 ()? (skill->GetT4 () >=
                                                     skill->GetPlayer ()->GetSkilllevel (4530) ? skill->GetT4 () : skill->GetPlayer ()->
                                                     GetSkilllevel (4530)) : (skill->GetT5 () >=
                                                                              skill->GetPlayer ()->GetSkilllevel (4530) ? skill->GetT5 () : skill->
                                                                              GetPlayer ()->GetSkilllevel (4530))));
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (skill->GetLevel () * skill->GetLevel () + 8 * skill->GetLevel () + 580);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 15000 - 2000 * skill->GetT1 () - 2250 * skill->GetT3 ();
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
        float GetPraydistance (Skill * skill) const
        {
            return (float) (9 + 3 * (norm (skill->GetT4 () / 5) + norm (skill->GetT5 () / 5) + norm (skill->GetPlayer ()->GetSkilllevel (4530) / 5)));
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7 +
                            3 * (norm (skill->GetT4 () / 5) + norm (skill->GetT5 () / 5) + norm (skill->GetPlayer ()->GetSkilllevel (4530) / 5)));
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (skill->GetLevel () * skill->GetLevel () + 8 * skill->GetLevel () + 580),
                               22,
                               INT (720 + 30 * skill->GetLevel () + 7 * skill->GetLevel () * skill->GetLevel ()),
                               8,
                               33,
                               30,
                               30,
                               6,
                               210 + 15 * skill->GetLevel (),
                               6,
                               33,
                               10 * skill->GetLevel (),
                               15,
                               skill->GetT4 () > 0 || skill->GetPlayer ()->GetSkilllevel (4530) > 0
                               || skill->GetPlayer ()->GetSkilllevel (4112) > 0 ? table.Find (7783) : table.Find (7784));

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
            skill->GetVictim ()->SetProbability (1.0 * (210 + 15 * skill->GetLevel () + 9 * skill->GetLevel () * skill->GetT0 ()));
            skill->GetVictim ()->SetTime (6000 + 100);
            skill->GetVictim ()->SetRatio (0.3 + 0.05 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetWeak (1);
            skill->GetVictim ()->SetProbability (1.0 * (33 * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0.1 * skill->GetLevel () * skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetMpleak (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (33 + 15 * skill->GetT2 ()));
            skill->GetVictim ()->SetTime (8000 + 100);
            skill->GetVictim ()->SetRatio (0.3 +
                                           0.03 * (skill->GetT4 () >=
                                                   skill->GetPlayer ()->GetSkilllevel (4112) ? (skill->GetT4 () >=
                                                                                                skill->GetPlayer ()->GetSkilllevel (4530) ? skill->
                                                                                                GetT4 () : skill->GetPlayer ()->
                                                                                                GetSkilllevel (4530)) : (skill->GetPlayer ()->
                                                                                                                         GetSkilllevel (4112) >=
                                                                                                                         skill->GetPlayer ()->
                                                                                                                         GetSkilllevel (4530) ?
                                                                                                                         skill->GetPlayer ()->
                                                                                                                         GetSkilllevel (4112) :
                                                                                                                         skill->GetPlayer ()->
                                                                                                                         GetSkilllevel (4530))));
            skill->GetVictim ()->SetDecdamage (1);
            return true;
        }
#endif
    };
}
#endif
