#ifndef __CPPGEN_GNET_SKILL1563
#define __CPPGEN_GNET_SKILL1563
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1563:public Skill
    {
      public:
        enum
        { SKILL_ID = 1563 };
          Skill1563 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1563Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes4 () + 80);
                skill->SetRatio (1 + 0.02 * skill->GetLevel () + 0.04 * (skill->GetT0 () + skill->GetT1 () + skill->GetT2 ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1563Stub ():SkillStub (1563)
        {
            occupation = 149;
            name = L"寒霜剑气II";
            nativename = "寒霜剑气II";
            icon = "寒霜剑气II.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 10;
            clearmask = 130;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 2;
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
            talent[0] = 1564;
            talent[1] = 2054;
            talent[2] = 2055;
            talent_size = 3;
            action[0] = "青云门_霜刃决";
            action[1] = "青云门_霜刃决";
            action[2] = "青云门_霜刃决";
            action[3] = "青云门_霜刃决";
            action[4] = "青云门_霜刃决";
            action[5] = "青云门_霜刃决";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "青云门_骑乘_霜刃决";
            action[16] = "青云门_骑乘_霜刃决";
            action[17] = "青云门_骑乘_霜刃决";
            action[18] = "青云门_骑乘_霜刃决";
            action[19] = "青云门_骑乘_霜刃决";
            action[20] = "青云门_骑乘_霜刃决";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "技能攻击1";
            rangetype = 1;
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
        virtual ~ Skill1563Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 226 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 9 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1500 + 10 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 16000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 125, 125, 125, 125, 125, 127, 129, 131, 133, 135 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (3);
        }
        float GetAttackdistance (Skill * skill) const
        {
            return (float) (17);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (16);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10.7);
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
                               1500 + 10 * skill->GetLevel (),
                               25, 2 * skill->GetLevel (), 15 + 5 * skill->GetLevel (), 2 * skill->GetLevel (), 8 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (600));
            skill->GetVictim ()->SetTime (16000 + 100);
            skill->GetVictim ()->SetRatio (0.15 + 0.05 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetSlow (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetVar1 () >
                                                  600 ? skill->GetPlayer ()->GetVar1 () * norm (zrand (100) / (100 - 2 * skill->GetLevel ())) : 600 *
                                                  norm (zrand (100) / (100 - 2 * skill->GetLevel ()))));
            skill->GetVictim ()->SetTime (6000 + 100);
            skill->GetVictim ()->SetSilent (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (25);
        }
#endif
    };
}
#endif
