#ifndef __CPPGEN_GNET_SKILL3912
#define __CPPGEN_GNET_SKILL3912
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3912:public Skill
    {
      public:
        enum
        { SKILL_ID = 3912 };
          Skill3912 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3912Stub:public SkillStub
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
      Skill3912Stub ():SkillStub (3912)
        {
            occupation = 151;
            name = L"ĞÇÓï¡¤Ê®²½Ò»É±";
            nativename = "ĞÇÓï¡¤Ê®²½Ò»É±";
            icon = "ÉñÍ¨_Ê®²½Ò»É±.dds";
            maxlevel = 1;
            maxlearn = 0;
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
            action[0] = "¹íÍõ×Ú_ÆÆ¼×";
            action[1] = "ÇàÔÆÃÅ_ÆÆ¼×";
            action[2] = "¹íÍõ×Ú_ÆÆ¼×";
            action[3] = "ºÏ»¶ÅÉ_ÆÆ¼×";
            action[4] = "ÌìÒôËÂ_ÆÆ¼×";
            action[5] = "¹íµÀ_ÆÆ¼×";
            action[6] = "·ÙÏã¹È_ÆÆ¼×";
            action[7] = "ÊŞÉñ_ÆÆ¼×";
            action[8] = "ºüÑı_ÆÆ¼×";
            action[9] = "ĞùÔ¯_ÆÆ¼×";
            action[10] = "»³¹â_ÆÆ¼×";
            action[11] = "Ì«ê»_ÆÆ¼×";
            action[12] = "Ìì»ª_ÆÆ¼×";
            action[13] = "ÁéÙí_ÆÆ¼×";
            action[14] = "Ó¢ÕĞ_ÆÆ¼×";
            action[15] = "¹íÍõ×Ú_Æï³Ë_ÆÆ¼×";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_ÆÆ¼×";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ÆÆ¼×";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_ÆÆ¼×";
            action[19] = "ÌìÒôËÂ_Æï³Ë_ÆÆ¼×";
            action[20] = "¹íµÀ_Æï³Ë_ÆÆ¼×";
            action[21] = "·ÙÏã¹È_Æï³Ë_ÆÆ¼×";
            action[22] = "ÊŞÉñ_Æï³Ë_ÆÆ¼×";
            action[23] = "ºüÑı_Æï³Ë_ÆÆ¼×";
            action[24] = "ĞùÔ¯_Æï³Ë_ÆÆ¼×";
            action[25] = "»³¹â_Æï³Ë_ÆÆ¼×";
            action[26] = "Ì«ê»_Æï³Ë_ÆÆ¼×";
            action[27] = "Ìì»ª_Æï³Ë_ÆÆ¼×";
            action[28] = "ÁéÙí_Æï³Ë_ÆÆ¼×";
            action[29] = "Ó¢ÕĞ_Æï³Ë_ÆÆ¼×";
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
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill3912Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (20);
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
            return 360 * 1000;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (15);
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
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (5000);
            skill->GetVictim ()->SetDiet (1);
            return true;
        }
#endif
    };
}
#endif
