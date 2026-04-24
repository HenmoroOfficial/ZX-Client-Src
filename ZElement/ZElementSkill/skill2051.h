#ifndef __CPPGEN_GNET_SKILL2051
#define __CPPGEN_GNET_SKILL2051
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2051:public Skill
    {
      public:
        enum
        { SKILL_ID = 2051 };
          Skill2051 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2051Stub:public SkillStub
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2051Stub ():SkillStub (2051)
        {
            occupation = 148;
            name = L"Ê¨×ÓºğII";
            nativename = "Ê¨×ÓºğII";
            icon = "Ê¨×ÓºğII.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 2;
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
            action[0] = "ÌìÒôËÂ_Ê¨×Óºğ";
            action[1] = "ÌìÒôËÂ_Ê¨×Óºğ";
            action[2] = "ÌìÒôËÂ_Ê¨×Óºğ";
            action[3] = "ÌìÒôËÂ_Ê¨×Óºğ";
            action[4] = "ÌìÒôËÂ_Ê¨×Óºğ";
            action[5] = "ÌìÒôËÂ_Ê¨×Óºğ";
            action[6] = "ÌìÒôËÂ_Ê¨×Óºğ";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_Ê¨×Óºğ";
            action[16] = "ÌìÒôËÂ_Æï³Ë_Ê¨×Óºğ";
            action[17] = "ÌìÒôËÂ_Æï³Ë_Ê¨×Óºğ";
            action[18] = "ÌìÒôËÂ_Æï³Ë_Ê¨×Óºğ";
            action[19] = "ÌìÒôËÂ_Æï³Ë_Ê¨×Óºğ";
            action[20] = "ÌìÒôËÂ_Æï³Ë_Ê¨×Óºğ";
            action[21] = "ÌìÒôËÂ_Æï³Ë_Ê¨×Óºğ";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 4;
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
        virtual ~ Skill2051Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 270 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 9 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (102 * skill->GetLevel () + 99);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 380000 - 20000 * skill->GetLevel ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 125, 125, 125, 125, 125, 127, 129, 131, 133, 135 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (1.5 * skill->GetLevel ());
        }
        float GetAngle (Skill * skill) const
        {
            return (float) (1 - 0.0111111 * (120));
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (5.8 + 0.2 * skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (2.3 + 0.2 * skill->GetLevel ());
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
                               102 * skill->GetLevel () + 99, 1.5 * skill->GetLevel (), 5 + skill->GetLevel (), 380 - 20 * skill->GetLevel ());

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
            skill->GetVictim ()->SetClearsleep (1);
            skill->GetVictim ()->SetTime (5000 + 1000 * skill->GetLevel ());
            skill->GetVictim ()->SetImmunesleep (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (15);
        }
#endif
    };
}
#endif
