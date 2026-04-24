#ifndef __CPPGEN_GNET_SKILL1542
#define __CPPGEN_GNET_SKILL1542
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1542:public Skill
    {
      public:
        enum
        { SKILL_ID = 1542 };
          Skill1542 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1542Stub:public SkillStub
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
      Skill1542Stub ():SkillStub (1542)
        {
            occupation = 148;
            name = L"´Èº½·¨Ô¸II";
            nativename = "´Èº½·¨Ô¸II";
            icon = "¼ÅÃğ±ÎÈÕII.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 2;
            eventflag = 0;
            spcost = 0;
            timetype = 1;
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
            action[0] = "ÌìÒôËÂ_´Èº½·¨Ô¸";
            action[1] = "ÌìÒôËÂ_´Èº½·¨Ô¸";
            action[2] = "ÌìÒôËÂ_´Èº½·¨Ô¸";
            action[3] = "ÌìÒôËÂ_´Èº½·¨Ô¸";
            action[4] = "ÌìÒôËÂ_´Èº½·¨Ô¸";
            action[5] = "ÌìÒôËÂ_´Èº½·¨Ô¸";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_´Èº½·¨Ô¸";
            action[16] = "ÌìÒôËÂ_Æï³Ë_´Èº½·¨Ô¸";
            action[17] = "ÌìÒôËÂ_Æï³Ë_´Èº½·¨Ô¸";
            action[18] = "ÌìÒôËÂ_Æï³Ë_´Èº½·¨Ô¸";
            action[19] = "ÌìÒôËÂ_Æï³Ë_´Èº½·¨Ô¸";
            action[20] = "ÌìÒôËÂ_Æï³Ë_´Èº½·¨Ô¸";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
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
        virtual ~ Skill1542Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 553 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 9 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (595 - 5 * skill->GetLevel ());
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
            static int array[10] = { 125, 125, 125, 125, 125, 127, 129, 131, 133, 135 };
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
        int GetCreditCost (int level) const
        {
            static int array[10] = { 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 38000, 45000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 2 * skill->GetLevel (), skill->GetLevel (), 595 - 5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (8000 + 100);
            skill->GetVictim ()->SetRatio (0.0201 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetDecskillaccu (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (8000 + 100);
            skill->GetVictim ()->SetRatio (0.0101 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetDecskilldodge (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (3600000);
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetAmount (21);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetEvilaura (1);
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
