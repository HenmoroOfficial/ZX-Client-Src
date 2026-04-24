#ifndef __CPPGEN_GNET_SKILL2473
#define __CPPGEN_GNET_SKILL2473
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2473:public Skill
    {
      public:
        enum
        { SKILL_ID = 2473 };
          Skill2473 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2473Stub:public SkillStub
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
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrit (0.01 * skill->GetJ2 ());
                skill->SetRatio (1 + 0.02 * skill->GetT0 () + 0.02 * skill->GetJ0 ());
                skill->SetPlus (INT (324 + 21 * skill->GetLevel () + 35 * skill->GetJ1 ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2473Stub ():SkillStub (2473)
        {
            occupation = 52;
            name = L"神引：紫竹三调";
            nativename = "神引：紫竹三调";
            icon = "神引：紫竹三调.dds";
            maxlevel = 9;
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
            skillelemsfactor = 1;
            preskillnum = 1;
            talent[0] = 2629;
            talent[1] = 2637;
            talent_size = 2;
            action[0] = "天华_南薰";
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
            action[12] = "天华_南薰";
            action[13] = "";
            action[14] = "";
            action[15] = "天华_骑乘_南薰";
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
            action[27] = "天华_骑乘_南薰";
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
        virtual ~ Skill2473Stub ()
        {
        }
        int GetSkillElem (int index) const
        {
            static int array[3] = { 2899, 2900, 2901 };
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
            static int array[1] = { 2472 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (5.6 * skill->GetLevel () + 122);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1400;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 10000 - 2000 * skill->GetT1 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[5] = { 62, 64, 66, 68, 70 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (18);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (15);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, INT (5.6 * skill->GetLevel () + 122), 324 + 21 * skill->GetLevel ());

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
