#ifndef __CPPGEN_GNET_SKILL3487
#define __CPPGEN_GNET_SKILL3487
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3487:public Skill
    {
      public:
        enum
        { SKILL_ID = 3487 };
          Skill3487 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3487Stub:public SkillStub
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
      Skill3487Stub ():SkillStub (3487)
        {
            occupation = 98;
            name = L"地煞狂灵·神";
            nativename = "地煞狂灵·神";
            icon = "地煞狂灵·神.dds";
            maxlevel = 8;
            maxlearn = 5;
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
            skill_class = 10;
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
            talent[0] = 3543;
            talent[1] = 3540;
            talent[2] = 3744;
            talent[3] = 3745;
            talent_size = 4;
            action[0] = "太昊_体型变大";
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
            action[11] = "太昊_体型变大";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "太昊_骑乘_体型变大";
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
            action[26] = "太昊_骑乘_体型变大";
            action[27] = "0";
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
            weapon_limit = 11;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill3487Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3486 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (543);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 180000 - 20000 * skill->GetT1 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[5] = { 95, 97, 100, 102, 105 };
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
            return _snwprintf (buffer, length, format,
                               2 * skill->GetLevel (), 3 * skill->GetLevel (), 10 * skill->GetLevel (), 5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (5000 * skill->GetLevel () + 5000 * skill->GetT1 () + 3000 * skill->GetT3 ());
            skill->GetVictim ()->SetRatio (1 + 0.05 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (10 * (skill->GetLevel () * 3 + skill->GetT0 () * 2));
            skill->GetVictim ()->SetAmount (0.02 * skill->GetLevel () + skill->GetT0 () * 0.015);
            skill->GetVictim ()->SetValue (0.1 * skill->GetLevel () - skill->GetT0 () * 0.15);
            skill->GetVictim ()->SetGuishen2 (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetT2 () == 0 ? 0 : 100));
            skill->GetVictim ()->SetTime (5000 * skill->GetLevel () + 5000 * skill->GetT1 () + 3000 * skill->GetT3 ());
            skill->GetVictim ()->SetRatio (0.05 * skill->GetT2 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetInccrithurt (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetT2 () == 0 ? 0 : 100));
            skill->GetVictim ()->SetTime (5000 * skill->GetLevel () + 5000 * skill->GetT1 () + 3000 * skill->GetT3 ());
            skill->GetVictim ()->SetRatio (0.01 * skill->GetT2 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetIncskillaccu (1);
            return true;
        }
#endif
    };
}
#endif
