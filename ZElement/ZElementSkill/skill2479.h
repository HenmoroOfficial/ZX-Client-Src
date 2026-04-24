#ifndef __CPPGEN_GNET_SKILL2479
#define __CPPGEN_GNET_SKILL2479
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2479:public Skill
    {
      public:
        enum
        { SKILL_ID = 2479 };
          Skill2479 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2479Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 100;
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
                return 2900;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2479Stub ():SkillStub (2479)
        {
            occupation = 52;
            name = L"天籁：高山流水";
            nativename = "天籁：高山流水";
            icon = "天籁：高山流水.dds";
            maxlevel = 15;
            maxlearn = 9;
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
            talent[0] = 2591;
            talent[1] = 2593;
            talent[2] = 2599;
            talent_size = 3;
            action[0] = "天华_领域_空山鸟语";
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
            action[12] = "天华_领域_空山鸟语";
            action[13] = "";
            action[14] = "";
            action[15] = "天华_领域_空山鸟语";
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
            action[27] = "天华_领域_空山鸟语";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 14;
            doenchant = true;
            dobless = true;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = 12;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill2479Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 2478 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 4 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (300 * (1 - 0.04 * skill->GetT2 ()));
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 120000 - 20000 * skill->GetT1 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 65, 67, 69, 71, 73, 75, 77, 79, 81 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (20);
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
            return _snwprintf (buffer, length, format, skill->GetLevel (), INT (skill->GetLevel () / 3) + 3);

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
            skill->GetVictim ()->SetClearsilent (1);
            skill->GetVictim ()->SetTime (3000);
            skill->GetVictim ()->SetImmunesilent (1);
            skill->GetVictim ()->SetProbability (1.0 * (120 * norm (skill->GetT0 ())));
            skill->GetVictim ()->SetTime (3000);
            skill->GetVictim ()->SetRatio (0.05 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetIncdefence (1);
            skill->GetVictim ()->SetTime (3000);
            skill->GetVictim ()->SetRatio (0.01 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetIncskilldamage (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (0));
            skill->GetVictim ()->SetTime (3600000);
            skill->GetVictim ()->SetRatio (31);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (1);
            skill->GetVictim ()->SetAbsulotearea (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (INT (skill->GetLevel () / 3) + 3);
        }
#endif
    };
}
#endif
