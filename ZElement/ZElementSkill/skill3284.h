#ifndef __CPPGEN_GNET_SKILL3284
#define __CPPGEN_GNET_SKILL3284
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3284:public Skill
    {
      public:
        enum
        { SKILL_ID = 3284 };
          Skill3284 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3284Stub:public SkillStub
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
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3284Stub ():SkillStub (3284)
        {
            occupation = 129;
            name = L"Ä°Â··²¸è";
            nativename = "Ä°Â··²¸è";
            icon = "Ä°Â··²¸è.dds";
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
            action[0] = "¹íÍõ×Ú_ÎŞ¼ä¼Å";
            action[1] = "¹íÍõ×Ú_ÎŞ¼ä¼Å";
            action[2] = "¹íÍõ×Ú_ÎŞ¼ä¼Å";
            action[3] = "¹íÍõ×Ú_ÎŞ¼ä¼Å";
            action[4] = "¹íÍõ×Ú_ÎŞ¼ä¼Å";
            action[5] = "¹íµÀ_¼á¶Ü";
            action[6] = "¹íÍõ×Ú_ÎŞ¼ä¼Å";
            action[7] = "ÊŞÉñ_»¢Á¦";
            action[8] = "ºüÑı_°²ÈçÉ½";
            action[9] = "ĞùÔ¯_¼ÓËÙ";
            action[10] = "»³¹â_ÍµÏ®";
            action[11] = "ÊŞÉñ_»¢Á¦";
            action[12] = "Ìì»ª_±¼À×";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_ÎŞ¼ä¼Å";
            action[16] = "¹íÍõ×Ú_Æï³Ë_ÎŞ¼ä¼Å";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ÎŞ¼ä¼Å";
            action[18] = "¹íÍõ×Ú_Æï³Ë_ÎŞ¼ä¼Å";
            action[19] = "¹íÍõ×Ú_Æï³Ë_ÎŞ¼ä¼Å";
            action[20] = "¹íµÀ_Æï³Ë_¼á¶Ü";
            action[21] = "¹íÍõ×Ú_Æï³Ë_ÎŞ¼ä¼Å";
            action[22] = "ÊŞÉñ_Æï³Ë_»¢Á¦";
            action[23] = "ºüÑı_Æï³Ë_°²ÈçÉ½";
            action[24] = "ĞùÔ¯_Æï³Ë_¼ÓËÙ";
            action[25] = "»³¹â_Æï³Ë_ÍµÏ®";
            action[26] = "ÊŞÉñ_Æï³Ë_»¢Á¦";
            action[27] = "Ìì»ª_Æï³Ë_±¼À×";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 5;
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
        virtual ~ Skill3284Stub ()
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
            skill->GetVictim ()->SetRatio (0.3);
            skill->GetVictim ()->SetBuffid (5);
            skill->GetVictim ()->SetDecfatalratio (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetRatio (3);
            skill->GetVictim ()->SetBuffid (5);
            skill->GetVictim ()->SetDecfatalhurt (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (15000);
            skill->GetVictim ()->SetRatio (0.40001);
            skill->GetVictim ()->SetBuffid (5);
            skill->GetVictim ()->SetIncskilldodge (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (15000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (4.001);
            skill->GetVictim ()->SetAddspeed (1);
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
