#ifndef __CPPGEN_GNET_SKILL5057
#define __CPPGEN_GNET_SKILL5057
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill5057:public Skill
    {
      public:
        enum
        { SKILL_ID = 5057 };
          Skill5057 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill5057Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes3 ());
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetRes4 ());
                skill->GetPlayer ()->SetVar3 (skill->GetPlayer ()->GetRes1 ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill5057Stub ():SkillStub (5057)
        {
            occupation = 128;
            name = L"无间寂";
            nativename = "无间寂";
            icon = "无间寂.dds";
            maxlevel = 1;
            maxlearn = 1;
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
            action[0] = "鬼王宗_无间寂";
            action[1] = "鬼王宗_无间寂";
            action[2] = "鬼王宗_无间寂";
            action[3] = "鬼王宗_无间寂";
            action[4] = "鬼王宗_无间寂";
            action[5] = "鬼王宗_无间寂";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "0";
            action[14] = "0";
            action[15] = "鬼王宗_骑乘_无间寂";
            action[16] = "鬼王宗_骑乘_无间寂";
            action[17] = "鬼王宗_骑乘_无间寂";
            action[18] = "鬼王宗_骑乘_无间寂";
            action[19] = "鬼王宗_骑乘_无间寂";
            action[20] = "鬼王宗_骑乘_无间寂";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "0";
            action[29] = "0";
            action[30] = "技能攻击2";
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
        virtual ~ Skill5057Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetHpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetInkcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (632 + 16 * 8 + 1.7 * 8 * 8);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 120000 - 12000 * 2;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (9 + 3 * 8);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (9 + 3 * 8);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (6 + 3 * 8);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, INT (632 + 16 * 8 + 1.7 * 8 * 8), 40, 9 + 3 * 8, 3 + 8);

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () > 600 ? skill->GetPlayer ()->GetVar1 () : 600));
            skill->GetVictim ()->SetTime ((3000 + 1000 * 8) * (1 + 0.3 * 2) + 100);
            skill->GetVictim ()->SetWrap (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (14000 - 2000 * 2);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount ((60 + 120 * 8) * 2 * 8);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar2 () > 600 ? skill->GetPlayer ()->GetVar2 () : 600));
            skill->GetVictim ()->SetTime (2050 * 2);
            skill->GetVictim ()->SetSilent (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar3 () > 600 ? skill->GetPlayer ()->GetVar3 () : 600));
            skill->GetVictim ()->SetTime ((3000 + 1000 * 8) * (1 + 0.3 * 2) + 100);
            skill->GetVictim ()->SetDizzy (1);
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
