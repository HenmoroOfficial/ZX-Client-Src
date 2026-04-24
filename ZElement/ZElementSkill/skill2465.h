#ifndef __CPPGEN_GNET_SKILL2465
#define __CPPGEN_GNET_SKILL2465
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2465:public Skill
    {
      public:
        enum
        { SKILL_ID = 2465 };
          Skill2465 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2465Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2465Stub ():SkillStub (2465)
        {
            occupation = 51;
            name = L"行云流水";
            nativename = "行云流水";
            icon = "行云流水.dds";
            maxlevel = 12;
            maxlearn = 8;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
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
            skill_class = 8;
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
            talent[0] = 2588;
            talent[1] = 2597;
            talent[2] = 2694;
            talent_size = 3;
            action[0] = "天华_奔雷";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "天华_奔雷";
            action[13] = "";
            action[14] = "";
            action[15] = "天华_骑乘_奔雷";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "天华_骑乘_奔雷";
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
        virtual ~ Skill2465Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 2464 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 4 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (90);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 90000 - 10000 * skill->GetT1 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[8] = { 22, 24, 26, 28, 30, 32, 34, 36 };
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
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 8 + skill->GetLevel (), 0.3 + 0.4 * skill->GetLevel (), 4 + skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (8000 + 1000 * skill->GetLevel () + 2000 * skill->GetT0 () + skill->GetT2 () * 1000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (0.3 + 0.4 * skill->GetLevel () + 0.5 * (skill->GetT2 () == 10 ? 1 : 0));
            skill->GetVictim ()->SetAddspeed (1);
            skill->GetVictim ()->SetTime (4000 + 1000 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (0.3 * skill->GetPlayer ()->GetVar1 () + 5 * skill->GetLevel ());
            skill->GetVictim ()->SetAddantisleep (1);
            return true;
        }
#endif
    };
}
#endif
