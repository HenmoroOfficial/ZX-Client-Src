#ifndef __CPPGEN_GNET_SKILL2964
#define __CPPGEN_GNET_SKILL2964
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2964:public Skill
    {
      public:
        enum
        { SKILL_ID = 2964 };
          Skill2964 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2964Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 0;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2964Stub ():SkillStub (2964)
        {
            occupation = 128;
            name = L"Ììî¸";
            nativename = "Ììî¸";
            icon = "´Èº½·¨Ô¸.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 2;
            eventflag = 0;
            spcost = 0;
            timetype = 1;
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
            action[0] = "ÌìÒôËÂ_´Èº½·¨Ô¸";
            action[1] = "ÌìÒôËÂ_´Èº½·¨Ô¸";
            action[2] = "ÌìÒôËÂ_´Èº½·¨Ô¸";
            action[3] = "ÌìÒôËÂ_´Èº½·¨Ô¸";
            action[4] = "ÌìÒôËÂ_´Èº½·¨Ô¸";
            action[5] = "ÌìÒôËÂ_´Èº½·¨Ô¸";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_´Èº½·¨Ô¸";
            action[16] = "ÌìÒôËÂ_Æï³Ë_´Èº½·¨Ô¸";
            action[17] = "ÌìÒôËÂ_Æï³Ë_´Èº½·¨Ô¸";
            action[18] = "ÌìÒôËÂ_Æï³Ë_´Èº½·¨Ô¸";
            action[19] = "ÌìÒôËÂ_Æï³Ë_´Èº½·¨Ô¸";
            action[20] = "ÌìÒôËÂ_Æï³Ë_´Èº½·¨Ô¸";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "";
            rangetype = 2;
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
#endif
        }
        virtual ~ Skill2964Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1000);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 0;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 12000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[1] = { 0 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (20);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (18);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
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
            skill->GetVictim ()->SetTime (8000 + 100);
            skill->GetVictim ()->SetRatio (0.2);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetIncskilldamage (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (8000);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetIncdefence (1);
            skill->GetVictim ()->SetTime (8000);
            skill->GetVictim ()->SetRatio (0.2);
            skill->GetVictim ()->SetInccritrate (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (3600000);
            skill->GetVictim ()->SetAmount (21);
            skill->GetVictim ()->SetHolyaura (1);
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
