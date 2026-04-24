#ifndef __CPPGEN_GNET_SKILL4759
#define __CPPGEN_GNET_SKILL4759
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4759:public Skill
    {
      public:
        enum
        { SKILL_ID = 4759 };
          Skill4759 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4759Stub:public SkillStub
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4759Stub ():SkillStub (4759)
        {
            occupation = 128;
            name = L"ÃÆ¹÷";
            nativename = "ÃÆ¹÷";
            icon = "Ò»»÷.dds";
            maxlevel = 1;
            maxlearn = 1;
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
            action[0] = "¹íÍõ×Ú_¾£¼¬";
            action[1] = "ÇàÔÆÃÅ_¾£¼¬";
            action[2] = "¹íÍõ×Ú_¾£¼¬";
            action[3] = "ºÏ»¶ÅÉ_¾£¼¬";
            action[4] = "ÌìÒôËÂ_¾£¼¬";
            action[5] = "¹íµÀ_¾£¼¬";
            action[6] = "¹íÍõ×Ú_¾£¼¬";
            action[7] = "ÊŞÉñ_¾£¼¬";
            action[8] = "ºüÑı_¾£¼¬";
            action[9] = "ĞùÔ¯_¾£¼¬";
            action[10] = "»³¹â_¾£¼¬";
            action[11] = "ÊŞÉñ_¾£¼¬";
            action[12] = "Ìì»ª_¾£¼¬";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_¾£¼¬";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_¾£¼¬";
            action[17] = "¹íÍõ×Ú_Æï³Ë_¾£¼¬";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_¾£¼¬";
            action[19] = "ÌìÒôËÂ_Æï³Ë_¾£¼¬";
            action[20] = "¹íµÀ_Æï³Ë_¾£¼¬";
            action[21] = "¹íÍõ×Ú_Æï³Ë_¾£¼¬";
            action[22] = "ÊŞÉñ_Æï³Ë_¾£¼¬";
            action[23] = "ºüÑı_Æï³Ë_¾£¼¬";
            action[24] = "ĞùÔ¯_Æï³Ë_¾£¼¬";
            action[25] = "»³¹â_Æï³Ë_¾£¼¬";
            action[26] = "ÊŞÉñ_Æï³Ë_¾£¼¬";
            action[27] = "Ìì»ª_Æï³Ë_¾£¼¬";
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
            skill_limit = 30;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill4759Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (8);
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
            return 1000;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (8);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (7);
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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetTmplid () == 57855 ? 100 : 0));
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetMaxhp () * 0.1 + zrand (100));
            skill->GetVictim ()->SetDirecthurt (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (10);
        }
#endif
    };
}
#endif
