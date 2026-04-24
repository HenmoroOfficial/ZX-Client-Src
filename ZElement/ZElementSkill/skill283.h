#ifndef __CPPGEN_GNET_SKILL283
#define __CPPGEN_GNET_SKILL283
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill283:public Skill
    {
      public:
        enum
        { SKILL_ID = 283 };
          Skill283 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill283Stub:public SkillStub
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
      Skill283Stub ():SkillStub (283)
        {
            occupation = 10;
            name = L"´Èº½ÆÕ¶É";
            nativename = "´Èº½ÆÕ¶É";
            icon = "´Èº½ÆÕ¶É.dds";
            maxlevel = 12;
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
            talent[0] = 706;
            talent_size = 1;
            action[0] = "ÌìÒôËÂ_´Èº½ÆÕ¶È";
            action[1] = "ÌìÒôËÂ_´Èº½ÆÕ¶È";
            action[2] = "ÌìÒôËÂ_´Èº½ÆÕ¶È";
            action[3] = "ÌìÒôËÂ_´Èº½ÆÕ¶È";
            action[4] = "ÌìÒôËÂ_´Èº½ÆÕ¶È";
            action[5] = "ÌìÒôËÂ_´Èº½ÆÕ¶È";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_´Èº½ÆÕ¶È";
            action[16] = "ÌìÒôËÂ_Æï³Ë_´Èº½ÆÕ¶È";
            action[17] = "ÌìÒôËÂ_Æï³Ë_´Èº½ÆÕ¶È";
            action[18] = "ÌìÒôËÂ_Æï³Ë_´Èº½ÆÕ¶È";
            action[19] = "ÌìÒôËÂ_Æï³Ë_´Èº½ÆÕ¶È";
            action[20] = "ÌìÒôËÂ_Æï³Ë_´Èº½ÆÕ¶È";
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
        virtual ~ Skill283Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 268 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 4 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0.7 * skill->GetLevel () * skill->GetLevel () + 85 * skill->GetLevel () + 227);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 2000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[5] = { 35, 40, 45, 49, 52 };
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
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (0.7 * skill->GetLevel () * skill->GetLevel () + 85 * skill->GetLevel () + 227), 30, 18, skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (1800000 + 900000 * skill->GetT0 ());
            skill->GetVictim ()->SetRatio (0.01 * skill->GetLevel () + 0.01 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetIncdefence (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (40);
        }
#endif
    };
}
#endif
