#ifndef __CPPGEN_GNET_SKILL2895
#define __CPPGEN_GNET_SKILL2895
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2895:public Skill
    {
      public:
        enum
        { SKILL_ID = 2895 };
          Skill2895 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2895Stub:public SkillStub
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2895Stub ():SkillStub (2895)
        {
            occupation = 129;
            name = L"御驾亲征·安";
            nativename = "御驾亲征·安";
            icon = "御驾亲征·安.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 5;
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
            action[0] = "鬼王宗_还神";
            action[1] = "青云门_还神";
            action[2] = "鬼王宗_还神";
            action[3] = "合欢派_还神";
            action[4] = "天音寺_还神";
            action[5] = "鬼道_还神";
            action[6] = "0";
            action[7] = "兽神_还神";
            action[8] = "狐妖_还神";
            action[9] = "0";
            action[10] = "怀光_偷袭";
            action[11] = "0";
            action[12] = "天华_春雷";
            action[13] = "";
            action[14] = "";
            action[15] = "鬼王宗_骑乘_还神";
            action[16] = "青云门_骑乘_还神";
            action[17] = "鬼王宗_骑乘_还神";
            action[18] = "合欢派_骑乘_还神";
            action[19] = "天音寺_骑乘_还神";
            action[20] = "鬼道_骑乘_还神";
            action[21] = "0";
            action[22] = "兽神_骑乘_还神";
            action[23] = "狐妖_骑乘_还神";
            action[24] = "0";
            action[25] = "怀光_骑乘_偷袭";
            action[26] = "0";
            action[27] = "天华_骑乘_春雷";
            action[28] = "";
            action[29] = "";
            action[30] = "技能攻击2";
            rangetype = 2;
            doenchant = true;
            dobless = true;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 1;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill2895Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (35);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (3000);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 480000;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (35);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (32);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (30);
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
            return _snwprintf (buffer, length, format, 3000, 60);

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (skill->GetPlayer ()->GetType () == 0 ? 30000 : 0);
            skill->GetVictim ()->SetRatio (0.60001);
            skill->GetVictim ()->SetAmount (25000);
            skill->GetVictim ()->SetDechurt (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetRatio (0.60001);
            skill->GetVictim ()->SetAmount (25000);
            skill->GetVictim ()->SetDechurt (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (40);
        }
#endif
    };
}
#endif
