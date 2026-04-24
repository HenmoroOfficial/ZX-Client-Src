#ifndef __CPPGEN_GNET_SKILL2494
#define __CPPGEN_GNET_SKILL2494
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2494:public Skill
    {
      public:
        enum
        { SKILL_ID = 2494 };
          Skill2494 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2494Stub:public SkillStub
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
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.09 + 0.02 * skill->GetLevel ());
                skill->SetPlus (15 * skill->GetLevel () + 900);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State3:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.09 + 0.02 * skill->GetLevel ());
                skill->SetPlus (15 * skill->GetLevel () + 900);
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State4:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.09 + 0.02 * skill->GetLevel ());
                skill->SetPlus (15 * skill->GetLevel () + 900);
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill2494Stub ():SkillStub (2494)
        {
            occupation = 54;
            name = L"昆玉华章";
            nativename = "昆玉华章";
            icon = "昆玉华章.dds";
            maxlevel = 15;
            maxlearn = 8;
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
            talent[0] = 2616;
            talent[1] = 2619;
            talent[2] = 2620;
            talent_size = 3;
            action[0] = "天华_万壑松风";
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
            action[12] = "天华_万壑松风";
            action[13] = "";
            action[14] = "";
            action[15] = "天华_骑乘_万壑松风";
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
            action[27] = "天华_骑乘_万壑松风";
            action[28] = "";
            action[29] = "";
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
            weapon_limit = 12;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
            statestub.push_back (new State3 ());
            statestub.push_back (new State4 ());
#endif
        }
        virtual ~ Skill2494Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 2493 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (25 * skill->GetLevel () + 640);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 4000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 8000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[8] = { 115, 117, 119, 121, 122, 123, 124, 125 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (20);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (17);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (15);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               25 * skill->GetLevel () + 640, 15 * skill->GetLevel () + 900, 9 + 2 * skill->GetLevel (), 6 + skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetValue (2);
            skill->GetVictim ()->SetRepel (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetRes6 () + 15 * skill->GetT2 ()));
            skill->GetVictim ()->SetTime (2000 * skill->GetT2 ());
            skill->GetVictim ()->SetRatio (0.2 * skill->GetT2 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetSlow (1);
            skill->GetVictim ()->SetTime (8000 * norm (skill->GetT0 ()));
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (30 * skill->GetT0 ());
            skill->GetVictim ()->SetSubantiweak (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (6 + skill->GetLevel () + 2 * skill->GetT1 ());
        }
#endif
    };
}
#endif
