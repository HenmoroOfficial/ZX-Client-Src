#ifndef __CPPGEN_GNET_SKILL3889
#define __CPPGEN_GNET_SKILL3889
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3889:public Skill
    {
      public:
        enum
        { SKILL_ID = 3889 };
          Skill3889 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3889Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 2000;
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
                return 500;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3889Stub ():SkillStub (3889)
        {
            occupation = 129;
            name = L"捕鱼达人";
            nativename = "捕鱼达人";
            icon = "捕捉.dds";
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
            action[0] = "青云门_动物捕捉";
            action[1] = "青云门_动物捕捉";
            action[2] = "鬼王宗_动物捕捉";
            action[3] = "合欢派_动物捕捉";
            action[4] = "天音寺_动物捕捉";
            action[5] = "鬼道_动物捕捉";
            action[6] = "青云门_动物捕捉";
            action[7] = "兽神_动物捕捉";
            action[8] = "狐妖_动物捕捉";
            action[9] = "兽神_动物捕捉";
            action[10] = "怀光_动物捕捉";
            action[11] = "兽神_动物捕捉";
            action[12] = "天华_动物捕捉";
            action[13] = "灵夙_动物捕捉";
            action[14] = "英招_动物捕捉";
            action[15] = "青云门_骑乘_动物捕捉";
            action[16] = "青云门_骑乘_动物捕捉";
            action[17] = "鬼王宗_骑乘_动物捕捉";
            action[18] = "合欢派_骑乘_动物捕捉";
            action[19] = "天音寺_骑乘_动物捕捉";
            action[20] = "鬼道_骑乘_动物捕捉";
            action[21] = "青云门_骑乘_动物捕捉";
            action[22] = "兽神_骑乘_动物捕捉";
            action[23] = "狐妖_骑乘_动物捕捉";
            action[24] = "兽神_骑乘_动物捕捉";
            action[25] = "怀光_骑乘_动物捕捉";
            action[26] = "兽神_骑乘_动物捕捉";
            action[27] = "天华_骑乘_动物捕捉";
            action[28] = "灵夙_骑乘_动物捕捉";
            action[29] = "英招_骑乘_动物捕捉";
            action[30] = "0";
            rangetype = 6;
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
        virtual ~ Skill3889Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2500;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 1000;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (5);
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
                                                 (skill->GetPlayer ()->GetTmplid () ==
                                                  52281 ? 100 : (skill->GetPlayer ()->GetTmplid () == 52282 ? 70 : 0)));
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetDirecthurt (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetTmplid () ==
                                                  52283 ? 40 : (skill->GetPlayer ()->GetTmplid () == 52284 ? 30 : 0)));
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetDirecthurt (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetTmplid () ==
                                                  52285 ? 20 : (skill->GetPlayer ()->GetTmplid () == 52286 ? 10 : 0)));
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetDirecthurt (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (20);
        }
#endif
    };
}
#endif
