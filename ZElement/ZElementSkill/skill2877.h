#ifndef __CPPGEN_GNET_SKILL2877
#define __CPPGEN_GNET_SKILL2877
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2877:public Skill
    {
      public:
        enum
        { SKILL_ID = 2877 };
          Skill2877 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2877Stub:public SkillStub
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
      Skill2877Stub ():SkillStub (2877)
        {
            occupation = 129;
            name = L"ºÓÑô¾üÁî";
            nativename = "ºÓÑô¾üÁî";
            icon = "";
            maxlevel = 10;
            maxlearn = 10;
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
            action[0] = "¹íÍõ×Ú_¿ÝÄ¾Öä";
            action[1] = "ÇàÔÆÃÅ_Á¶Æø»¹Éñ";
            action[2] = "¹íÍõ×Ú_¿ÝÄ¾Öä";
            action[3] = "ºÏ»¶ÅÉ_¼¤Àø";
            action[4] = "ÌìÒôËÂ_Ò×½î¾­";
            action[5] = "ÄýÉñÑªÖä";
            action[6] = "0";
            action[7] = "ÊÞÉñ_ºáÐÐ";
            action[8] = "ºüÑý_¹ÌÔªÊõ";
            action[9] = "0";
            action[10] = "»³¹â_ÍµÏ®";
            action[11] = "0";
            action[12] = "Ìì»ª_´ºÀ×";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_¿ÝÄ¾Öä";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_Á¶Æø»¹Éñ";
            action[17] = "¹íÍõ×Ú_Æï³Ë_¿ÝÄ¾Öä";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_¼¤Àø";
            action[19] = "ÌìÒôËÂ_Æï³Ë_Ò×½î¾­";
            action[20] = "ÄýÉñÑªÖä";
            action[21] = "0";
            action[22] = "ÊÞÉñ_Æï³Ë_ºáÐÐ";
            action[23] = "ºüÑý_Æï³Ë_¹ÌÔªÊõ";
            action[24] = "0";
            action[25] = "»³¹â_Æï³Ë_ÍµÏ®";
            action[26] = "0";
            action[27] = "Ìì»ª_Æï³Ë_´ºÀ×";
            action[28] = "";
            action[29] = "";
            action[30] = "¼¼ÄÜ¹¥»÷2";
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
        virtual ~ Skill2877Stub ()
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
            return 600000;
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
            skill->GetVictim ()->SetTime (90000);
            skill->GetVictim ()->SetRatio (0.06 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetInchp (1);
            skill->GetVictim ()->SetTime (90000);
            skill->GetVictim ()->SetRatio (0.06 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetIncmp (1);
            return true;
        }
#endif
    };
}
#endif
