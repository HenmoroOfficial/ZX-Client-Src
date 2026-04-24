#ifndef __CPPGEN_GNET_SKILL1870
#define __CPPGEN_GNET_SKILL1870
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1870:public Skill
    {
      public:
        enum
        { SKILL_ID = 1870 };
          Skill1870 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1870Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 700;
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
                return 675;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrit (0.01 * skill->GetT0 ());
                skill->SetRatio (1 + 0.01 * int (skill->GetT3 () / 2.99) + 0.01 * (skill->GetLevel () + skill->GetT2 ()));
                  skill->SetPlus (232 + 11 * skill->GetLevel ());
                  skill->GetPlayer ()->SetPerform (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State3:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 625;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrit (0.01 * skill->GetT0 ());
                skill->SetRatio (1 + 0.01 * int (skill->GetT3 () / 2.99) + 0.01 * (skill->GetLevel () + skill->GetT2 ()));
                  skill->SetPlus (232 + 11 * skill->GetLevel ());
                  skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill1870Stub ():SkillStub (1870)
        {
            occupation = 36;
            name = L"连山";
            nativename = "连山";
            icon = "连山.dds";
            maxlevel = 10;
            maxlearn = 9;
            type = 1;
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
            skill_class = 5;
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
            talent[0] = 2093;
            talent[1] = 2095;
            talent[2] = 2096;
            talent[3] = 1861;
            talent_size = 4;
            action[0] = "兽神_连山";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "兽神_连山";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "0";
            action[14] = "0";
            action[15] = "兽神_骑乘_连山";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "兽神_骑乘_连山";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "0";
            action[29] = "0";
            action[30] = "0";
            rangetype = 2;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = 7;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
            statestub.push_back (new State3 ());
#endif
        }
        virtual ~ Skill1870Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 0 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 24 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (6);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (395 + 13 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 8000 - 1000 * skill->GetT0 () - 1000 * int (skill->GetT3 () / 2.99);
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 105, 107, 109, 111, 113, 115, 117, 119, 121 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (8);
        }
        float GetAngle (Skill * skill) const
        {
            return (float) (1 - 0.0111111 * (180));
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (3);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 395 + 13 * skill->GetLevel (), skill->GetLevel (), 232 + 11 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (zrand (100) < 5 * skill->GetT1 ()? 600 : 0));
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetWrap (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (10 + skill->GetLevel ());
        }
#endif
    };
}
#endif
