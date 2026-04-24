#ifndef __CPPGEN_GNET_SKILL1911
#define __CPPGEN_GNET_SKILL1911
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1911:public Skill
    {
      public:
        enum
        { SKILL_ID = 1911 };
          Skill1911 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1911Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 667;
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
                return 2000;
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
                return 333;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrithurt ((skill->GetCharging () > 250 ? skill->GetCharging () * 0.0005 * (0.05 * skill->GetLevel ()) : 0));
                skill->SetCrit ((skill->GetCharging () > 250 ? skill->GetCharging () * 0.0005 * (0.01 * skill->GetLevel ()) : 0));
                skill->SetRatio (1 + 0.01 * (2 + skill->GetLevel ()) +
                                 (skill->GetCharging () > 250 ? skill->GetCharging () * 0.0005 * (0.5 + 0.05 * skill->GetLevel ()) : 0));
                skill->SetPlus (496 + 19 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1911Stub ():SkillStub (1911)
        {
            occupation = 42;
            name = L"ÕòÁú»÷";
            nativename = "ÕòÁú»÷";
            icon = "ÕòÁú»÷.dds";
            maxlevel = 12;
            maxlearn = 8;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 3;
            targettype = 0;
            autoattack = 1;
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
            skill_class = 6;
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
            talent[0] = 2153;
            talent[1] = 2157;
            talent_size = 2;
            action[0] = "ºüÑý_ÕòÁú»÷";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "ºüÑý_ÕòÁú»÷";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ºüÑý_Æï³Ë_ÕòÁú»÷";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "ºüÑý_Æï³Ë_ÕòÁú»÷";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = 8;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
            statestub.push_back (new State3 ());
#endif
        }
        virtual ~ Skill1911Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1910 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (40);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (637);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 30000 - 5000 * skill->GetT0 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[8] = { 110, 112, 114, 116, 118, 120, 122, 124 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (30);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (25);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               2 + skill->GetLevel (), 496 + 19 * skill->GetLevel (), 20 + 5 * skill->GetLevel (), 50 + 5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (20 + 5 * skill->GetLevel () + 5 * skill->GetT1 ()));
            skill->GetVictim ()->SetAmount (1);
            skill->GetVictim ()->SetClearbuff (1);
            return true;
        }
#endif
    };
}
#endif
