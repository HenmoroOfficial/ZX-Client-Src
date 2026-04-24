#ifndef __CPPGEN_GNET_SKILL4351
#define __CPPGEN_GNET_SKILL4351
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4351:public Skill
    {
      public:
        enum
        { SKILL_ID = 4351 };
          Skill4351 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4351Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 500;
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
                return 500;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetPlus (2000 + skill->GetPlayer ()->GetMaxmp () * 0.02 * skill->GetLevel ());
                skill->SetCrithurt (0.05 * skill->GetT1 ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4351Stub ():SkillStub (4351)
        {
            occupation = 60;
            name = L"";
            nativename = "";
            icon = "";
            maxlevel = 6;
            maxlearn = 0;
            type = 11;
            eventflag = 0;
            spcost = 0;
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
            skill_class = 11;
            guide = false;
            summon_id = 0;
            trigger = true;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 4352;
            targetcnt = 6;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 0;
            talent[0] = 4142;
            talent[1] = 4148;
            talent_size = 2;
            action[0] = "ĞùÔ¯_ÈÕÑ×Êõ_02";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "ĞùÔ¯_ÈÕÑ×Êõ_02";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "ĞùÔ¯_Æï³Ë_ÈÕÑ×Êõ_02";
            action[14] = "0";
            action[15] = "0";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "ĞùÔ¯_Æï³Ë_ÈÕÑ×Êõ_02";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            rangetype = 11;
            doenchant = true;
            dobless = true;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = 9;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill4351Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 0;
        }
        float GetAttackdistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (17);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (15);
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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetT0 () > 0 ? 120 : 0));
            skill->GetVictim ()->SetTime (2000);
            skill->GetVictim ()->SetRatio (0.01 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetIncfatalratio (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (2000);
            skill->GetVictim ()->SetAmount (skill->GetLevel ());
            skill->GetVictim ()->SetValue (4352);
            skill->GetVictim ()->SetTriggerskill (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (1000);
            skill->GetVictim ()->SetPowerless (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (20);
        }
#endif
    };
}
#endif
