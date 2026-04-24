#ifndef __CPPGEN_GNET_SKILL3975
#define __CPPGEN_GNET_SKILL3975
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3975:public Skill
    {
      public:
        enum
        { SKILL_ID = 3975 };
          Skill3975 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3975Stub:public SkillStub
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
      Skill3975Stub ():SkillStub (3975)
        {
            occupation = 151;
            name = L"°ËÃÅ¶Ý¼×¡¤»¯Éú";
            nativename = "°ËÃÅ¶Ý¼×¡¤»¯Éú";
            icon = "°®ÇéÁ¼Ò©.dds";
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
            action[0] = "¹íÍõ×Ú_»Ø´º";
            action[1] = "ÇàÔÆÃÅ_»Ø´º";
            action[2] = "¹íÍõ×Ú_»Ø´º";
            action[3] = "ºÏ»¶ÅÉ_»Ø´º";
            action[4] = "ÌìÒôËÂ_»Ø´º";
            action[5] = "¹íµÀ_»Ø´º";
            action[6] = "·ÙÏã¹È_»Ø´º";
            action[7] = "ÊÞÉñ_»Ø´º";
            action[8] = "ºüÑý_»Ø´º";
            action[9] = "ÐùÔ¯_»Ø´º";
            action[10] = "»³¹â_»Ø´º";
            action[11] = "Ì«ê»_»Ø´º";
            action[12] = "Ìì»ª_»Ø´º";
            action[13] = "ÁéÙí_»Ø´º";
            action[14] = "Ó¢ÕÐ_»Ø´º";
            action[15] = "¹íÍõ×Ú_Æï³Ë_»Ø´º";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_»Ø´º";
            action[17] = "¹íÍõ×Ú_Æï³Ë_»Ø´º";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_»Ø´º";
            action[19] = "ÌìÒôËÂ_Æï³Ë_»Ø´º";
            action[20] = "¹íµÀ_Æï³Ë_»Ø´º";
            action[21] = "·ÙÏã¹È_Æï³Ë_»Ø´º";
            action[22] = "ÊÞÉñ_Æï³Ë_»Ø´º";
            action[23] = "ºüÑý_Æï³Ë_»Ø´º";
            action[24] = "ÐùÔ¯_Æï³Ë_»Ø´º";
            action[25] = "»³¹â_Æï³Ë_»Ø´º";
            action[26] = "Ì«ê»_Æï³Ë_»Ø´º";
            action[27] = "Ìì»ª_Æï³Ë_»Ø´º";
            action[28] = "ÁéÙí_Æï³Ë_»Ø´º";
            action[29] = "Ó¢ÕÐ_Æï³Ë_»Ø´º";
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
        virtual ~ Skill3975Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
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
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0.6 * skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetHpgen (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0.6 * skill->GetPlayer ()->GetMaxmp ());
            skill->GetVictim ()->SetMpgen (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (0.6 * skill->GetPlayer ()->GetMaxdp ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetDivinityfury (1);
            return true;
        }
#endif
    };
}
#endif
