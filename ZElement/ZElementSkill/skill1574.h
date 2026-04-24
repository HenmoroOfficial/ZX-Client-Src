#ifndef __CPPGEN_GNET_SKILL1574
#define __CPPGEN_GNET_SKILL1574
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1574:public Skill
    {
      public:
        enum
        { SKILL_ID = 1574 };
          Skill1574 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1574Stub:public SkillStub
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
      Skill1574Stub ():SkillStub (1574)
        {
            occupation = 150;
            name = L"无相无我";
            nativename = "无相无我";
            icon = "无相无我.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 3;
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
            action[0] = "青云门_无相无我";
            action[1] = "青云门_无相无我";
            action[2] = "鬼王宗_无相无我";
            action[3] = "合欢派_无相无我";
            action[4] = "天音寺_无相无我";
            action[5] = "鬼道_无相无我";
            action[6] = "焚香谷_无相无我";
            action[7] = "兽神_无相无我";
            action[8] = "狐妖_无相无我";
            action[9] = "轩辕_无相无我";
            action[10] = "怀光_无相无我";
            action[11] = "太昊_无相无我";
            action[12] = "天华_无相无我";
            action[13] = "灵夙_无相无我";
            action[14] = "英招_无相无我";
            action[15] = "青云门_骑乘_无相无我";
            action[16] = "青云门_骑乘_无相无我";
            action[17] = "鬼王宗_骑乘_无相无我";
            action[18] = "合欢派_骑乘_无相无我";
            action[19] = "天音寺_骑乘_无相无我";
            action[20] = "鬼道_骑乘_无相无我";
            action[21] = "焚香谷_骑乘_无相无我";
            action[22] = "兽神_骑乘_无相无我";
            action[23] = "狐妖_骑乘_无相无我";
            action[24] = "轩辕_骑乘_无相无我";
            action[25] = "怀光_骑乘_无相无我";
            action[26] = "太昊_骑乘_无相无我";
            action[27] = "天华_骑乘_无相无我";
            action[28] = "灵夙_骑乘_无相无我";
            action[29] = "英招_骑乘_无相无我";
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
#endif
        }
        virtual ~ Skill1574Stub ()
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
            return (float) (1125 + 45 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 600000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 90, 95, 100, 105, 110, 115, 120, 125, 125, 125 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10);
        }
        int GetCreditCost (int level) const
        {
            static int array[10] = { 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 38000, 45000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 1125 + 45 * skill->GetLevel (), 25, 10, 30 * skill->GetLevel (), 10 + 5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (3000 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (8);
            skill->GetVictim ()->SetValue (30 * skill->GetLevel ());
            skill->GetVictim ()->SetSubanti (1);
            skill->GetVictim ()->SetProbability (1.0 * (25));
            skill->GetVictim ()->SetTime (600000);
            skill->GetVictim ()->SetRatio (1547);
            skill->GetVictim ()->SetAmount (1548);
            skill->GetVictim ()->SetValue (1550);
            skill->GetVictim ()->SetSetcooldown (1);
            skill->GetVictim ()->SetProbability (1.0 * (25));
            skill->GetVictim ()->SetTime (600000);
            skill->GetVictim ()->SetRatio (1551);
            skill->GetVictim ()->SetAmount (1554);
            skill->GetVictim ()->SetValue (1553);
            skill->GetVictim ()->SetSetcooldown (1);
            skill->GetVictim ()->SetProbability (1.0 * (25));
            skill->GetVictim ()->SetTime (600000);
            skill->GetVictim ()->SetRatio (1555);
            skill->GetVictim ()->SetAmount (1556);
            skill->GetVictim ()->SetValue (1546);
            skill->GetVictim ()->SetSetcooldown (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetRatio (1572 + 4 * norm (zrand (100) / (100 - 10 - 5 * skill->GetLevel ())));
            skill->GetVictim ()->SetAmount (1572 + 5 * norm (zrand (100) / (100 - 10 - 5 * skill->GetLevel ())));
            skill->GetVictim ()->SetValue (1572 + 6 * norm (zrand (100) / (100 - 10 - 5 * skill->GetLevel ())));
            skill->GetVictim ()->SetClearcooldown (1);
            skill->GetVictim ()->SetRatio (1572 + 7 * norm (zrand (100) / (100 - 10 - 5 * skill->GetLevel ())));
            skill->GetVictim ()->SetAmount (1572);
            skill->GetVictim ()->SetValue (1572);
            skill->GetVictim ()->SetClearcooldown (1);
            return true;
        }
#endif
    };
}
#endif
