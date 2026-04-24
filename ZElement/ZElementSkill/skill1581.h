#ifndef __CPPGEN_GNET_SKILL1581
#define __CPPGEN_GNET_SKILL1581
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1581:public Skill
    {
      public:
        enum
        { SKILL_ID = 1581 };
          Skill1581 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1581Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 5000;
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
                return 1000;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetHp (1);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1581Stub ():SkillStub (1581)
        {
            occupation = 150;
            name = L"金光舍利";
            nativename = "金光舍利";
            icon = "金光舍利.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 2;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 11;
            clearmask = 4;
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
            action[0] = "青云门_金光舍利";
            action[1] = "青云门_金光舍利";
            action[2] = "鬼王宗_金光舍利";
            action[3] = "合欢派_金光舍利";
            action[4] = "天音寺_金光舍利";
            action[5] = "鬼道_金光舍利";
            action[6] = "焚香谷_金光舍利";
            action[7] = "兽神_金光舍利";
            action[8] = "狐妖_金光舍利";
            action[9] = "轩辕_金光舍利";
            action[10] = "怀光_金光舍利";
            action[11] = "太昊_金光舍利";
            action[12] = "天华_金光舍利";
            action[13] = "灵夙_金光舍利";
            action[14] = "英招_金光舍利";
            action[15] = "青云门_骑乘_金光舍利";
            action[16] = "青云门_骑乘_金光舍利";
            action[17] = "鬼王宗_骑乘_金光舍利";
            action[18] = "合欢派_骑乘_金光舍利";
            action[19] = "天音寺_骑乘_金光舍利";
            action[20] = "鬼道_骑乘_金光舍利";
            action[21] = "焚香谷_骑乘_金光舍利";
            action[22] = "兽神_骑乘_金光舍利";
            action[23] = "狐妖_骑乘_金光舍利";
            action[24] = "轩辕_骑乘_金光舍利";
            action[25] = "怀光_骑乘_金光舍利";
            action[26] = "太昊_骑乘_金光舍利";
            action[27] = "天华_骑乘_金光舍利";
            action[28] = "灵夙_骑乘_金光舍利";
            action[29] = "英招_骑乘_金光舍利";
            action[30] = "0";
            rangetype = 5;
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
        virtual ~ Skill1581Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1572 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 6000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 7200000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 90, 95, 100, 105, 110, 115, 120, 125, 125, 125 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        int GetCreditCost (int level) const
        {
            static int array[10] = { 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 38000, 45000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 1, 2, 1 * skill->GetLevel (), 3 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetAmount (1);
            skill->GetVictim ()->SetValue (21594 + skill->GetLevel ());
            skill->GetVictim ()->SetCreateitem (1);
            return true;
        }
#endif
    };
}
#endif
