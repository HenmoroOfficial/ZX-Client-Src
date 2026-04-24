#ifndef __CPPGEN_GNET_SKILL2050
#define __CPPGEN_GNET_SKILL2050
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2050:public Skill
    {
      public:
        enum
        { SKILL_ID = 2050 };
          Skill2050 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2050Stub:public SkillStub
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
      Skill2050Stub ():SkillStub (2050)
        {
            occupation = 148;
            name = L"ËÄ´ó½Ô¿ÕII";
            nativename = "ËÄ´ó½Ô¿ÕII";
            icon = "ËÄ´ó½Ô¿ÕII.dds";
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
            talent[0] = 382;
            talent[1] = 383;
            talent[2] = 384;
            talent[3] = 385;
            talent_size = 4;
            action[0] = "ÌìÒôËÂ_ËÄ´ó½Ô¿Õ»ð";
            action[1] = "ÌìÒôËÂ_ËÄ´ó½Ô¿Õ»ð";
            action[2] = "ÌìÒôËÂ_ËÄ´ó½Ô¿Õ»ð";
            action[3] = "ÌìÒôËÂ_ËÄ´ó½Ô¿Õ»ð";
            action[4] = "ÌìÒôËÂ_ËÄ´ó½Ô¿Õ»ð";
            action[5] = "ÌìÒôËÂ_ËÄ´ó½Ô¿Õ»ð";
            action[6] = "ÌìÒôËÂ_ËÄ´ó½Ô¿Õ»ð";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_ËÄ´ó½Ô¿Õ»ð";
            action[16] = "ÌìÒôËÂ_Æï³Ë_ËÄ´ó½Ô¿Õ»ð";
            action[17] = "ÌìÒôËÂ_Æï³Ë_ËÄ´ó½Ô¿Õ»ð";
            action[18] = "ÌìÒôËÂ_Æï³Ë_ËÄ´ó½Ô¿Õ»ð";
            action[19] = "ÌìÒôËÂ_Æï³Ë_ËÄ´ó½Ô¿Õ»ð";
            action[20] = "ÌìÒôËÂ_Æï³Ë_ËÄ´ó½Ô¿Õ»ð";
            action[21] = "ÌìÒôËÂ_Æï³Ë_ËÄ´ó½Ô¿Õ»ð";
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
        virtual ~ Skill2050Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 382 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 8 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (750);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 60000 * 2;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 125, 125, 125, 125, 125, 127, 129, 131, 133, 135 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        int GetCreditCost (int level) const
        {
            static int array[10] = { 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 38000, 45000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 750, 300 - 3 * skill->GetLevel () * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (1000 * (skill->GetT0 () + skill->GetT1 () + skill->GetT2 () + skill->GetT3 ()) + 200);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (300 - 3 * skill->GetLevel () * skill->GetLevel ());
            skill->GetVictim ()->SetSubanti (1);
            skill->GetVictim ()->SetTime (1000 * (skill->GetT0 () + skill->GetT1 () + skill->GetT2 () + skill->GetT3 ()) + 200);
            skill->GetVictim ()->SetImmunesleep (1);
            return true;
        }
#endif
    };
}
#endif
