#ifndef __CPPGEN_GNET_SKILL1567
#define __CPPGEN_GNET_SKILL1567
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1567:public Skill
    {
      public:
        enum
        { SKILL_ID = 1567 };
          Skill1567 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1567Stub:public SkillStub
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1567Stub ():SkillStub (1567)
        {
            occupation = 149;
            name = L"·ğ¹âÆÕÕÕII";
            nativename = "·ğ¹âÆÕÕÕII";
            icon = "·ğ¹âÆÕÕÕII.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 3;
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
            talent[0] = 277;
            talent_size = 1;
            action[0] = "ÌìÒôËÂ_·ğ¹âÆÕÕÕ";
            action[1] = "ÌìÒôËÂ_·ğ¹âÆÕÕÕ";
            action[2] = "ÌìÒôËÂ_·ğ¹âÆÕÕÕ";
            action[3] = "ÌìÒôËÂ_·ğ¹âÆÕÕÕ";
            action[4] = "ÌìÒôËÂ_·ğ¹âÆÕÕÕ";
            action[5] = "ÌìÒôËÂ_·ğ¹âÆÕÕÕ";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_·ğ¹âÆÕÕÕ";
            action[16] = "ÌìÒôËÂ_Æï³Ë_·ğ¹âÆÕÕÕ";
            action[17] = "ÌìÒôËÂ_Æï³Ë_·ğ¹âÆÕÕÕ";
            action[18] = "ÌìÒôËÂ_Æï³Ë_·ğ¹âÆÕÕÕ";
            action[19] = "ÌìÒôËÂ_Æï³Ë_·ğ¹âÆÕÕÕ";
            action[20] = "ÌìÒôËÂ_Æï³Ë_·ğ¹âÆÕÕÕ";
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
            rangetype = 2;
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
        virtual ~ Skill1567Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 277 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 12 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (400 * skill->GetLevel ());
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
        float GetRadius (Skill * skill) const
        {
            return (float) (18);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (18);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10);
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
                               400 * skill->GetLevel (), 25, 20, 18, 50 * skill->GetLevel () + 8 * skill->GetLevel () * skill->GetLevel (), 60);

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
            skill->GetVictim ()->SetTime (60000 + 100);
            skill->GetVictim ()->SetRatio ((1 + 0.2 * skill->GetT0 ()) * (50 * skill->GetLevel () + 8 * skill->GetLevel () * skill->GetLevel ()));
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetCrazy (1);
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
