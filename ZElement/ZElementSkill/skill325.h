#ifndef __CPPGEN_GNET_SKILL325
#define __CPPGEN_GNET_SKILL325
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill325:public Skill
    {
      public:
        enum
        { SKILL_ID = 325 };
          Skill325 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill325Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 2400;
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
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill325Stub ():SkillStub (325)
        {
            occupation = 130;
            name = L"Éå»ê¶áÆÇ";
            nativename = "Éå»ê¶áÆÇ";
            icon = "Éå»ê¶áÆÇ.dds";
            maxlevel = 8;
            maxlearn = 8;
            type = 3;
            eventflag = 1;
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
            action[0] = "ºÏ»¶ÅÉ_ÐÄÁéÕðÉã";
            action[1] = "ºÏ»¶ÅÉ_ÐÄÁéÕðÉã";
            action[2] = "ºÏ»¶ÅÉ_ÐÄÁéÕðÉã";
            action[3] = "ºÏ»¶ÅÉ_ÐÄÁéÕðÉã";
            action[4] = "ºÏ»¶ÅÉ_ÐÄÁéÕðÉã";
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
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_ÐÄÁéÕðÉã";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_ÐÄÁéÕðÉã";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_ÐÄÁéÕðÉã";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_ÐÄÁéÕðÉã";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_ÐÄÁéÕðÉã";
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
        virtual ~ Skill325Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (16);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1.7 * skill->GetLevel () * skill->GetLevel () + 25 * skill->GetLevel () + 220);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
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
            return (float) (11);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (11);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (1.7 * skill->GetLevel () * skill->GetLevel () + 25 * skill->GetLevel () + 220),
                               240 * skill->GetLevel (), 110 + 15 * skill->GetLevel (), 2 + skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetLevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (110 + 15 * skill->GetLevel () + skill->GetPlayer ()->GetLevel ()));
            skill->GetVictim ()->SetTime (2000 + 1000 * skill->GetLevel () + 1000);
            skill->GetVictim ()->SetSilent (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (16000);
            skill->GetVictim ()->SetBuffid (4);
            skill->GetVictim ()->SetAmount (240 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetPlayer ()->GetLevel ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (30));
            skill->GetVictim ()->SetRatio (0.12);
            skill->GetVictim ()->SetXiaoxin (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (22 + skill->GetLevel ());
        }
        bool TakeEffect (Skill * skill) const
        {
            skill->GetPlayer ()->SetPasaddsmite (0.08 * skill->GetLevel ());
            return true;
        }
#endif
    };
}
#endif
