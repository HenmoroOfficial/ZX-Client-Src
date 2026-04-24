#ifndef __CPPGEN_GNET_SKILL1515
#define __CPPGEN_GNET_SKILL1515
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1515:public Skill
    {
      public:
        enum
        { SKILL_ID = 1515 };
          Skill1515 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1515Stub:public SkillStub
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
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1515Stub ():SkillStub (1515)
        {
            occupation = 129;
            name = L"·²³¾Îï»ª";
            nativename = "·²³¾Îï»ª";
            icon = "»Ø»ê¼À.dds";
            maxlevel = 1;
            maxlearn = 10;
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
            action[0] = "¹íÍõ×Ú_ÎÞ¼ä¼Å";
            action[1] = "¹íÍõ×Ú_ÎÞ¼ä¼Å";
            action[2] = "¹íÍõ×Ú_ÎÞ¼ä¼Å";
            action[3] = "¹íÍõ×Ú_ÎÞ¼ä¼Å";
            action[4] = "¹íÍõ×Ú_ÎÞ¼ä¼Å";
            action[5] = "¹íµÀ_ÌìÍâÌì";
            action[6] = "·ÙÏã¹È_»ðá÷»¤Ö÷";
            action[7] = "ÊÞÉñ_»¢Á¦";
            action[8] = "ºüÑý_°²ÈçÉ½";
            action[9] = "ÐùÔ¯_ÄÜ»·";
            action[10] = "»³¹â_ÍµÏ®";
            action[11] = "Ì«ê»_¼ÓËÙ";
            action[12] = "Ìì»ª_±¼À×";
            action[13] = "ÁéÙí_¼á¶Ü";
            action[14] = "Ó¢ÕÐ_¼á¶Ü";
            action[15] = "¹íÍõ×Ú_Æï³Ë_ÎÞ¼ä¼Å";
            action[16] = "¹íÍõ×Ú_Æï³Ë_ÎÞ¼ä¼Å";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ÎÞ¼ä¼Å";
            action[18] = "¹íÍõ×Ú_Æï³Ë_ÎÞ¼ä¼Å";
            action[19] = "¹íÍõ×Ú_Æï³Ë_ÎÞ¼ä¼Å";
            action[20] = "¹íµÀ_Æï³Ë_ÌìÍâÌì";
            action[21] = "·ÙÏã¹È_Æï³Ë_»ðá÷»¤Ö÷";
            action[22] = "ÊÞÉñ_Æï³Ë_»¢Á¦";
            action[23] = "ºüÑý_Æï³Ë_°²ÈçÉ½";
            action[24] = "ÐùÔ¯_Æï³Ë_ÄÜ»·";
            action[25] = "»³¹â_Æï³Ë_ÍµÏ®";
            action[26] = "Ì«ê»_Æï³Ë_¼ÓËÙ";
            action[27] = "Ìì»ª_Æï³Ë_±¼À×";
            action[28] = "ÁéÙí_Æï³Ë_¼á¶Ü";
            action[29] = "Ó¢ÕÐ_Æï³Ë_¼á¶Ü";
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
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill1515Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (600);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 900000;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (25);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, INT (600), 25, 30, 30, 300);

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
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetRatio (0.3);
            skill->GetVictim ()->SetBuffid (5);
            skill->GetVictim ()->SetDeccritrate (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetRatio (3);
            skill->GetVictim ()->SetBuffid (5);
            skill->GetVictim ()->SetDeccrithurt (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetBarehanded (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (12000);
            skill->GetVictim ()->SetInvincible (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (40);
        }
#endif
    };
}
#endif
