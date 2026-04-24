#ifndef __CPPGEN_GNET_SKILL3073
#define __CPPGEN_GNET_SKILL3073
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3073:public Skill
    {
      public:
        enum
        { SKILL_ID = 3073 };
          Skill3073 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3073Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes1 ());
                skill->SetRatio (1 + 0.05 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) +
                                 0.07 * skill->GetT2 ());
                skill->SetPlus ((835 + 22 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel ()) *
                                (1 + INT (zrand (99 + 7 * skill->GetT3 ()) / 100)));
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes1 ());
                skill->SetRatio (1 + 0.05 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) +
                                 0.07 * skill->GetT2 ());
                skill->SetPlus ((835 + 22 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel ()) *
                                (1 + INT (zrand (99 + 7 * skill->GetT3 ()) / 100)));
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes1 ());
                skill->SetRatio (1 + 0.05 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) +
                                 0.07 * skill->GetT2 ());
                skill->SetPlus ((835 + 22 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel ()) *
                                (1 + INT (zrand (99 + 7 * skill->GetT3 ()) / 100)));
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes1 ());
                skill->SetRatio (1 + 0.05 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) +
                                 0.07 * skill->GetT2 ());
                skill->SetPlus ((835 + 22 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel ()) *
                                (1 + INT (zrand (99 + 7 * skill->GetT3 ()) / 100)));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill3073Stub ():SkillStub (3073)
        {
            occupation = 168;
            name = L"À×Òô·¨ÚÍ<É·>";
            nativename = "À×Òô·¨ÚÍ<É·>";
            icon = "Ä§¡¤À×Òô·¨ÚÍ.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
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
            talent[0] = 684;
            talent[1] = 698;
            talent[2] = 702;
            talent[3] = 708;
            talent_size = 4;
            action[0] = "ÌìÒôËÂ_À×Òô·¨ÚÍ";
            action[1] = "ÌìÒôËÂ_À×Òô·¨ÚÍ";
            action[2] = "ÌìÒôËÂ_À×Òô·¨ÚÍ";
            action[3] = "ÌìÒôËÂ_À×Òô·¨ÚÍ";
            action[4] = "ÌìÒôËÂ_À×Òô·¨ÚÍ";
            action[5] = "ÌìÒôËÂ_À×Òô·¨ÚÍ";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_À×Òô·¨ÚÍ";
            action[16] = "ÌìÒôËÂ_Æï³Ë_À×Òô·¨ÚÍ";
            action[17] = "ÌìÒôËÂ_Æï³Ë_À×Òô·¨ÚÍ";
            action[18] = "ÌìÒôËÂ_Æï³Ë_À×Òô·¨ÚÍ";
            action[19] = "ÌìÒôËÂ_Æï³Ë_À×Òô·¨ÚÍ";
            action[20] = "ÌìÒôËÂ_Æï³Ë_À×Òô·¨ÚÍ";
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
#endif
        }
        virtual ~ Skill3073Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (200 + 34 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 787 };
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
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (19 + 0.4 * skill->GetLevel ());
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (2.6 * skill->GetLevel () * skill->GetLevel () - 3.3 * skill->GetLevel () + 1820);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 16000 - 3200 * skill->GetT0 ();
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
        float GetPraydistance (Skill * skill) const
        {
            return (float) (16 + 0.4 * skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10.7 + 0.4 * skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (2.6 * skill->GetLevel () * skill->GetLevel () - 3.3 * skill->GetLevel () + 1820),
                               200 + 34 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel (),
                               16,
                               10.7,
                               INT (5 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 3 * (skill->GetLevel () - 15)),
                               INT (835 + 22 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel ()),
                               4 * skill->GetLevel (),
                               INT ((500 + 11 * skill->GetLevel () + 1.5 * skill->GetLevel () * skill->GetLevel ()) * skill->GetLevel ()),
                               60 + 0.5 * skill->GetLevel (), 6, 32 * skill->GetLevel (), 0.4 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (100 * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetTime (4000 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount ((500 + 11 * skill->GetLevel () + 1.5 * skill->GetLevel () * skill->GetLevel ()) * skill->GetLevel ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (6 * skill->GetT1 ()));
            skill->GetVictim ()->SetTime (4050 * skill->GetT1 ());
            skill->GetVictim ()->SetRatio (0.03 * skill->GetT1 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecdefence (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetVar1 () * (0.6 + 0.005 * skill->GetLevel ()) +
                                                  norm (skill->GetLevel () / 19.9) * 30));
            skill->GetVictim ()->SetTime (6000 + 100);
            skill->GetVictim ()->SetDizzy (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetValue (32 * skill->GetLevel () * (1 + skill->GetT3 ()));
            skill->GetVictim ()->SetJuqi (1);
            return true;
        }
#endif
    };
}
#endif
