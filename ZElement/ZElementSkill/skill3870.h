#ifndef __CPPGEN_GNET_SKILL3870
#define __CPPGEN_GNET_SKILL3870
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3870:public Skill
    {
      public:
        enum
        { SKILL_ID = 3870 };
          Skill3870 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3870Stub:public SkillStub
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
      Skill3870Stub ():SkillStub (3870)
        {
            occupation = 129;
            name = L"ÐíÔ¸";
            nativename = "ÐíÔ¸";
            icon = "Çéñ².dds";
            maxlevel = 1;
            maxlearn = 1;
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
            action[0] = "¹íÍõ×Ú_³ÕÇéÖä";
            action[1] = "¹íÍõ×Ú_³ÕÇéÖä";
            action[2] = "¹íÍõ×Ú_³ÕÇéÖä";
            action[3] = "¹íÍõ×Ú_³ÕÇéÖä";
            action[4] = "¹íÍõ×Ú_³ÕÇéÖä";
            action[5] = "¹íµÀ_É¢¾¡";
            action[6] = "¹íÍõ×Ú_³ÕÇéÖä";
            action[7] = "ÊÞÉñ_É¢¾¡";
            action[8] = "ºüÑý_É¢¾¡";
            action[9] = "0";
            action[10] = "»³¹â_É¢¾¡";
            action[11] = "ÊÞÉñ_É¢¾¡";
            action[12] = "Ìì»ª_É¢¾¡";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_³ÕÇéÖä";
            action[16] = "¹íÍõ×Ú_Æï³Ë_³ÕÇéÖä";
            action[17] = "¹íÍõ×Ú_Æï³Ë_³ÕÇéÖä";
            action[18] = "¹íÍõ×Ú_Æï³Ë_³ÕÇéÖä";
            action[19] = "¹íÍõ×Ú_Æï³Ë_³ÕÇéÖä";
            action[20] = "¹íµÀ_Æï³Ë_É¢¾¡";
            action[21] = "¹íÍõ×Ú_Æï³Ë_³ÕÇéÖä";
            action[22] = "ÊÞÉñ_Æï³Ë_É¢¾¡";
            action[23] = "ºüÑý_Æï³Ë_É¢¾¡";
            action[24] = "0";
            action[25] = "»³¹â_Æï³Ë_É¢¾¡";
            action[26] = "ÊÞÉñ_Æï³Ë_É¢¾¡";
            action[27] = "Ìì»ª_Æï³Ë_É¢¾¡";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
            doenchant = false;
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
        virtual ~ Skill3870Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (10);
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
            return 100;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (18);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (9);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (8);
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
        int GetCoverage (Skill * skill) const
        {
            return (int) (12);
        }
#endif
    };
}
#endif
