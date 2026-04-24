#ifndef __CPPGEN_GNET_SKILL2977
#define __CPPGEN_GNET_SKILL2977
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2977:public Skill
    {
      public:
        enum
        { SKILL_ID = 2977 };
          Skill2977 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2977Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
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
      Skill2977Stub ():SkillStub (2977)
        {
            occupation = 151;
            name = L"¼ÑÆÚ";
            nativename = "¼ÑÆÚ";
            icon = "ÓÀ½áÍ¬ÐÄ.dds";
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
            action[0] = "¹íÍõ×Ú_È¼ÉÕ";
            action[1] = "ÇàÔÆÃÅ_È¼ÉÕ";
            action[2] = "¹íÍõ×Ú_È¼ÉÕ";
            action[3] = "ºÏ»¶ÅÉ_È¼ÉÕ";
            action[4] = "ÌìÒôËÂ_È¼ÉÕ";
            action[5] = "¹íµÀ_È¼ÉÕ";
            action[6] = "¹íÍõ×Ú_È¼ÉÕ";
            action[7] = "ÊÞÉñ_È¼ÉÕ";
            action[8] = "ºüÑý_È¼ÉÕ";
            action[9] = "";
            action[10] = "»³¹â_È¼ÉÕ";
            action[11] = "ÊÞÉñ_È¼ÉÕ";
            action[12] = "Ìì»ª_È¼ÉÕ";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_È¼ÉÕ";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_È¼ÉÕ";
            action[17] = "¹íÍõ×Ú_Æï³Ë_È¼ÉÕ";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_È¼ÉÕ";
            action[19] = "ÌìÒôËÂ_Æï³Ë_È¼ÉÕ";
            action[20] = "¹íµÀ_Æï³Ë_È¼ÉÕ";
            action[21] = "¹íÍõ×Ú_Æï³Ë_È¼ÉÕ";
            action[22] = "ÊÞÉñ_Æï³Ë_È¼ÉÕ";
            action[23] = "ºüÑý_Æï³Ë_È¼ÉÕ";
            action[24] = "";
            action[25] = "»³¹â_Æï³Ë_È¼ÉÕ";
            action[26] = "ÊÞÉñ_Æï³Ë_È¼ÉÕ";
            action[27] = "Ìì»ª_Æï³Ë_È¼ÉÕ";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
            doenchant = true;
            dobless = true;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 23;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill2977Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (22);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (77);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 600000;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (16);
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
            return _snwprintf (buffer, length, format, 77);

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (25200000);
            skill->GetVictim ()->SetRatio (0.05);
            skill->GetVictim ()->SetBuffid (8);
            skill->GetVictim ()->SetInccrithurt (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (25200000);
            skill->GetVictim ()->SetRatio (0.1);
            skill->GetVictim ()->SetBuffid (8);
            skill->GetVictim ()->SetInccrithurt (1);
            return true;
        }
#endif
    };
}
#endif
