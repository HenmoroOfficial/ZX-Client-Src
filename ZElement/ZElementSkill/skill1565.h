#ifndef __CPPGEN_GNET_SKILL1565
#define __CPPGEN_GNET_SKILL1565
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1565:public Skill
    {
      public:
        enum
        { SKILL_ID = 1565 };
          Skill1565 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1565Stub:public SkillStub
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
                skill->SetCrithurt (0.15 * skill->GetT0 ());
                skill->SetRatio (1 + 0.14 * skill->GetLevel () + 0.04 * skill->GetLevel () * skill->GetT0 ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1565Stub ():SkillStub (1565)
        {
            occupation = 149;
            name = L"真元华闪II";
            nativename = "真元华闪II";
            icon = "真元华闪II.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
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
            talent[0] = 598;
            talent_size = 1;
            action[0] = "青云门_真元华闪";
            action[1] = "青云门_真元华闪";
            action[2] = "青云门_真元华闪";
            action[3] = "青云门_真元华闪";
            action[4] = "青云门_真元华闪";
            action[5] = "青云门_真元华闪";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "青云门_骑乘_真元华闪";
            action[16] = "青云门_骑乘_真元华闪";
            action[17] = "青云门_骑乘_真元华闪";
            action[18] = "青云门_骑乘_真元华闪";
            action[19] = "青云门_骑乘_真元华闪";
            action[20] = "青云门_骑乘_真元华闪";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
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
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill1565Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 542 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 7 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (22 + skill->GetLevel ());
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1950 + 25 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return (380 - 20 * skill->GetLevel ()) * 1000;
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
        float GetPraydistance (Skill * skill) const
        {
            return (float) (17 + skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (11 + skill->GetLevel ());
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
                               1950 + 25 * skill->GetLevel (), 11 + skill->GetLevel (), 14 * skill->GetLevel (), 380 - 20 * skill->GetLevel ());

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
            skill->GetVictim ()->SetRatio ((1 + 0.2 * (skill->GetPlayer ()->GetMaxhp () + 5000) / (skill->GetPlayer ()->GetHp () + 5000)) *
                                           (1 + skill->GetPlayer ()->GetBuffcnt () * 0.02 * skill->GetT0 ()));
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetSecondattack (1);
            return true;
        }
#endif
    };
}
#endif
