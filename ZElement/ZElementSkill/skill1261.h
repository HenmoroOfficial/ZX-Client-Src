#ifndef __CPPGEN_GNET_SKILL1261
#define __CPPGEN_GNET_SKILL1261
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1261:public Skill
    {
      public:
        enum
        { SKILL_ID = 1261 };
          Skill1261 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1261Stub:public SkillStub
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
                return 1200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1261Stub ():SkillStub (1261)
        {
            occupation = 145;
            name = L"Éñ½£ÓùÀ×";
            nativename = "Éñ½£ÓùÀ×";
            icon = "Éñ½£ÓùÀ×Õæ¾÷.dds";
            maxlevel = 1;
            maxlearn = 10;
            type = 1;
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
            action[0] = "ÇàÔÆÃÅ_Éñ½£ÓùÀ×Õæ¾÷";
            action[1] = "ÇàÔÆÃÅ_Éñ½£ÓùÀ×Õæ¾÷";
            action[2] = "ÇàÔÆÃÅ_Éñ½£ÓùÀ×Õæ¾÷";
            action[3] = "ÇàÔÆÃÅ_Éñ½£ÓùÀ×Õæ¾÷";
            action[4] = "ÇàÔÆÃÅ_Éñ½£ÓùÀ×Õæ¾÷";
            action[5] = "ÇàÔÆÃÅ_Éñ½£ÓùÀ×Õæ¾÷";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_Éñ½£ÓùÀ×Õæ¾÷";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_Éñ½£ÓùÀ×Õæ¾÷";
            action[17] = "ÇàÔÆÃÅ_Æï³Ë_Éñ½£ÓùÀ×Õæ¾÷";
            action[18] = "ÇàÔÆÃÅ_Æï³Ë_Éñ½£ÓùÀ×Õæ¾÷";
            action[19] = "ÇàÔÆÃÅ_Æï³Ë_Éñ½£ÓùÀ×Õæ¾÷";
            action[20] = "ÇàÔÆÃÅ_Æï³Ë_Éñ½£ÓùÀ×Õæ¾÷";
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
        virtual ~ Skill1261Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (900);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 60000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (26);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (18);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, INT (900));

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
            skill->GetVictim ()->SetProbability (1.0 * (15));
            skill->GetVictim ()->SetTime (3000);
            skill->GetVictim ()->SetDiet (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0.25 * skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetValue (0.25 * skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetHpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (3000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (3 * skill->GetPlayer ()->GetLevel ());
            skill->GetVictim ()->SetSubdefence (1);
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
