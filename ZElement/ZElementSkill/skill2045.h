#ifndef __CPPGEN_GNET_SKILL2045
#define __CPPGEN_GNET_SKILL2045
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2045:public Skill
    {
      public:
        enum
        { SKILL_ID = 2045 };
          Skill2045 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2045Stub:public SkillStub
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes3 ());
                skill->SetRatio (1 + 0.02 * skill->GetLevel () +
                                 0.20 * INT (skill->GetPlayer ()->GetMp () / (0.95 - 0.1 * skill->GetT1 ()) / skill->GetPlayer ()->GetMaxmp ()));
                skill->SetPlus (0.01 * skill->GetLevel () * skill->GetPlayer ()->GetMaxmp ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2045Stub ():SkillStub (2045)
        {
            occupation = 148;
            name = L"天诛剑气II";
            nativename = "天诛剑气II";
            icon = "天诛剑气II.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 1;
            eventflag = 1;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 9;
            clearmask = 129;
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
            talent[0] = 541;
            talent[1] = 612;
            talent_size = 2;
            action[0] = "青云门_天诛剑气";
            action[1] = "青云门_天诛剑气";
            action[2] = "青云门_天诛剑气";
            action[3] = "青云门_天诛剑气";
            action[4] = "青云门_天诛剑气";
            action[5] = "青云门_天诛剑气";
            action[6] = "青云门_天诛剑气";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "青云门_骑乘_天诛剑气";
            action[16] = "青云门_骑乘_天诛剑气";
            action[17] = "青云门_骑乘_天诛剑气";
            action[18] = "青云门_骑乘_天诛剑气";
            action[19] = "青云门_骑乘_天诛剑气";
            action[20] = "青云门_骑乘_天诛剑气";
            action[21] = "青云门_骑乘_天诛剑气";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
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
        virtual ~ Skill2045Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 541 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 9 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (30);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (100 * skill->GetLevel () + 213);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 60000;
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
            return (float) (6);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (20);
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
                               100 * skill->GetLevel () + 213,
                               20, 2 * skill->GetLevel (), skill->GetLevel (), 30 + 10 * skill->GetLevel (), 5 + skill->GetLevel (), 10, 1);

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
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetVar1 () + 30 + 10 * skill->GetLevel () +
                                                  10 * int (skill->GetT0 () / 2.99)));
            skill->GetVictim ()->SetTime (5000 + 1000 * skill->GetLevel () + 1000 * int (skill->GetT0 () / 2.99));
              skill->GetVictim ()->SetWrap (1);
              return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetBuffid (INT (skill->GetPlayer ()->GetMp () / (0.95 - 0.1 * skill->GetT1 ()) / skill->GetPlayer ()->GetMaxmp ()));
            skill->GetVictim ()->SetUniqprompt (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (20);
        }
#endif
    };
}
#endif
