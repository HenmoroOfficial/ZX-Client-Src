#ifndef __CPPGEN_GNET_SKILL2583
#define __CPPGEN_GNET_SKILL2583
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2583:public Skill
    {
      public:
        enum
        { SKILL_ID = 2583 };
          Skill2583 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2583Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 9750;
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
                return 250;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2583Stub ():SkillStub (2583)
        {
            occupation = 151;
            name = L"双宿双飞";
            nativename = "双宿双飞";
            icon = "双宿双飞.dds";
            maxlevel = 3;
            maxlearn = 1;
            type = 2;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
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
            action[0] = "鬼王宗_夫妻技能_天涯比翼";
            action[1] = "青云门_夫妻技能_天涯比翼";
            action[2] = "鬼王宗_夫妻技能_天涯比翼";
            action[3] = "合欢派_夫妻技能_天涯比翼";
            action[4] = "天音寺_夫妻技能_天涯比翼";
            action[5] = "鬼道_夫妻技能_天涯比翼";
            action[6] = "0";
            action[7] = "兽神_夫妻技能_天涯比翼";
            action[8] = "狐妖_夫妻技能_天涯比翼";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "鬼王宗_骑乘_夫妻技能_天涯比翼";
            action[16] = "青云门_骑乘_夫妻技能_天涯比翼";
            action[17] = "鬼王宗_骑乘_夫妻技能_天涯比翼";
            action[18] = "合欢派_骑乘_夫妻技能_天涯比翼";
            action[19] = "天音寺_骑乘_夫妻技能_天涯比翼";
            action[20] = "鬼道_骑乘_夫妻技能_天涯比翼";
            action[21] = "0";
            action[22] = "兽神_骑乘_夫妻技能_天涯比翼";
            action[23] = "狐妖_骑乘_夫妻技能_天涯比翼";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
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
        virtual ~ Skill2583Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (100);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 10000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 3600000 * (18 - 6 * skill->GetLevel ());
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 18 - 6 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetJumptospouse (1);
            return true;
        }
#endif
    };
}
#endif
