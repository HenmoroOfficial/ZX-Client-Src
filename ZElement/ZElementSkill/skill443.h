#ifndef __CPPGEN_GNET_SKILL443
#define __CPPGEN_GNET_SKILL443
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill443:public Skill
    {
      public:
        enum
        { SKILL_ID = 443 };
          Skill443 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill443Stub:public SkillStub
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
      Skill443Stub ():SkillStub (443)
        {
            occupation = 131;
            name = L"群策群力";
            nativename = "群策群力";
            icon = "群策群力.dds";
            maxlevel = 15;
            maxlearn = 15;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
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
            preskillnum = 0;
            talent[0] = 1714;
            talent_size = 1;
            action[0] = "鬼王宗_群策群力";
            action[1] = "青云门_群策群力";
            action[2] = "鬼王宗_群策群力";
            action[3] = "合欢派_群策群力";
            action[4] = "天音寺_群策群力";
            action[5] = "合欢派_群策群力";
            action[6] = "焚香谷_群策群力";
            action[7] = "兽神_群策群力";
            action[8] = "狐妖_群策群力";
            action[9] = "轩辕_群策群力";
            action[10] = "怀光_群策群力";
            action[11] = "太昊_群策群力";
            action[12] = "天华_群策群力";
            action[13] = "灵夙_群策群力";
            action[14] = "英招_群策群力";
            action[15] = "鬼王宗_骑乘_群策群力";
            action[16] = "青云门_骑乘_群策群力";
            action[17] = "鬼王宗_骑乘_群策群力";
            action[18] = "合欢派_骑乘_群策群力";
            action[19] = "天音寺_骑乘_群策群力";
            action[20] = "合欢派_骑乘_群策群力";
            action[21] = "焚香谷_骑乘_群策群力";
            action[22] = "兽神_骑乘_群策群力";
            action[23] = "狐妖_骑乘_群策群力";
            action[24] = "轩辕_骑乘_群策群力";
            action[25] = "怀光_骑乘_群策群力";
            action[26] = "太昊_骑乘_群策群力";
            action[27] = "天华_骑乘_群策群力";
            action[28] = "灵夙_骑乘_群策群力";
            action[29] = "英招_骑乘_群策群力";
            action[30] = "0";
            rangetype = 2;
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
        virtual ~ Skill443Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (30);
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
            return 1800000 - 450000 * skill->GetT0 ();
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (30);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (30);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 0, skill->GetLevel (), 200 + 100 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetLevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (600000);
            skill->GetVictim ()->SetRatio (0.01 * skill->GetLevel () * (1 + 0.25 * skill->GetT0 ()));
            skill->GetVictim ()->SetValue ((200 + 100 * skill->GetLevel ()) * (1 + 0.25 * skill->GetT0 ()));
            skill->GetVictim ()->SetFamilyincmp (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (15);
        }
#endif
    };
}
#endif
