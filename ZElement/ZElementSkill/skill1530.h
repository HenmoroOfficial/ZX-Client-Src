#ifndef __CPPGEN_GNET_SKILL1530
#define __CPPGEN_GNET_SKILL1530
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1530:public Skill
    {
      public:
        enum
        { SKILL_ID = 1530 };
          Skill1530 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1530Stub:public SkillStub
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1530Stub ():SkillStub (1530)
        {
            occupation = 148;
            name = L"天外天";
            nativename = "天外天";
            icon = "天外天.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 0;
            credittype = 9;
            clearmask = 1;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = -2;
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
            talent[0] = 1545;
            talent_size = 1;
            action[0] = "青云门_天外天";
            action[1] = "青云门_天外天";
            action[2] = "鬼王宗_天外天";
            action[3] = "合欢派_天外天";
            action[4] = "天音寺_天外天";
            action[5] = "鬼道_天外天";
            action[6] = "焚香谷_天外天";
            action[7] = "兽神_天外天";
            action[8] = "狐妖_天外天";
            action[9] = "轩辕_天外天";
            action[10] = "怀光_天外天";
            action[11] = "太昊_天外天";
            action[12] = "天华_天外天";
            action[13] = "灵夙_天外天";
            action[14] = "英招_天外天";
            action[15] = "青云门_骑乘_天外天";
            action[16] = "青云门_骑乘_天外天";
            action[17] = "鬼王宗_骑乘_天外天";
            action[18] = "合欢派_骑乘_天外天";
            action[19] = "天音寺_骑乘_天外天";
            action[20] = "鬼道_骑乘_天外天";
            action[21] = "焚香谷_骑乘_天外天";
            action[22] = "兽神_骑乘_天外天";
            action[23] = "狐妖_骑乘_天外天";
            action[24] = "轩辕_骑乘_天外天";
            action[25] = "怀光_骑乘_天外天";
            action[26] = "太昊_骑乘_天外天";
            action[27] = "天华_骑乘_天外天";
            action[28] = "灵夙_骑乘_天外天";
            action[29] = "英招_骑乘_天外天";
            action[30] = "0";
            rangetype = 4;
            doenchant = true;
            dobless = false;
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
#endif
        }
        virtual ~ Skill1530Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1529 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 6);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (2125 + 125 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 600000 - 18000 * skill->GetT0 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 90, 95, 100, 105, 110, 115, 120, 125, 125, 125 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (18);
        }
        float GetAngle (Skill * skill) const
        {
            return (float) (1 - 0.0111111 * (170));
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (18);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (8);
        }
        int GetCreditCost (int level) const
        {
            static int array[10] = { 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 38000, 45000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               2125 + 125 * skill->GetLevel (),
                               18 + 2 * skill->GetLevel (), 18, 30, 5 * skill->GetLevel (), 50, 6 + 2 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount (15 * 0.05 * skill->GetLevel () * skill->GetPlayer ()->GetMaxmp ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetMpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (50));
            skill->GetVictim ()->SetTime (6000 + 2000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetDiet (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (18 + 2 * skill->GetLevel ());
        }
#endif
    };
}
#endif
