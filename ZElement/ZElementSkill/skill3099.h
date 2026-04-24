#ifndef __CPPGEN_GNET_SKILL3099
#define __CPPGEN_GNET_SKILL3099
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3099:public Skill
    {
      public:
        enum
        { SKILL_ID = 3099 };
          Skill3099 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3099Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1200;
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
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrit (0.01 * skill->GetJ1 () + 0.003 * skill->GetLevel () * skill->GetJ1 () + 0.02 * skill->GetT0 ());
                skill->SetRatio (1 + (0.1 + 0.02 * skill->GetLevel ()) * skill->GetJ2 () + 0.01 * skill->GetJ2 ());
                skill->SetPlus (0.091 * skill->GetLevel () * skill->GetLevel () + 14 * skill->GetLevel () +
                                skill->GetPlayer ()->GetRes3 () * skill->GetJ0 () * (3 + 0.2 * skill->GetLevel ()) + 543 + 90 * skill->GetJ0 ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State3:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrit (0.01 * skill->GetJ1 () + 0.003 * skill->GetLevel () * skill->GetJ1 () + 0.02 * skill->GetT0 ());
                skill->SetRatio (1 + (0.1 + 0.02 * skill->GetLevel ()) * skill->GetJ2 () + 0.01 * skill->GetJ2 ());
                skill->SetPlus (0.091 * skill->GetLevel () * skill->GetLevel () + 14 * skill->GetLevel () +
                                skill->GetPlayer ()->GetRes3 () * skill->GetJ0 () * (3 + 0.2 * skill->GetLevel ()) + 543 + 90 * skill->GetJ0 ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State4:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrit (0.01 * skill->GetJ1 () + 0.003 * skill->GetLevel () * skill->GetJ1 () + 0.02 * skill->GetT0 ());
                skill->SetRatio (1 + (0.1 + 0.02 * skill->GetLevel ()) * skill->GetJ2 () + 0.01 * skill->GetJ2 ());
                skill->SetPlus (0.091 * skill->GetLevel () * skill->GetLevel () + 14 * skill->GetLevel () +
                                skill->GetPlayer ()->GetRes3 () * skill->GetJ0 () * (3 + 0.2 * skill->GetLevel ()) + 543 + 90 * skill->GetJ0 ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill3099Stub ():SkillStub (3099)
        {
            occupation = 168;
            name = L"梅花三弄<煞>";
            nativename = "梅花三弄<煞>";
            icon = "魔·神引：梅花三弄.dds";
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
            comboskill = 1;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 8;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 3;
            skillelemsfactor = 3;
            preskillnum = 1;
            talent[0] = 2617;
            talent[1] = 2605;
            talent[2] = 2606;
            talent_size = 3;
            action[0] = "天华_九霄环佩";
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
            action[11] = "0";
            action[12] = "天华_九霄环佩";
            action[13] = "";
            action[14] = "";
            action[15] = "天华_骑乘_九霄环佩";
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
            action[26] = "0";
            action[27] = "天华_骑乘_九霄环佩";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 2;
            doenchant = false;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = 12;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
            statestub.push_back (new State3 ());
            statestub.push_back (new State4 ());
#endif
        }
        virtual ~ Skill3099Stub ()
        {
        }
        int GetSkillElem (int index) const
        {
            static int array[3] = { 2919, 2920, 2921 };
            return array[index - 1];
        }
        int GetComboSkillCapacity (int level) const
        {
            static int array[20] = { 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6 };
            return array[level - 1];
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (200 + 34 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 2500 };
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
                { 200, 500, 500, 600, 900, 1900, 3200, 4500, 8600, 10800, 20700, 32200, 51000, 65200, 84800, 159900, 260100, 427500, 743600,
 1450600 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 1, 3, 5, 7, 9, 11, 14, 17, 21, 25, 29, 34, 39, 44, 49, 55, 61, 67, 73, 79 };
            return array[level - 1];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (30 + 2 * skill->GetT1 ());
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (25 * skill->GetLevel () + 1650);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 10000 - 2000 * skill->GetT2 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (20 + 2 * skill->GetT1 ());
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (17 + 2 * skill->GetT1 ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (15 + 2 * skill->GetT1 ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               25 * skill->GetLevel () + 1650,
                               200 + 34 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel (),
                               10,
                               5 + skill->GetLevel (),
                               INT (1 + INT (skill->GetLevel () / 3.9)),
                               INT (0.091 * skill->GetLevel () * skill->GetLevel () + 14 * skill->GetLevel () + 543),
                               0.3 * skill->GetLevel (), 10 + 2 * skill->GetLevel (), 3 + 0.2 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        int GetCoverage (Skill * skill) const
        {
            return (int) (5 + skill->GetLevel ());
        }
#endif
    };
}
#endif
