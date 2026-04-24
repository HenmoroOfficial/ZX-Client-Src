#ifndef __CPPGEN_GNET_SKILL1519
#define __CPPGEN_GNET_SKILL1519
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1519:public Skill
    {
      public:
        enum
        { SKILL_ID = 1519 };
          Skill1519 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1519Stub:public SkillStub
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
      Skill1519Stub ():SkillStub (1519)
        {
            occupation = 129;
            name = L"±Ìº£Çé³Õ";
            nativename = "±Ìº£Çé³Õ";
            icon = "ÍùÉúÙíÔ¸.dds";
            maxlevel = 1;
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
            action[0] = "¹íÍõ×Ú_³ÕÇéÖä";
            action[1] = "¹íÍõ×Ú_³ÕÇéÖä";
            action[2] = "¹íÍõ×Ú_³ÕÇéÖä";
            action[3] = "¹íÍõ×Ú_³ÕÇéÖä";
            action[4] = "¹íÍõ×Ú_³ÕÇéÖä";
            action[5] = "¹íµÀ_¿ÕÁé";
            action[6] = "0";
            action[7] = "ÊŞÉñ_É¢¾¡";
            action[8] = "ºüÑı_É¢¾¡";
            action[9] = "ĞùÔ¯_É¢¾¡";
            action[10] = "»³¹â_É¢¾¡";
            action[11] = "Ì«ê»_¼ÓËÙ";
            action[12] = "Ìì»ª_É¢¾¡";
            action[13] = "ÁéÙí_É¢¾¡";
            action[14] = "Ó¢ÕĞ_É¢¾¡";
            action[15] = "¹íÍõ×Ú_Æï³Ë_³ÕÇéÖä";
            action[16] = "¹íÍõ×Ú_Æï³Ë_³ÕÇéÖä";
            action[17] = "¹íÍõ×Ú_Æï³Ë_³ÕÇéÖä";
            action[18] = "¹íÍõ×Ú_Æï³Ë_³ÕÇéÖä";
            action[19] = "¹íÍõ×Ú_Æï³Ë_³ÕÇéÖä";
            action[20] = "¹íµÀ_Æï³Ë_¿ÕÁé";
            action[21] = "0";
            action[22] = "ÊŞÉñ_Æï³Ë_É¢¾¡";
            action[23] = "ºüÑı_Æï³Ë_É¢¾¡";
            action[24] = "ĞùÔ¯_Æï³Ë_É¢¾¡";
            action[25] = "»³¹â_Æï³Ë_É¢¾¡";
            action[26] = "Ì«ê»_Æï³Ë_¼ÓËÙ";
            action[27] = "Ìì»ª_Æï³Ë_É¢¾¡";
            action[28] = "ÁéÙí_Æï³Ë_É¢¾¡";
            action[29] = "Ó¢ÕĞ_Æï³Ë_É¢¾¡";
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
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill1519Stub ()
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
            return 200;
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
            return (float) (20);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, INT (600), 5, 25, 1000, 75, 3000);

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
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetRatio (0.25);
            skill->GetVictim ()->SetAmount (1000 * skill->GetLevel () * skill->GetPlayer ()->GetLevel ());
            skill->GetVictim ()->SetDechurt (1);
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetRatio (0.05);
            skill->GetVictim ()->SetInccritrate (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetRatio (0.75);
            skill->GetVictim ()->SetAmount (3000 * skill->GetLevel () * skill->GetPlayer ()->GetLevel ());
            skill->GetVictim ()->SetDechurt (1);
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
