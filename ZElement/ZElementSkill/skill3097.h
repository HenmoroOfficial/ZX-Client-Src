#ifndef __CPPGEN_GNET_SKILL3097
#define __CPPGEN_GNET_SKILL3097
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3097:public Skill
    {
      public:
        enum
        { SKILL_ID = 3097 };
          Skill3097 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3097Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
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
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrit (0.02 * skill->GetT0 () + 0.02 * skill->GetJ1 () + 0.003 * skill->GetLevel () * skill->GetJ1 ());
                skill->SetCrithurt (0.1 * skill->GetJ2 () + skill->GetPlayer ()->GetRes4 () * 0.00002 * skill->GetLevel ());
                skill->SetRatio (1.05 + 0.05 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) +
                                 0.03 * skill->GetJ0 ());
                skill->SetPlus (0.071 * skill->GetLevel () * skill->GetLevel () + 13 * skill->GetLevel () + 1027 +
                                skill->GetPlayer ()->GetRes3 () * (2 + 0.2 * skill->GetLevel ()) * skill->GetJ0 ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3097Stub ():SkillStub (3097)
        {
            occupation = 168;
            name = L"神引：阳关三叠<煞>";
            nativename = "神引：阳关三叠<煞>";
            icon = "魔·神引：阳关三叠.dds";
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
            skillelemsfactor = 1.3;
            preskillnum = 1;
            talent[0] = 2610;
            talent[1] = 2605;
            talent[2] = 2606;
            talent_size = 3;
            action[0] = "天华_冠古韵磬";
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
            action[12] = "天华_冠古韵磬";
            action[13] = "";
            action[14] = "";
            action[15] = "天华_骑乘_冠古韵磬";
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
            action[27] = "天华_骑乘_冠古韵磬";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
            doenchant = true;
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
#endif
        }
        virtual ~ Skill3097Stub ()
        {
        }
        int GetSkillElem (int index) const
        {
            static int array[3] = { 2916, 2917, 2918 };
            return array[index - 1];
        }
        int GetComboSkillCapacity (int level) const
        {
            static int array[20] = { 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6 };
            return array[level - 1];
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (170 + 42 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 2498 };
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
            return (float) (25 + 1 * skill->GetT1 ());
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1.5 * skill->GetLevel () * skill->GetLevel () - 3.8 * skill->GetLevel () + 897);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 10000 - 1000 * skill->GetT2 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (20);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (20 + 1 * skill->GetT1 ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (15 + 1 * skill->GetT1 ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (1.5 * skill->GetLevel () * skill->GetLevel () - 3.8 * skill->GetLevel () + 897),
                               170 + 42 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel (),
                               10,
                               INT (1 + INT (skill->GetLevel () / 3.9)),
                               INT (0.071 * skill->GetLevel () * skill->GetLevel () + 13 * skill->GetLevel () + 1027),
                               INT (5 + INT (5 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 3 * (skill->GetLevel () - 15))),
                               0.4 * skill->GetLevel (), 0.2 * skill->GetLevel (), 2 + 0.2 * skill->GetLevel (), 20 + 2 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (20 + 2 * skill->GetLevel () + norm (skill->GetLevel () / 19.9) * 10));
            skill->GetVictim ()->SetRatio (7);
            skill->GetVictim ()->SetAmount (65);
            skill->GetVictim ()->SetValue (67);
            skill->GetVictim ()->SetDispel (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetRatio (9);
            skill->GetVictim ()->SetAmount (74);
            skill->GetVictim ()->SetValue (67);
            skill->GetVictim ()->SetDispel (1);
            return true;
        }
#endif
    };
}
#endif
