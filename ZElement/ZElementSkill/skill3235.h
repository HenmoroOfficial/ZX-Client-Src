#ifndef __CPPGEN_GNET_SKILL3235
#define __CPPGEN_GNET_SKILL3235
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3235:public Skill
    {
      public:
        enum
        { SKILL_ID = 3235 };
          Skill3235 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3235Stub:public SkillStub
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
      Skill3235Stub ():SkillStub (3235)
        {
            occupation = 129;
            name = L"Ñ©ÔÏäÁÇé";
            nativename = "Ñ©ÔÏäÁÇé";
            icon = "·ÙÐÄÀ÷À×.dds";
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
            action[0] = "ÇàÔÆÃÅ_ÆÆÄ§½£Æø";
            action[1] = "ÇàÔÆÃÅ_ÆÆÄ§½£Æø";
            action[2] = "ÇàÔÆÃÅ_ÆÆÄ§½£Æø";
            action[3] = "ÇàÔÆÃÅ_ÆÆÄ§½£Æø";
            action[4] = "ÇàÔÆÃÅ_ÆÆÄ§½£Æø";
            action[5] = "¹íµÀ_ÓñÕæ¾÷";
            action[6] = "ÇàÔÆÃÅ_ÆÆÄ§½£Æø";
            action[7] = "ÊÞÉñ_Ñ©ç÷Ö®±ùËª";
            action[8] = "ºüÑý_Ñ©ç÷Ö®±ùËª";
            action[9] = "ÐùÔ¯_Ñ©ç÷Ö®±ùËª";
            action[10] = "»³¹â_Ñ©ç÷Ö®±ùËª";
            action[11] = "ÊÞÉñ_Ñ©ç÷Ö®±ùËª";
            action[12] = "Ìì»ª_Ñ©ç÷Ö®±ùËª";
            action[13] = "";
            action[14] = "";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_ÆÆÄ§½£Æø";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_ÆÆÄ§½£Æø";
            action[17] = "ÇàÔÆÃÅ_Æï³Ë_ÆÆÄ§½£Æø";
            action[18] = "ÇàÔÆÃÅ_Æï³Ë_ÆÆÄ§½£Æø";
            action[19] = "ÇàÔÆÃÅ_Æï³Ë_ÆÆÄ§½£Æø";
            action[20] = "¹íµÀ_Æï³Ë_ÓñÕæ¾÷";
            action[21] = "ÇàÔÆÃÅ_Æï³Ë_ÆÆÄ§½£Æø";
            action[22] = "ÊÞÉñ_Æï³Ë_Ñ©ç÷Ö®±ùËª";
            action[23] = "ºüÑý_Æï³Ë_Ñ©ç÷Ö®±ùËª";
            action[24] = "ÐùÔ¯_Æï³Ë_Ñ©ç÷Ö®±ùËª";
            action[25] = "»³¹â_Æï³Ë_Ñ©ç÷Ö®±ùËª";
            action[26] = "ÊÞÉñ_Æï³Ë_Ñ©ç÷Ö®±ùËª";
            action[27] = "Ìì»ª_Æï³Ë_Ñ©ç÷Ö®±ùËª";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 1;
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
        virtual ~ Skill3235Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (35);
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
            return (float) (2);
        }
        float GetAttackdistance (Skill * skill) const
        {
            return (float) (30);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (25);
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
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetRatio (0.3);
            skill->GetVictim ()->SetBuffid (5);
            skill->GetVictim ()->SetDecskilldamage (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetBuffid (5);
            skill->GetVictim ()->SetValue (106);
            skill->GetVictim ()->SetSubanti (1);
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
