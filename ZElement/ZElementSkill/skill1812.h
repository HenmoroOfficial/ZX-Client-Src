#ifndef __CPPGEN_GNET_SKILL1812
#define __CPPGEN_GNET_SKILL1812
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1812:public Skill
    {
      public:
        enum
        { SKILL_ID = 1812 };
          Skill1812 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1812Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 2925;
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
                return 75;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetHp (skill->GetPlayer ()->GetHp () * 0.01 + 1);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1812Stub ():SkillStub (1812)
        {
            occupation = 152;
            name = L"再续前缘";
            nativename = "再续前缘";
            icon = "再续前缘.dds";
            maxlevel = 10;
            maxlearn = 5;
            type = 2;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 1;
            autoattack = 0;
            allowform = 7;
            credittype = 6;
            clearmask = 8;
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
            action[0] = "鬼王宗_夫妻技能_再续前缘";
            action[1] = "青云门_夫妻技能_再续前缘";
            action[2] = "鬼王宗_夫妻技能_再续前缘";
            action[3] = "合欢派_夫妻技能_再续前缘";
            action[4] = "天音寺_夫妻技能_再续前缘";
            action[5] = "鬼道_夫妻技能_再续前缘";
            action[6] = "焚香谷_夫妻技能_再续前缘";
            action[7] = "兽神_夫妻技能_再续前缘";
            action[8] = "狐妖_夫妻技能_再续前缘";
            action[9] = "轩辕_夫妻技能_再续前缘";
            action[10] = "怀光_夫妻技能_再续前缘";
            action[11] = "太昊_夫妻技能_再续前缘";
            action[12] = "天华_夫妻技能_再续前缘";
            action[13] = "灵夙_夫妻技能_再续前缘";
            action[14] = "英招_夫妻技能_再续前缘";
            action[15] = "鬼王宗_骑乘_夫妻技能_再续前缘";
            action[16] = "青云门_骑乘_夫妻技能_再续前缘";
            action[17] = "鬼王宗_骑乘_夫妻技能_再续前缘";
            action[18] = "合欢派_骑乘_夫妻技能_再续前缘";
            action[19] = "天音寺_骑乘_夫妻技能_再续前缘";
            action[20] = "鬼道_骑乘_夫妻技能_再续前缘";
            action[21] = "焚香谷_骑乘_夫妻技能_再续前缘";
            action[22] = "兽神_骑乘_夫妻技能_再续前缘";
            action[23] = "狐妖_骑乘_夫妻技能_再续前缘";
            action[24] = "轩辕_骑乘_夫妻技能_再续前缘";
            action[25] = "怀光_骑乘_夫妻技能_再续前缘";
            action[26] = "太昊_骑乘_夫妻技能_再续前缘";
            action[27] = "天华_骑乘_夫妻技能_再续前缘";
            action[28] = "灵夙_骑乘_夫妻技能_再续前缘";
            action[29] = "英招_骑乘_夫妻技能_再续前缘";
            action[30] = "0";
            rangetype = 8;
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
        virtual ~ Skill1812Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1811 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (16 + 2 * skill->GetLevel ());
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return (6 - skill->GetLevel () + 0.5 * int (skill->GetLevel () / 5.9)) *3600000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[5] = { 30, 30, 30, 30, 30 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (14 + 2 * skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (12 + 2 * skill->GetLevel ());
        }
        int GetCreditCost (int level) const
        {
            static int array[5] = { 1200, 2400, 3600, 4800, 6000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, (6 - skill->GetLevel () + 0.5 * int (skill->GetLevel () / 5.9)));

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
            skill->GetVictim ()->SetRatio (0.03);
            skill->GetVictim ()->SetValue (0.2);
            skill->GetVictim ()->SetSalvation (1);
            return true;
        }
#endif
    };
}
#endif
