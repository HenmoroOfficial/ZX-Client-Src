#ifndef __CPPGEN_GNET_SKILL2386
#define __CPPGEN_GNET_SKILL2386
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2386:public Skill
    {
      public:
        enum
        { SKILL_ID = 2386 };
          Skill2386 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2386Stub:public SkillStub
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
      Skill2386Stub ():SkillStub (2386)
        {
            occupation = 129;
            name = L"µãÊ¯³É½ð";
            nativename = "µãÊ¯³É½ð";
            icon = "½ð×©.dds";
            maxlevel = 99;
            maxlearn = 99;
            type = 3;
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
            action[0] = "¹íÍõ×Ú_ÕÕÑý";
            action[1] = "ÇàÔÆÃÅ_ÕÕÑý";
            action[2] = "¹íÍõ×Ú_ÕÕÑý";
            action[3] = "ºÏ»¶ÅÉ_ÕÕÑý";
            action[4] = "ÌìÒôËÂ_ÕÕÑý";
            action[5] = "¹íµÀ_ÕÕÑý";
            action[6] = "0";
            action[7] = "ÊÞÉñ_ÕÕÑý";
            action[8] = "ºüÑý_ÕÕÑý";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_ÕÕÑý";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_ÕÕÑý";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ÕÕÑý";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_ÕÕÑý";
            action[19] = "ÌìÒôËÂ_Æï³Ë_ÕÕÑý";
            action[20] = "¹íµÀ_Æï³Ë_ÕÕÑý";
            action[21] = "0";
            action[22] = "ÊÞÉñ_Æï³Ë_ÕÕÑý";
            action[23] = "ºüÑý_Æï³Ë_ÕÕÑý";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
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
        virtual ~ Skill2386Stub ()
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
            return (float) (15);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 15000;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (10);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (6);
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
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetValue (171);
            skill->GetVictim ()->SetBackorigin (1);
            return true;
        }
#endif
    };
}
#endif
