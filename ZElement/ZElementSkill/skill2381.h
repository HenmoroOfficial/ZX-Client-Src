#ifndef __CPPGEN_GNET_SKILL2381
#define __CPPGEN_GNET_SKILL2381
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2381:public Skill
    {
      public:
        enum
        { SKILL_ID = 2381 };
          Skill2381 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2381Stub:public SkillStub
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
      Skill2381Stub ():SkillStub (2381)
        {
            occupation = 128;
            name = L"ğÎ×Ó·­Éí";
            nativename = "ğÎ×Ó·­Éí";
            icon = "ğÎ×Ó·­Éí.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 2;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            clearmask = 0;
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
            talent[0] = 619;
            talent_size = 1;
            action[0] = "¹íÍõ×Ú_¿ñ·çÖä";
            action[1] = "¹íÍõ×Ú_¿ñ·çÖä";
            action[2] = "¹íÍõ×Ú_¿ñ·çÖä";
            action[3] = "¹íÍõ×Ú_¿ñ·çÖä";
            action[4] = "¹íÍõ×Ú_¿ñ·çÖä";
            action[5] = "¹íÍõ×Ú_¿ñ·çÖä";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_¿ñ·çÖä";
            action[16] = "¹íÍõ×Ú_Æï³Ë_¿ñ·çÖä";
            action[17] = "¹íÍõ×Ú_Æï³Ë_¿ñ·çÖä";
            action[18] = "¹íÍõ×Ú_Æï³Ë_¿ñ·çÖä";
            action[19] = "¹íÍõ×Ú_Æï³Ë_¿ñ·çÖä";
            action[20] = "¹íÍõ×Ú_Æï³Ë_¿ñ·çÖä";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
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
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill2381Stub ()
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
            return 45000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[1] = { 1 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (20);
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
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (10000);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetValue (2.5);
            skill->GetVictim ()->SetAddspeed (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (10);
        }
#endif
    };
}
#endif
