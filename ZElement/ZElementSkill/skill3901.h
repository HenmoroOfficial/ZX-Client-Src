#ifndef __CPPGEN_GNET_SKILL3901
#define __CPPGEN_GNET_SKILL3901
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3901:public Skill
    {
      public:
        enum
        { SKILL_ID = 3901 };
          Skill3901 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3901Stub:public SkillStub
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
      Skill3901Stub ():SkillStub (3901)
        {
            occupation = 128;
            name = L"打倒怪叔叔";
            nativename = "打倒怪叔叔";
            icon = "捕快.dds";
            maxlevel = 1;
            maxlearn = 0;
            type = 3;
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
            action[0] = "技能攻击3";
            action[1] = "技能攻击3";
            action[2] = "技能攻击3";
            action[3] = "技能攻击3";
            action[4] = "技能攻击3";
            action[5] = "技能攻击3";
            action[6] = "技能攻击3";
            action[7] = "技能攻击3";
            action[8] = "技能攻击3";
            action[9] = "技能攻击3";
            action[10] = "技能攻击3";
            action[11] = "技能攻击3";
            action[12] = "技能攻击3";
            action[13] = "";
            action[14] = "";
            action[15] = "技能攻击3";
            action[16] = "技能攻击3";
            action[17] = "技能攻击3";
            action[18] = "技能攻击3";
            action[19] = "技能攻击3";
            action[20] = "技能攻击3";
            action[21] = "技能攻击3";
            action[22] = "技能攻击3";
            action[23] = "技能攻击3";
            action[24] = "技能攻击3";
            action[25] = "技能攻击3";
            action[26] = "技能攻击3";
            action[27] = "技能攻击3";
            action[28] = "";
            action[29] = "";
            action[30] = "技能攻击3";
            rangetype = 0;
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
        virtual ~ Skill3901Stub ()
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
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 4000;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10);
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
            skill->GetVictim ()->SetProbability (1.0 *
                                                 ((skill->GetPlayer ()->GetTmplid () == 52260 && skill->GetPlayer ()->GetRes1 () < 2000) ? 120 : 0));
            skill->GetVictim ()->SetValue (5000);
            skill->GetVictim ()->SetDirecthurt (1);
            return true;
        }
#endif
    };
}
#endif
