#ifndef __CPPGEN_GNET_SKILL1509
#define __CPPGEN_GNET_SKILL1509
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1509:public Skill
    {
      public:
        enum
        { SKILL_ID = 1509 };
          Skill1509 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1509Stub:public SkillStub
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
      Skill1509Stub ():SkillStub (1509)
        {
            occupation = 147;
            name = L"天罡战意";
            nativename = "天罡战意";
            icon = "天罡战意.dds";
            maxlevel = 50;
            maxlearn = 12;
            type = 2;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 8;
            clearmask = 64;
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
            talent[0] = 1508;
            talent[1] = 1505;
            talent_size = 2;
            action[0] = "青云门_天罡战意";
            action[1] = "青云门_天罡战意";
            action[2] = "鬼王宗_天罡战意";
            action[3] = "合欢派_天罡战意";
            action[4] = "天音寺_天罡战意";
            action[5] = "鬼道_天罡战意";
            action[6] = "焚香谷_天罡战意";
            action[7] = "兽神_天罡战意";
            action[8] = "狐妖_天罡战意";
            action[9] = "太昊_天罡战意";
            action[10] = "怀光_天罡战意";
            action[11] = "太昊_天罡战意";
            action[12] = "天华_天罡战意";
            action[13] = "灵夙_天罡战意";
            action[14] = "英招_天罡战意";
            action[15] = "青云门_骑乘_天罡战意";
            action[16] = "青云门_骑乘_天罡战意";
            action[17] = "鬼王宗_骑乘_天罡战意";
            action[18] = "合欢派_骑乘_天罡战意";
            action[19] = "天音寺_骑乘_天罡战意";
            action[20] = "鬼道_骑乘_天罡战意";
            action[21] = "焚香谷_骑乘_天罡战意";
            action[22] = "兽神_骑乘_天罡战意";
            action[23] = "狐妖_骑乘_天罡战意";
            action[24] = "太昊_骑乘_天罡战意";
            action[25] = "怀光_骑乘_天罡战意";
            action[26] = "太昊_骑乘_天罡战意";
            action[27] = "天华_骑乘_天罡战意";
            action[28] = "灵夙_骑乘_天罡战意";
            action[29] = "英招_骑乘_天罡战意";
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
        virtual ~ Skill1509Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1508 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 600000 - 30000 * skill->GetT0 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[12] = { 105, 105, 105, 110, 115, 120, 125, 125, 125, 125, 125, 125 };
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
            static int array[12] = { 25, 50, 125, 250, 500, 1000, 1800, 3600, 7200, 14400, 14400, 14400 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 0, 4 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel (), 5, 10);

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
            skill->GetVictim ()->SetTime (180000 * (1 + 0.05 * skill->GetT1 ()));
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue ((4 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel ()) *
                                           (1 + 0.05 * skill->GetPlayer ()->GetLevel () / 15) * (1 + 0.05 * skill->GetT1 ()));
            skill->GetVictim ()->SetAddattack4 (1);
            return true;
        }
#endif
    };
}
#endif
