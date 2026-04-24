#ifndef __CPPGEN_GNET_SKILL3499
#define __CPPGEN_GNET_SKILL3499
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3499:public Skill
    {
      public:
        enum
        { SKILL_ID = 3499 };
          Skill3499 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3499Stub:public SkillStub
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (2);
                skill->SetRatio (1 + 0.02 * skill->GetLevel ());
                skill->SetPlus (650 + 25 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel ());
                skill->SetCrit (0.01 * skill->GetLevel ());
                skill->SetCrithurt (0.03 * skill->GetLevel () +
                                    0.05 * (skill->GetT2 () >=
                                            skill->GetT3 ()? (skill->GetT2 () >=
                                                              skill->GetT4 ()? skill->GetT2 () : skill->GetT4 ()) : (skill->GetT3 () >=
                                                                                                                     skill->GetT4 ()? skill->
                                                                                                                     GetT3 () : skill->GetT4 ())));
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
                skill->GetPlayer ()->SetVar1 (3);
                skill->SetRatio (1 + 0.02 * skill->GetLevel ());
                skill->SetPlus (650 + 25 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel ());
                skill->SetCrit (0.01 * skill->GetLevel ());
                skill->SetCrithurt (0.03 * skill->GetLevel () +
                                    0.05 * (skill->GetT2 () >=
                                            skill->GetT3 ()? (skill->GetT2 () >=
                                                              skill->GetT4 ()? skill->GetT2 () : skill->GetT4 ()) : (skill->GetT3 () >=
                                                                                                                     skill->GetT4 ()? skill->
                                                                                                                     GetT3 () : skill->GetT4 ())));
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
                skill->GetPlayer ()->SetVar1 (4);
                skill->SetRatio (1 + 0.02 * skill->GetLevel ());
                skill->SetPlus (650 + 25 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel ());
                skill->SetCrit (0.01 * skill->GetLevel ());
                skill->SetCrithurt (0.03 * skill->GetLevel () +
                                    0.05 * (skill->GetT2 () >=
                                            skill->GetT3 ()? (skill->GetT2 () >=
                                                              skill->GetT4 ()? skill->GetT2 () : skill->GetT4 ()) : (skill->GetT3 () >=
                                                                                                                     skill->GetT4 ()? skill->
                                                                                                                     GetT3 () : skill->GetT4 ())));
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
                skill->GetPlayer ()->SetVar1 (1);
                skill->SetRatio (1 + 0.02 * skill->GetLevel ());
                skill->SetPlus (650 + 25 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel ());
                skill->SetCrit (0.01 * skill->GetLevel ());
                skill->SetCrithurt (0.03 * skill->GetLevel () +
                                    0.05 * (skill->GetT2 () >=
                                            skill->GetT3 ()? (skill->GetT2 () >=
                                                              skill->GetT4 ()? skill->GetT2 () : skill->GetT4 ()) : (skill->GetT3 () >=
                                                                                                                     skill->GetT4 ()? skill->
                                                                                                                     GetT3 () : skill->GetT4 ())));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State6:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (5);
                skill->SetRatio (1 + 0.02 * skill->GetLevel ());
                skill->SetPlus (650 + 25 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel ());
                skill->SetCrit (0.01 * skill->GetLevel ());
                skill->SetCrithurt (0.03 * skill->GetLevel () +
                                    0.05 * (skill->GetT2 () >=
                                            skill->GetT3 ()? (skill->GetT2 () >=
                                                              skill->GetT4 ()? skill->GetT2 () : skill->GetT4 ()) : (skill->GetT3 () >=
                                                                                                                     skill->GetT4 ()? skill->
                                                                                                                     GetT3 () : skill->GetT4 ())));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill3499Stub ():SkillStub (3499)
        {
            occupation = 100;
            name = L"Ç¬À¤µ´";
            nativename = "Ç¬À¤µ´";
            icon = "Ç¬À¤µ´.dds";
            maxlevel = 8;
            maxlearn = 5;
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
            skill_class = 10;
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
            talent[0] = 3507;
            talent[1] = 3498;
            talent[2] = 2947;
            talent[3] = 4112;
            talent[4] = 4530;
            talent_size = 5;
            action[0] = "Ì«ê»_À×öªÕðÅ­";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "Ì«ê»_À×öªÕðÅ­";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "Ì«ê»_Æï³Ë_À×öªÕðÅ­";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "Ì«ê»_Æï³Ë_À×öªÕðÅ­";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
            doenchant = false;
            dobless = true;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = 11;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
            statestub.push_back (new State3 ());
            statestub.push_back (new State4 ());
            statestub.push_back (new State5 ());
            statestub.push_back (new State6 ());
#endif
        }
        virtual ~ Skill3499Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3498 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (350 + 5 * skill->GetLevel () + 10 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 4000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[5] = { 137, 138, 139, 140, 141 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (10);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (3);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               350 + 5 * skill->GetLevel () + 10 * skill->GetLevel () * skill->GetLevel (),
                               650 + 25 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel (),
                               skill->GetLevel (),
                               2 * skill->GetLevel (),
                               3 * skill->GetLevel (),
                               skill->GetT2 () > 0 || skill->GetT3 () > 0 || skill->GetT4 () > 0 ? table.Find (34993) : table.Find (34994));

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetVar1 () ==
                                                  1 ? 20 + 3 * skill->GetT0 () + skill->GetT1 () + (skill->GetT2 () >=
                                                                                                    skill->GetT3 ()? (skill->GetT2 () >=
                                                                                                                      skill->GetT4 ()? skill->
                                                                                                                      GetT2 () : skill->
                                                                                                                      GetT4 ()) : (skill->GetT3 () >=
                                                                                                                                   skill->
                                                                                                                                   GetT4 ()? skill->
                                                                                                                                   GetT3 () : skill->
                                                                                                                                   GetT4 ())) +
                                                  skill->GetPlayer ()->GetSkilllevel (3584) / 2 +
                                                  norm (skill->GetPlayer ()->GetSkilllevel (3584) / 19.99) * 5 : 0));
            skill->GetVictim ()->SetTime (15000);
            skill->GetVictim ()->SetAmount (skill->GetLevel () + skill->GetPlayer ()->GetSkilllevel (3852));
            skill->GetVictim ()->SetValue (3707);
            skill->GetVictim ()->SetTriggerskill (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (10);
            skill->GetVictim ()->SetRatio (3707);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetSetcooldown (1);
            return true;
        }
#endif
    };
}
#endif
