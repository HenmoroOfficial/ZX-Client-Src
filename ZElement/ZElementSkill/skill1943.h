#ifndef __CPPGEN_GNET_SKILL1943
#define __CPPGEN_GNET_SKILL1943
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1943:public Skill
    {
      public:
        enum
        { SKILL_ID = 1943 };
          Skill1943 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1943Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1800;
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
                skill->SetRatio (2.2);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1943Stub ():SkillStub (1943)
        {
            occupation = 151;
            name = L"ÎŞÎª";
            nativename = "ÎŞÎª";
            icon = "¹íÀ÷Ö®²»·².dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 7;
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
            action[0] = "¹íÍõ×Ú_¹íÀ÷²»·²";
            action[1] = "ÇàÔÆÃÅ_¹íÀ÷²»·²";
            action[2] = "¹íÍõ×Ú_¹íÀ÷²»·²";
            action[3] = "ºÏ»¶ÅÉ_¹íÀ÷²»·²";
            action[4] = "ÌìÒôËÂ_¹íÀ÷²»·²";
            action[5] = "¹íµÀ_¹íÀ÷²»·²";
            action[6] = "·ÙÏã_¹íÀ÷²»·²";
            action[7] = "ÊŞÉñ_¹íÀ÷²»·²";
            action[8] = "ºüÑı_¹íÀ÷²»·²";
            action[9] = "ĞùÔ¯_¹íÀ÷²»·²";
            action[10] = "»³¹â_¹íÀ÷²»·²";
            action[11] = "Ì«ê»_¹íÀ÷²»·²";
            action[12] = "Ìì»ª_¹íÀ÷²»·²";
            action[13] = "ÁéÙí_¹íÀ÷²»·²";
            action[14] = "Ó¢ÕĞ_¹íÀ÷²»·²";
            action[15] = "¹íÍõ×Ú_Æï³Ë_¹íÀ÷²»·²";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_¹íÀ÷²»·²";
            action[17] = "¹íÍõ×Ú_Æï³Ë_¹íÀ÷²»·²";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_¹íÀ÷²»·²";
            action[19] = "ÌìÒôËÂ_Æï³Ë_¹íÀ÷²»·²";
            action[20] = "¹íµÀ_Æï³Ë_¹íÀ÷²»·²";
            action[21] = "·ÙÏã_Æï³Ë_¹íÀ÷²»·²";
            action[22] = "ÊŞÉñ_Æï³Ë_¹íÀ÷²»·²";
            action[23] = "ºüÑı_Æï³Ë_¹íÀ÷²»·²";
            action[24] = "ĞùÔ¯_Æï³Ë_¹íÀ÷²»·²";
            action[25] = "»³¹â_Æï³Ë_¹íÀ÷²»·²";
            action[26] = "Ì«ê»_Æï³Ë_¹íÀ÷²»·²";
            action[27] = "Ìì»ª_Æï³Ë_¹íÀ÷²»·²";
            action[28] = "ÁéÙí_Æï³Ë_¹íÀ÷²»·²";
            action[29] = "Ó¢ÕĞ_Æï³Ë_¹íÀ÷²»·²";
            action[30] = "0";
            rangetype = 2;
            doenchant = true;
            dobless = true;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 32;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill1943Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 90000;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (20);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (20);
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
            skill->GetVictim ()->SetProbability (1.0 * (600));
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetDizzy (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (15000);
            skill->GetVictim ()->SetRatio (0.15);
            skill->GetVictim ()->SetInccritrate (1);
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
