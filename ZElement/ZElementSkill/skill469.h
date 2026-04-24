#ifndef __CPPGEN_GNET_SKILL469
#define __CPPGEN_GNET_SKILL469
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill469:public Skill
    {
      public:
        enum
        { SKILL_ID = 469 };
          Skill469 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill469Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1100;
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
                return 900;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill469Stub ():SkillStub (469)
        {
            occupation = 128;
            name = L"²âÊÔÖĞ¼¼ÄÜ2";
            nativename = "²âÊÔÖĞ¼¼ÄÜ2";
            icon = "×··çÖä.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 3;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
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
            action[0] = "ºÏ»¶ÅÉ_ÃÔÀë";
            action[1] = "ºÏ»¶ÅÉ_ÃÔÀë";
            action[2] = "ºÏ»¶ÅÉ_ÃÔÀë";
            action[3] = "ºÏ»¶ÅÉ_ÃÔÀë";
            action[4] = "ºÏ»¶ÅÉ_ÃÔÀë";
            action[5] = "0";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_ÃÔÀë";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_ÃÔÀë";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_ÃÔÀë";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_ÃÔÀë";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_ÃÔÀë";
            action[20] = "0";
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
        virtual ~ Skill469Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 0;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (7);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
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
            skill->GetVictim ()->SetProbability (1.0 * (50));
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetSleep (1);
            skill->GetVictim ()->SetProbability (1.0 * (-2));
            skill->GetVictim ()->SetTime (8000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (800);
            skill->GetVictim ()->SetValue (400);
            skill->GetVictim ()->SetHpleak (1);
            return true;
        }
#endif
    };
}
#endif
