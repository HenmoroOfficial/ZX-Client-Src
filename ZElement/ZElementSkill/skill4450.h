#ifndef __CPPGEN_GNET_SKILL4450
#define __CPPGEN_GNET_SKILL4450
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4450:public Skill
    {
      public:
        enum
        { SKILL_ID = 4450 };
          Skill4450 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4450Stub:public SkillStub
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
      Skill4450Stub ():SkillStub (4450)
        {
            occupation = 151;
            name = L"ÖÁ×ðÎÞÉÏ£¨µØ£©";
            nativename = "ÖÁ×ðÎÞÉÏ£¨µØ£©";
            icon = "±ÌÂäÑª.dds";
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
            action[0] = "¹íÍõ×Ú_ÃÍ»÷";
            action[1] = "ÇàÔÆÃÅ_ÃÍ»÷";
            action[2] = "¹íÍõ×Ú_ÃÍ»÷";
            action[3] = "ºÏ»¶ÅÉ_ÃÍ»÷";
            action[4] = "ÌìÒôËÂ_ÃÍ»÷";
            action[5] = "¹íµÀ_ÃÍ»÷";
            action[6] = "¹íÍõ×Ú_ÃÍ»÷";
            action[7] = "ÊÞÉñ_ÃÍ»÷";
            action[8] = "ºüÑý_ÃÍ»÷";
            action[9] = "ÐùÔ¯_ÃÍ»÷";
            action[10] = "»³¹â_ÃÍ»÷";
            action[11] = "ÊÞÉñ_ÃÍ»÷";
            action[12] = "Ìì»ª_ÃÍ»÷";
            action[13] = "ÁéÙí_ÃÍ»÷";
            action[14] = "Ó¢ÕÐ_ÃÍ»÷";
            action[15] = "¹íÍõ×Ú_Æï³Ë_ÃÍ»÷";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_ÃÍ»÷";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ÃÍ»÷";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_ÃÍ»÷";
            action[19] = "ÌìÒôËÂ_Æï³Ë_ÃÍ»÷";
            action[20] = "¹íµÀ_Æï³Ë_ÃÍ»÷";
            action[21] = "¹íÍõ×Ú_Æï³Ë_ÃÍ»÷";
            action[22] = "ÊÞÉñ_Æï³Ë_ÃÍ»÷";
            action[23] = "ºüÑý_Æï³Ë_ÃÍ»÷";
            action[24] = "ÐùÔ¯_Æï³Ë_ÃÍ»÷";
            action[25] = "»³¹â_Æï³Ë_ÃÍ»÷";
            action[26] = "ÊÞÉñ_Æï³Ë_ÃÍ»÷";
            action[27] = "Ìì»ª_Æï³Ë_ÃÍ»÷";
            action[28] = "ÁéÙí_Æï³Ë_ÃÍ»÷";
            action[29] = "Ó¢ÕÐ_Æï³Ë_ÃÍ»÷";
            action[30] = "0";
            rangetype = 0;
            doenchant = true;
            dobless = true;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 7;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill4450Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 9);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (2012);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 300000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 8);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 7);
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
            return _snwprintf (buffer, length, format, 2012);

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (25000);
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (-70);
            skill->GetVictim ()->SetDivinityburst (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (25000);
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (70);
            skill->GetVictim ()->SetDivinityburst (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (100);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (1000000);
            skill->GetVictim ()->SetValue (1000000);
            skill->GetVictim ()->SetDivinityfury (1);
            return true;
        }
#endif
    };
}
#endif
