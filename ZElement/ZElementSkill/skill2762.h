#ifndef __CPPGEN_GNET_SKILL2762
#define __CPPGEN_GNET_SKILL2762
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2762:public Skill
    {
      public:
        enum
        { SKILL_ID = 2762 };
          Skill2762 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2762Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 400;
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
      Skill2762Stub ():SkillStub (2762)
        {
            occupation = 128;
            name = L"ÁìÍÁÕ½ËşÓÃ³°·í";
            nativename = "ÁìÍÁÕ½ËşÓÃ³°·í";
            icon = "Àä³°ÈÈ·í.dds";
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
            action[0] = "¹íÍõ×Ú_¼¤Å­";
            action[1] = "ÇàÔÆÃÅ_»¢Ğ¥";
            action[2] = "¹íÍõ×Ú_¼¤Å­";
            action[3] = "ºÏ»¶ÅÉ_»¢Ğ¥";
            action[4] = "ÌìÒôËÂ_»¢Ğ¥";
            action[5] = "¹íµÀ_»¢Ğ¥";
            action[6] = "0";
            action[7] = "¹íÍõ×Ú_¼¤Å­";
            action[8] = "¹íÍõ×Ú_¼¤Å­";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_¼¤Å­";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_»¢Ğ¥";
            action[17] = "¹íÍõ×Ú_Æï³Ë_¼¤Å­";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_»¢Ğ¥";
            action[19] = "ÌìÒôËÂ_Æï³Ë_»¢Ğ¥";
            action[20] = "¹íµÀ_Æï³Ë_»¢Ğ¥";
            action[21] = "0";
            action[22] = "¹íÍõ×Ú_Æï³Ë_¼¤Å­";
            action[23] = "¹íÍõ×Ú_Æï³Ë_¼¤Å­";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "¼¼ÄÜ¹¥»÷1";
            rangetype = 2;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 6;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill2762Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 4);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 600;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 200;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 4);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 1);
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
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetAssault (1);
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
