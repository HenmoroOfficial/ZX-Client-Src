#ifndef __CPPGEN_GNET_SKILL2071
#define __CPPGEN_GNET_SKILL2071
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2071:public Skill
    {
      public:
        enum
        { SKILL_ID = 2071 };
          Skill2071 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2071Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1600;
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMp ());
                skill->GetPlayer ()->SetMp (0);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2071Stub ():SkillStub (2071)
        {
            occupation = 150;
            name = L"天地同寿II";
            nativename = "天地同寿II";
            icon = "天地同寿II.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 2;
            eventflag = 1;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 11;
            clearmask = 132;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 3;
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
            action[0] = "天音寺_舍生取义";
            action[1] = "天音寺_舍生取义";
            action[2] = "天音寺_舍生取义";
            action[3] = "天音寺_舍生取义";
            action[4] = "天音寺_舍生取义";
            action[5] = "天音寺_舍生取义";
            action[6] = "天音寺_舍生取义";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "天音寺_骑乘_舍生取义";
            action[16] = "天音寺_骑乘_舍生取义";
            action[17] = "天音寺_骑乘_舍生取义";
            action[18] = "天音寺_骑乘_舍生取义";
            action[19] = "天音寺_骑乘_舍生取义";
            action[20] = "天音寺_骑乘_舍生取义";
            action[21] = "天音寺_骑乘_舍生取义";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
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
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill2071Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 284 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 9 };
            return array[index];
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
            return 60000 * 2;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 125, 125, 125, 125, 125, 127, 129, 131, 133, 135 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (5.8 + 0.2 * skill->GetLevel ());
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (5.8 + 0.2 * skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (2.8 + 0.2 * skill->GetLevel ());
        }
        int GetCreditCost (int level) const
        {
            static int array[10] = { 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 38000, 45000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               10 + 2 * skill->GetLevel (), 30 + 5 * skill->GetLevel (), 2 * skill->GetLevel (), 15 + 1.5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue ((0.10 + 0.02 * skill->GetLevel ()) * skill->GetPlayer ()->GetVar1 () * 15);
            skill->GetVictim ()->SetHpgen (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (15000 + 1500 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio (0.02 * skill->GetLevel ());
            skill->GetVictim ()->SetDecdamage (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (15000 + 1500 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio (0.3 + 0.05 * skill->GetLevel ());
            skill->GetVictim ()->SetDecdamage (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (10);
        }
#endif
    };
}
#endif
