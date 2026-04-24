#ifndef __CPPGEN_GNET_SKILL1541
#define __CPPGEN_GNET_SKILL1541
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1541:public Skill
    {
      public:
        enum
        { SKILL_ID = 1541 };
          Skill1541 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1541Stub:public SkillStub
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
                skill->SetRatio (1);
                skill->SetPlus ((0.1 + 0.06 * skill->GetLevel ()) * skill->GetPlayer ()->GetHp ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1541Stub ():SkillStub (1541)
        {
            occupation = 148;
            name = L"Í¬¹é¼«ÀÖII";
            nativename = "Í¬¹é¼«ÀÖII";
            icon = "Í¬¹é¼«ÀÖII.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
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
            skill_class = 3;
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
            action[0] = "ÌìÒôËÂ_Í¬¹é¼«ÀÖ";
            action[1] = "ÌìÒôËÂ_Í¬¹é¼«ÀÖ";
            action[2] = "ÌìÒôËÂ_Í¬¹é¼«ÀÖ";
            action[3] = "ÌìÒôËÂ_Í¬¹é¼«ÀÖ";
            action[4] = "ÌìÒôËÂ_Í¬¹é¼«ÀÖ";
            action[5] = "ÌìÒôËÂ_Í¬¹é¼«ÀÖ";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_Í¬¹é¼«ÀÖ";
            action[16] = "ÌìÒôËÂ_Æï³Ë_Í¬¹é¼«ÀÖ";
            action[17] = "ÌìÒôËÂ_Æï³Ë_Í¬¹é¼«ÀÖ";
            action[18] = "ÌìÒôËÂ_Æï³Ë_Í¬¹é¼«ÀÖ";
            action[19] = "ÌìÒôËÂ_Æï³Ë_Í¬¹é¼«ÀÖ";
            action[20] = "ÌìÒôËÂ_Æï³Ë_Í¬¹é¼«ÀÖ";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
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
        virtual ~ Skill1541Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 275 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 9 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (20 + skill->GetLevel ());
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (2260 + 60 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 24000 + 12000 * skill->GetLevel ();
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
            return (float) (16 + skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (13 + skill->GetLevel ());
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
                               INT (2260 + 60 * skill->GetLevel ()),
                               10 + 6 * skill->GetLevel (), 2 + skill->GetLevel (), 24 + 12 * skill->GetLevel (), 13 + skill->GetLevel ());

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
            skill->GetVictim ()->SetBreakcasting (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (2000 + 1000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetDiet (1);
            return true;
        }
#endif
    };
}
#endif
