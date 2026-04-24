#ifndef __CPPGEN_GNET_SKILL3306
#define __CPPGEN_GNET_SKILL3306
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3306:public Skill
    {
      public:
        enum
        { SKILL_ID = 3306 };
          Skill3306 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3306Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3306Stub ():SkillStub (3306)
        {
            occupation = 128;
            name = L"´óèó°ãÈô";
            nativename = "´óèó°ãÈô";
            icon = "´ó·²°ãÈô.dds";
            maxlevel = 1;
            maxlearn = 1;
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
            skill_class = -2;
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
            preskillnum = 0;
            action[0] = "ÌìÒôËÂ_´ó·²°ãÈô";
            action[1] = "ÌìÒôËÂ_´ó·²°ãÈô";
            action[2] = "ÌìÒôËÂ_´ó·²°ãÈô";
            action[3] = "ÌìÒôËÂ_´ó·²°ãÈô";
            action[4] = "ÌìÒôËÂ_´ó·²°ãÈô";
            action[5] = "ÌìÒôËÂ_´ó·²°ãÈô";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_´ó·²°ãÈô";
            action[16] = "ÌìÒôËÂ_Æï³Ë_´ó·²°ãÈô";
            action[17] = "ÌìÒôËÂ_Æï³Ë_´ó·²°ãÈô";
            action[18] = "ÌìÒôËÂ_Æï³Ë_´ó·²°ãÈô";
            action[19] = "ÌìÒôËÂ_Æï³Ë_´ó·²°ãÈô";
            action[20] = "ÌìÒôËÂ_Æï³Ë_´ó·²°ãÈô";
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
            rangetype = 5;
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
        virtual ~ Skill3306Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (500);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 180000 - 18000 * 3;
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
            return _snwprintf (buffer, length, format, 500, 20, 2, 3 * 5, 3 * 5);

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (20000 + 2000 * 3 + 100);
            skill->GetVictim ()->SetRatio (0.03 * 5 + 0.02 * 3);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0.03 * 5 * skill->GetPlayer ()->GetMaxhp () * 10 * (1 + 0.2 * 3) * (1 + 0.1 * 3));
            skill->GetVictim ()->SetBanruo (1);
            skill->GetVictim ()->SetTime (2000 + 100 + 2000 * 3);
            skill->GetVictim ()->SetInvincible (1);
            return true;
        }
#endif
    };
}
#endif
