#ifndef __CPPGEN_GNET_SKILL3372
#define __CPPGEN_GNET_SKILL3372
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3372:public Skill
    {
      public:
        enum
        { SKILL_ID = 3372 };
          Skill3372 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3372Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 3000;
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
                return 5000;
            }
            void Calculate (Skill * skill) const
            {
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
                return 2000;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3372Stub ():SkillStub (3372)
        {
            occupation = 64;
            name = L"天守";
            nativename = "天守";
            icon = "天守·阵法.dds";
            maxlevel = 20;
            maxlearn = 5;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 3;
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
            cycle = 1;
            cyclegfx = "法阵结阵";
            cyclemode = 1;
            skill_class = 9;
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
            talent[0] = 3415;
            talent[1] = 3419;
            talent[2] = 3408;
            talent[3] = 3758;
            talent_size = 4;
            action[0] = "焚香谷_阵法精研【结阵】";
            action[1] = "焚香谷_阵法精研【结阵】";
            action[2] = "焚香谷_阵法精研【结阵】";
            action[3] = "焚香谷_阵法精研【结阵】";
            action[4] = "焚香谷_阵法精研【结阵】";
            action[5] = "焚香谷_阵法精研【结阵】";
            action[6] = "焚香谷_阵法精研【结阵】";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "焚香谷_骑乘_阵法精研【结阵】";
            action[16] = "焚香谷_骑乘_阵法精研【结阵】";
            action[17] = "焚香谷_骑乘_阵法精研【结阵】";
            action[18] = "焚香谷_骑乘_阵法精研【结阵】";
            action[19] = "焚香谷_骑乘_阵法精研【结阵】";
            action[20] = "焚香谷_骑乘_阵法精研【结阵】";
            action[21] = "焚香谷_骑乘_阵法精研【结阵】";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "";
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
            statestub.push_back (new State3 ());
#endif
        }
        virtual ~ Skill3372Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3371 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 6 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (200);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 10000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 120000 - 15000 * skill->GetT0 () - 120000 * 0.02 * skill->GetT3 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[5] = { 40, 43, 46, 50, 55 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (28);
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
            return _snwprintf (buffer, length, format, 10 * skill->GetLevel (), skill->GetLevel (), 10, 1, 10);

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
            skill->GetVictim ()->SetProbability (1.0 * (20 * skill->GetT2 ()));
            skill->GetVictim ()->SetBreakcasting (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (10000 * skill->GetLevel () + 10000 * (skill->GetPlayer ()->GetCyclemembercnt () - 1) +
                                          5000 * skill->GetT0 () + int (skill->GetCharging () / 4800) * 10000 + 1000 * skill->GetT3 ());
              skill->GetVictim ()->SetRatio (skill->GetLevel () + skill->GetT1 () + skill->GetPlayer ()->GetCyclemembercnt () - 1);
              skill->GetVictim ()->SetBuffid (0);
              skill->GetVictim ()->SetAmount (3604);
              skill->GetVictim ()->SetValue (2);
              skill->GetVictim ()->SetCycle (1);
              skill->GetVictim ()->SetTime (900);
              skill->GetVictim ()->SetRatio (0);
              skill->GetVictim ()->SetAmount (0);
              skill->GetVictim ()->SetValue (0);
              skill->GetVictim ()->SetEvilaura (1);
              skill->GetVictim ()->SetRatio (zrand (100) < skill->GetPlayer ()->GetSkilllevel (3759) * 2 ? 3449 : 218);
              skill->GetVictim ()->SetAmount (218);
              skill->GetVictim ()->SetValue (218);
              skill->GetVictim ()->SetClearcooldown (1);
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
