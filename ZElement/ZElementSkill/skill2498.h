#ifndef __CPPGEN_GNET_SKILL2498
#define __CPPGEN_GNET_SKILL2498
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2498:public Skill
    {
      public:
        enum
        { SKILL_ID = 2498 };
          Skill2498 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2498Stub:public SkillStub
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
                skill->SetCrit (0.02 * skill->GetT0 () + 0.02 * skill->GetJ1 ());
                skill->SetCrithurt (0.1 * skill->GetJ2 ());
                skill->SetRatio (1.05 + 0.03 * skill->GetJ0 ());
                skill->SetPlus (0.071 * skill->GetLevel () * skill->GetLevel () + 13 * skill->GetLevel () + 1027);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2498Stub ():SkillStub (2498)
        {
            occupation = 55;
            name = L"神引：阳关三叠";
            nativename = "神引：阳关三叠";
            icon = "神引：阳关三叠.dds";
            maxlevel = 6;
            maxlearn = 5;
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
#endif
        }
        virtual ~ Skill2498Stub ()
        {
        }
        int GetSkillElem (int index) const
        {
            static int array[3] = { 2916, 2917, 2918 };
            return array[index - 1];
        }
        int GetComboSkillCapacity (int level) const
        {
            static int array[5] = { 1, 2, 3, 4, 5 };
            return array[level - 1];
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 0 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 24 };
            return array[index];
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
            static int array[5] = { 135, 137, 139, 140, 141 };
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
                               INT (0.071 * skill->GetLevel () * skill->GetLevel () + 13 * skill->GetLevel () + 1027), 2 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
#endif
    };
}
#endif
