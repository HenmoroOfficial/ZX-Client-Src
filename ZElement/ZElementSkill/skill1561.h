#ifndef __CPPGEN_GNET_SKILL1561
#define __CPPGEN_GNET_SKILL1561
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1561:public Skill
    {
      public:
        enum
        { SKILL_ID = 1561 };
          Skill1561 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1561Stub:public SkillStub
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
                skill->SetRatio (1 + 0.03 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1561Stub ():SkillStub (1561)
        {
            occupation = 149;
            name = L"É½ÃËII";
            nativename = "É½ÃËII";
            icon = "É½ÃËII.dds";
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
            skill_class = 1;
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
            talent[0] = 324;
            talent_size = 1;
            action[0] = "ºÏ»¶ÅÉ_É½ÃË";
            action[1] = "ºÏ»¶ÅÉ_É½ÃË";
            action[2] = "ºÏ»¶ÅÉ_É½ÃË";
            action[3] = "ºÏ»¶ÅÉ_É½ÃË";
            action[4] = "ºÏ»¶ÅÉ_É½ÃË";
            action[5] = "ºÏ»¶ÅÉ_É½ÃË";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_É½ÃË";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_É½ÃË";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_É½ÃË";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_É½ÃË";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_É½ÃË";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_É½ÃË";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "¼¼ÄÜ¹¥»÷1";
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
        virtual ~ Skill1561Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 324 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 9 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (950 + 30 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
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
        float GetPraydistance (Skill * skill) const
        {
            return (float) (7);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
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
                               950 + 30 * skill->GetLevel (),
                               3 * skill->GetLevel (), 2 * skill->GetLevel (), 5 * skill->GetLevel (), skill->GetLevel (), 1);

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
            skill->GetVictim ()->SetTime (100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (0.02 * skill->GetLevel () * skill->GetPlayer ()->GetMaxhp () + skill->GetPlayer ()->GetDef ());
            skill->GetVictim ()->SetBleeding (1);
            skill->GetVictim ()->SetProbability (1.0 * (5 * skill->GetLevel () + skill->GetLevel () * int (skill->GetT0 () / 2.99)));
            skill->GetVictim ()->SetTime (4100 + 1000 * int (skill->GetT0 () / 2.99));
              skill->GetVictim ()->SetDiet (1);
              return true;
        }
#endif
    };
}
#endif
