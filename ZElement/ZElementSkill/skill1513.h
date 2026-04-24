#ifndef __CPPGEN_GNET_SKILL1513
#define __CPPGEN_GNET_SKILL1513
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1513:public Skill
    {
      public:
        enum
        { SKILL_ID = 1513 };
          Skill1513 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1513Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1600;
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
            }
        };
#endif
      Skill1513Stub ():SkillStub (1513)
        {
            occupation = 129;
            name = L"°Á½£ÎŞºÛ";
            nativename = "°Á½£ÎŞºÛ";
            icon = "·ÙĞÄÀ÷À×.dds";
            maxlevel = 1;
            maxlearn = 10;
            type = 1;
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
            action[0] = "ÇàÔÆÃÅ_ËªÌìÑ©Îè";
            action[1] = "ÇàÔÆÃÅ_ËªÌìÑ©Îè";
            action[2] = "ÇàÔÆÃÅ_ËªÌìÑ©Îè";
            action[3] = "ÇàÔÆÃÅ_ËªÌìÑ©Îè";
            action[4] = "ÇàÔÆÃÅ_ËªÌìÑ©Îè";
            action[5] = "ÇàÔÆÃÅ_ËªÌìÑ©Îè";
            action[6] = "·ÙÏã¹È_³õ¼¶»ğ¹¥";
            action[7] = "ÊŞÉñ_Ñ©ç÷Ö®±ùËª";
            action[8] = "ºüÑı_Ñ©ç÷Ö®±ùËª";
            action[9] = "ĞùÔ¯_Ñ©ç÷Ö®±ùËª";
            action[10] = "»³¹â_Ñ©ç÷Ö®±ùËª";
            action[11] = "Ì«ê»_´¸»÷";
            action[12] = "Ìì»ª_Ñ©ç÷Ö®±ùËª";
            action[13] = "ÁéÙí_Ñ©ç÷Ö®±ùËª";
            action[14] = "Ó¢ÕĞ_Ñ©ç÷Ö®±ùËª";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_ËªÌìÑ©Îè";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_ËªÌìÑ©Îè";
            action[17] = "ÇàÔÆÃÅ_Æï³Ë_ËªÌìÑ©Îè";
            action[18] = "ÇàÔÆÃÅ_Æï³Ë_ËªÌìÑ©Îè";
            action[19] = "ÇàÔÆÃÅ_Æï³Ë_ËªÌìÑ©Îè";
            action[20] = "ÇàÔÆÃÅ_Æï³Ë_ËªÌìÑ©Îè";
            action[21] = "·ÙÏã¹È_Æï³Ë_³õ¼¶»ğ¹¥";
            action[22] = "ÊŞÉñ_Æï³Ë_Ñ©ç÷Ö®±ùËª";
            action[23] = "ºüÑı_Æï³Ë_Ñ©ç÷Ö®±ùËª";
            action[24] = "ĞùÔ¯_Æï³Ë_Ñ©ç÷Ö®±ùËª";
            action[25] = "»³¹â_Æï³Ë_Ñ©ç÷Ö®±ùËª";
            action[26] = "Ì«ê»_Æï³Ë_´¸»÷";
            action[27] = "Ìì»ª_Æï³Ë_Ñ©ç÷Ö®±ùËª";
            action[28] = "ÁéÙí_Æï³Ë_Ñ©ç÷Ö®±ùËª";
            action[29] = "Ó¢ÕĞ_Æï³Ë_Ñ©ç÷Ö®±ùËª";
            action[30] = "0";
            rangetype = 3;
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
        virtual ~ Skill1513Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (30);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (600);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
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
            return _snwprintf (buffer, length, format, INT (600), 25, 90);

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
            skill->GetVictim ()->SetTime (100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0.333 * skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak1 (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetRatio (0.9);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecaccuracy (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (15000);
            skill->GetVictim ()->SetBuffid (8);
            skill->GetVictim ()->SetValue (9999);
            skill->GetVictim ()->SetAdddefence (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (25);
        }
#endif
    };
}
#endif
