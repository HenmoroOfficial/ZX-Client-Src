#ifndef __CPPGEN_GNET_SKILL389
#define __CPPGEN_GNET_SKILL389
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill389:public Skill
    {
      public:
        enum
        { SKILL_ID = 389 };
          Skill389 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill389Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 7000;
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill389Stub ():SkillStub (389)
        {
            occupation = 129;
            name = L"传送术";
            nativename = "传送术";
            icon = "回城术.dds";
            maxlevel = 1;
            maxlearn = 1;
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
            action[0] = "回城_空";
            action[1] = "回城_剑";
            action[2] = "回城_轮";
            action[3] = "回城_刀";
            action[4] = "回城_杖";
            action[5] = "回城_爪";
            action[6] = "回城_刃";
            action[7] = "回城_斧";
            action[8] = "回城_弓";
            action[9] = "回城_空";
            action[10] = "回城_镰";
            action[11] = "回城_拳";
            action[12] = "回城_琴";
            action[13] = "回城_笔";
            action[14] = "回城_矛";
            action[15] = "回城_骑乘_空";
            action[16] = "回城_骑乘_剑";
            action[17] = "回城_骑乘_刀";
            action[18] = "回城_骑乘_轮";
            action[19] = "回城_骑乘_杖";
            action[20] = "回城_骑乘_爪";
            action[21] = "回城_骑乘_刃";
            action[22] = "回城_骑乘_斧";
            action[23] = "回城_骑乘_弓";
            action[24] = "回城_骑乘_空";
            action[25] = "回城_骑乘_镰";
            action[26] = "回城_骑乘_拳";
            action[27] = "回城_骑乘_琴";
            action[28] = "回城_骑乘_琴";
            action[29] = "回城_矛";
            action[30] = "技能攻击1";
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
        virtual ~ Skill389Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 8000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 3600000;
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
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetGohome (1);
            return true;
        }
#endif
    };
}
#endif
