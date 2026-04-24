#ifndef __CPPGEN_GNET_SKILL3086
#define __CPPGEN_GNET_SKILL3086
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3086:public Skill
    {
      public:
        enum
        { SKILL_ID = 3086 };
          Skill3086 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3086Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 350;
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
                skill->GetPlayer ()->SetVar1 (1);
                skill->SetRatio (1.02 + 0.02 * skill->GetT0 () + 0.05 * skill->GetLevel () +
                                 norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) + 0.01 * skill->GetT1 ());
                skill->SetPlus (skill->GetT1 () > 0 ? (133 + 10 * skill->GetT1 ()) : 0);
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
                return 225;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (2);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetRes2 ());
                skill->SetRatio (1.02 + 0.02 * skill->GetT0 () + 0.05 * skill->GetLevel () +
                                 norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) + 0.01 * skill->GetT2 ());
                skill->SetPlus (skill->GetT2 () > 0 ? (324 + 21 * skill->GetT2 ()) : 0);
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
                return 500;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (3);
                skill->GetPlayer ()->SetVar3 (skill->GetPlayer ()->GetRes3 ());
                skill->SetRatio (1.02 + 0.02 * skill->GetT0 () + 0.05 * skill->GetLevel () +
                                 norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) + 0.01 * skill->GetT3 ());
                skill->SetPlus (skill->GetT3 () > 0 ? (1043 + 33 * skill->GetT3 ()) : 0);
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
                return 525;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (4);
                skill->GetPlayer ()->SetVar4 (skill->GetPlayer ()->GetRes1 ());
                skill->SetRatio (1.02 + 0.02 * skill->GetT0 () + 0.05 * skill->GetLevel () +
                                 norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) + 0.01 * skill->GetT4 ());
                skill->SetPlus (skill->GetT4 () > 0 ? (1543 + 32 * skill->GetT4 ()) : 0);
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill3086Stub ():SkillStub (3086)
        {
            occupation = 168;
            name = L"∞ÀÎ≈<…∑>";
            nativename = "∞ÀÎ≈<…∑>";
            icon = "ƒß°§∞ÀÎ≈.dds";
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
            skill_class = 5;
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
            talent[0] = 2084;
            talent[1] = 1847;
            talent[2] = 1856;
            talent[3] = 1865;
            talent[4] = 1874;
            talent_size = 5;
            action[0] = " ﬁ…Ò_∞ÀÎ≈";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = " ﬁ…Ò_∞ÀÎ≈";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = " ﬁ…Ò_∆Ô≥À_∞ÀÎ≈";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = " ﬁ…Ò_∆Ô≥À_∞ÀÎ≈";
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
            weapon_limit = 7;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
            statestub.push_back (new State3 ());
            statestub.push_back (new State4 ());
            statestub.push_back (new State5 ());
#endif
        }
        virtual ~ Skill3086Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (170 + 42 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1880 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 5 };
            return array[index];
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 400, 500, 500, 700, 1500, 2400, 3500, 5600, 8500, 8600, 19000, 26500, 49100, 57600, 85800, 140300, 241000, 410600, 614500,
 1344300 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 2, 4, 6, 8, 10, 12, 15, 18, 21, 24, 28, 33, 38, 43, 48, 54, 60, 66, 72, 78 };
            return array[level - 1];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1376 + 8 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 10000 - 400 - 1000 * skill->GetT0 () - 1000 * int (skill->GetT1 () / 8.99) - 1000 * int (skill->GetT2 () / 8.99) -
                1000 * int (skill->GetT3 () / 8.99) - 1000 * int (skill->GetT4 () / 8.99);
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
            return (float) (7);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (3);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               1376 + 8 * skill->GetLevel (),
                               170 + 42 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel (),
                               10,
                               INT (2 + INT (5 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 3 * (skill->GetLevel () - 15))),
                               2 * skill->GetLevel (), 2 + 0.5 * skill->GetLevel (), skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () == 1 ? (skill->GetT1 () > 0 ? 600 : 0) : 0));
            skill->GetVictim ()->SetTime (4000);
            skill->GetVictim ()->SetRatio (0.15 + 0.05 * skill->GetT1 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetSlow (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetVar1 () ==
                                                  2 ? INT (skill->GetPlayer ()->GetVar2 () * skill->GetT2 () / 9 + 2 * skill->GetLevel ()) : 0));
            skill->GetVictim ()->SetTime (4000);
            skill->GetVictim ()->SetRatio (0.2);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetWeak (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetVar1 () ==
                                                  3 ? INT (skill->GetPlayer ()->GetVar3 () * skill->GetT3 () / 9 + 2 * skill->GetLevel ()) : 0));
            skill->GetVictim ()->SetTime (4000);
            skill->GetVictim ()->SetWrap (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetVar1 () ==
                                                  4 ? INT (skill->GetPlayer ()->GetVar4 () * skill->GetT4 () / 9 + 2 * skill->GetLevel ()) : 0));
            skill->GetVictim ()->SetTime (4000);
            skill->GetVictim ()->SetDizzy (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () > 1 ? 10 * skill->GetT0 () : 0));
            skill->GetVictim ()->SetAmount (1);
            skill->GetVictim ()->SetCleardebuff (1);
            skill->GetVictim ()->SetTime (2050 + 500 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetValue (0.01 * skill->GetLevel () * skill->GetPlayer ()->GetRes5 ());
            skill->GetVictim ()->SetAddantisleep (1);
            return true;
        }
#endif
    };
}
#endif
