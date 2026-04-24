#ifndef __CPPGEN_GNET_SKILL4769
#define __CPPGEN_GNET_SKILL4769
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4769:public Skill
    {
      public:
        enum
        { SKILL_ID = 4769 };
          Skill4769 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4769Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetSpeed ());
            }
        };
#endif
      Skill4769Stub ():SkillStub (4769)
        {
            occupation = 129;
            name = L"°×Å£±äÉíÈÎÎñ°æ";
            nativename = "°×Å£±äÉíÈÎÎñ°æ";
            icon = "";
            maxlevel = 1;
            maxlearn = 1;
            type = 13;
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
            action[0] = "¹íÍõ×Ú_Ðé¿ÕÕ¶";
            action[1] = "¹íÍõ×Ú_Ðé¿ÕÕ¶";
            action[2] = "¹íÍõ×Ú_Ðé¿ÕÕ¶";
            action[3] = "¹íÍõ×Ú_Ðé¿ÕÕ¶";
            action[4] = "¹íÍõ×Ú_Ðé¿ÕÕ¶";
            action[5] = "¹íÍõ×Ú_Ðé¿ÕÕ¶";
            action[6] = "¹íÍõ×Ú_Ðé¿ÕÕ¶";
            action[7] = "ÊÞÉñ_ºáÐÐ";
            action[8] = "ºüÑý_¹ÌÔªÊõ";
            action[9] = "ÐùÔ¯_·Éµ¯";
            action[10] = "»³¹â_ÍµÏ®";
            action[11] = "Ì«ê»_¼ÓËÙ";
            action[12] = "Ìì»ª_³õÔªÒôÈÐ";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_Ðé¿ÕÕ¶";
            action[16] = "¹íÍõ×Ú_Æï³Ë_Ðé¿ÕÕ¶";
            action[17] = "¹íÍõ×Ú_Æï³Ë_Ðé¿ÕÕ¶";
            action[18] = "¹íÍõ×Ú_Æï³Ë_Ðé¿ÕÕ¶";
            action[19] = "¹íÍõ×Ú_Æï³Ë_Ðé¿ÕÕ¶";
            action[20] = "¹íÍõ×Ú_Æï³Ë_Ðé¿ÕÕ¶";
            action[21] = "¹íÍõ×Ú_Æï³Ë_Ðé¿ÕÕ¶";
            action[22] = "ÊÞÉñ_Æï³Ë_ºáÐÐ";
            action[23] = "ºüÑý_Æï³Ë_¹ÌÔªÊõ";
            action[24] = "ÐùÔ¯_Æï³Ë_·Éµ¯";
            action[25] = "»³¹â_Æï³Ë_ÍµÏ®";
            action[26] = "Ì«ê»_Æï³Ë_¼ÓËÙ";
            action[27] = "Ìì»ª_Æï³Ë_³õÔªÒôÈÐ";
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
        virtual ~ Skill4769Stub ()
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
            return 30000;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (5);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (5);
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
            skill->GetVictim ()->SetAmount (10);
            skill->GetVictim ()->SetClearbuff (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (900000);
            skill->GetVictim ()->SetRatio (0.03);
            skill->GetVictim ()->SetBuffid (8);
            skill->GetVictim ()->SetIncdefence (1);
            skill->GetVictim ()->SetTime (900000);
            skill->GetVictim ()->SetRatio (0.01);
            skill->GetVictim ()->SetBuffid (8);
            skill->GetVictim ()->SetInchp (1);
            return true;
        }
#endif
    };
}
#endif
