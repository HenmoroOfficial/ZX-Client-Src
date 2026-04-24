#ifndef __CPPGEN_GNET_SKILL2721
#define __CPPGEN_GNET_SKILL2721
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2721:public Skill
    {
      public:
        enum
        { SKILL_ID = 2721 };
          Skill2721 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2721Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2721Stub ():SkillStub (2721)
        {
            occupation = 128;
            name = L"千变万化";
            nativename = "千变万化";
            icon = "嘲讽.dds";
            maxlevel = 10;
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
            action[0] = "鬼道_百鬼夜行";
            action[1] = "青云门_五气朝元";
            action[2] = "鬼王宗_锐金咒";
            action[3] = "合欢派_热情";
            action[4] = "天音寺_慈航普度";
            action[5] = "鬼道_百鬼夜行";
            action[6] = "0";
            action[7] = "兽神_横行";
            action[8] = "狐妖_风行者";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "鬼道_乘骑_百鬼夜行";
            action[16] = "青云门_乘骑_五气朝元";
            action[17] = "鬼王宗_乘骑_锐金咒";
            action[18] = "合欢派_乘骑_热情";
            action[19] = "天音寺_乘骑_慈航普度";
            action[20] = "鬼道_乘骑_百鬼夜行";
            action[21] = "0";
            action[22] = "兽神_乘骑_横行";
            action[23] = "狐妖_乘骑_风行者";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
            doenchant = true;
            dobless = false;
            need_item = true;
            item_relation = 0;
            item1_id = 34297;
            item1_num = 1;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 23;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill2721Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (20);
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
            return 1000;
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
            skill->GetVictim ()->SetProbability (1.0 * (99));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetAmount (1);
            skill->GetVictim ()->SetValue (skill->GetLevel () + 34297);
            skill->GetVictim ()->SetCreateitem (1);
            return true;
        }
#endif
    };
}
#endif
