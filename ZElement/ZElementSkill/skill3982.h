#ifndef __CPPGEN_GNET_SKILL3982
#define __CPPGEN_GNET_SKILL3982
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3982:public Skill
    {
      public:
        enum
        { SKILL_ID = 3982 };
          Skill3982 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3982Stub:public SkillStub
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
      Skill3982Stub ():SkillStub (3982)
        {
            occupation = 151;
            name = L"°ËÃÅ¶Ý¼×¡¤¾øÉË";
            nativename = "°ËÃÅ¶Ý¼×¡¤¾øÉË";
            icon = "µêÂô3µµÍ¨ÓÃ½ðÉí.dds";
            maxlevel = 1;
            maxlearn = 0;
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
            action[0] = "¹íÍõ×Ú_¼á¶Ü";
            action[1] = "ÇàÔÆÃÅ_¼á¶Ü";
            action[2] = "¹íÍõ×Ú_¼á¶Ü";
            action[3] = "ºÏ»¶ÅÉ_¼á¶Ü";
            action[4] = "ÌìÒôËÂ_¼á¶Ü";
            action[5] = "¹íµÀ_¼á¶Ü";
            action[6] = "·ÙÏã¹È_¼á¶Ü";
            action[7] = "ÊÞÉñ_¼á¶Ü";
            action[8] = "ºüÑý_¼á¶Ü";
            action[9] = "ÐùÔ¯_¼á¶Ü";
            action[10] = "»³¹â_¼á¶Ü";
            action[11] = "Ì«ê»_¼á¶Ü";
            action[12] = "Ìì»ª_¼á¶Ü";
            action[13] = "ÁéÙí_¼á¶Ü";
            action[14] = "Ó¢ÕÐ_¼á¶Ü";
            action[15] = "¹íÍõ×Ú_Æï³Ë_¼á¶Ü";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_¼á¶Ü";
            action[17] = "¹íÍõ×Ú_Æï³Ë_¼á¶Ü";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_¼á¶Ü";
            action[19] = "ÌìÒôËÂ_Æï³Ë_¼á¶Ü";
            action[20] = "¹íµÀ_Æï³Ë_¼á¶Ü";
            action[21] = "·ÙÏã¹È_Æï³Ë_¼á¶Ü";
            action[22] = "ÊÞÉñ_Æï³Ë_¼á¶Ü";
            action[23] = "ºüÑý_Æï³Ë_¼á¶Ü";
            action[24] = "ÐùÔ¯_Æï³Ë_¼á¶Ü";
            action[25] = "»³¹â_Æï³Ë_¼á¶Ü";
            action[26] = "Ì«ê»_Æï³Ë_¼á¶Ü";
            action[27] = "Ìì»ª_Æï³Ë_¼á¶Ü";
            action[28] = "ÁéÙí_Æï³Ë_¼á¶Ü";
            action[29] = "Ó¢ÕÐ_Æï³Ë_¼á¶Ü";
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
        virtual ~ Skill3982Stub ()
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
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 10000;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
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
            skill->GetVictim ()->SetTime (10000);
            skill->GetVictim ()->SetInvincible (1);
            return true;
        }
#endif
    };
}
#endif
