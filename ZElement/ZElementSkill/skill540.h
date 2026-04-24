#ifndef __CPPGEN_GNET_SKILL540
#define __CPPGEN_GNET_SKILL540
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill540:public Skill
    {
      public:
        enum
        { SKILL_ID = 540 };
          Skill540 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill540Stub:public SkillStub
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill540Stub ():SkillStub (540)
        {
            occupation = 19;
            name = L"极度深寒";
            nativename = "极度深寒";
            icon = "极度深寒.dds";
            maxlevel = 18;
            maxlearn = 7;
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
            talent[0] = 592;
            talent[1] = 596;
            talent[2] = 599;
            talent[3] = 615;
            talent_size = 4;
            action[0] = "青云门_极度深寒";
            action[1] = "青云门_极度深寒";
            action[2] = "青云门_极度深寒";
            action[3] = "青云门_极度深寒";
            action[4] = "青云门_极度深寒";
            action[5] = "青云门_极度深寒";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "青云门_骑乘_极度深寒";
            action[16] = "青云门_骑乘_极度深寒";
            action[17] = "青云门_骑乘_极度深寒";
            action[18] = "青云门_骑乘_极度深寒";
            action[19] = "青云门_骑乘_极度深寒";
            action[20] = "青云门_骑乘_极度深寒";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 3;
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
        virtual ~ Skill540Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 537 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (18 + 3 * skill->GetLevel ());
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (350 + 20 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 90000 - 10000 * skill->GetT1 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[7] = { 110, 112, 114, 116, 118, 120, 122 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (15 + 3 * skill->GetLevel ());
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (16 + 3 * skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (15 + 3 * skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               350 + 20 * skill->GetLevel (),
                               15,
                               15 + 3 * skill->GetLevel (), 15 + 3 * skill->GetLevel (), 16, 64 * skill->GetLevel () + 16, 4 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (16000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount ((64 * skill->GetLevel () + 16) * (1 + 0.25 * skill->GetT2 ()) * 1.125);
            skill->GetVictim ()->SetCycsubdefence (1);
            skill->GetVictim ()->SetProbability (1.0 * (20 * skill->GetT3 ()));
            skill->GetVictim ()->SetTime (8010 * skill->GetT3 ());
            skill->GetVictim ()->SetRatio (0.25);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetSlow (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (16000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (skill->GetPlayer ()->GetLevel () * 10);
            skill->GetVictim ()->SetValue (0.04 * skill->GetLevel () * skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetSubhp (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (4000 * norm (skill->GetT0 ()));
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0.15 * skill->GetT0 () * (350 + 20 * skill->GetLevel ()));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetMpgen (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (15);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
