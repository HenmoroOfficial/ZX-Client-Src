#ifndef __CPPGEN_GNET_SKILL1970
#define __CPPGEN_GNET_SKILL1970
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1970:public Skill
    {
      public:
        enum
        { SKILL_ID = 1970 };
          Skill1970 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1970Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 4000;
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
                return 2000;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1970Stub ():SkillStub (1970)
        {
            occupation = 166;
            name = L"²¶×½";
            nativename = "²¶×½";
            icon = "²¶×½.dds";
            maxlevel = 10;
            maxlearn = 10;
            type = 3;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            clearmask = 256;
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
            action[0] = "ÇàÔÆÃÅ_¶¯Îï²¶×½";
            action[1] = "ÇàÔÆÃÅ_¶¯Îï²¶×½";
            action[2] = "¹íÍõ×Ú_¶¯Îï²¶×½";
            action[3] = "ºÏ»¶ÅÉ_¶¯Îï²¶×½";
            action[4] = "ÌìÒôËÂ_¶¯Îï²¶×½";
            action[5] = "¹íµÀ_¶¯Îï²¶×½";
            action[6] = "¹íµÀ_¶¯Îï²¶×½";
            action[7] = "ÊŞÉñ_¶¯Îï²¶×½";
            action[8] = "ºüÑı_¶¯Îï²¶×½";
            action[9] = "ĞùÔ¯_¶¯Îï²¶×½";
            action[10] = "»³¹â_¶¯Îï²¶×½";
            action[11] = "Ì«ê»_¶¯Îï²¶×½";
            action[12] = "Ìì»ª_¶¯Îï²¶×½";
            action[13] = "ÁéÙí_¶¯Îï²¶×½";
            action[14] = "Ó¢ÕĞ_¶¯Îï²¶×½";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_¶¯Îï²¶×½";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_¶¯Îï²¶×½";
            action[17] = "¹íÍõ×Ú_Æï³Ë_¶¯Îï²¶×½";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_¶¯Îï²¶×½";
            action[19] = "ÌìÒôËÂ_Æï³Ë_¶¯Îï²¶×½";
            action[20] = "¹íµÀ_Æï³Ë_¶¯Îï²¶×½";
            action[21] = "ÇàÔÆÃÅ_Æï³Ë_¶¯Îï²¶×½";
            action[22] = "ÊŞÉñ_Æï³Ë_¶¯Îï²¶×½";
            action[23] = "ºüÑı_Æï³Ë_¶¯Îï²¶×½";
            action[24] = "ĞùÔ¯_Æï³Ë_¶¯Îï²¶×½";
            action[25] = "»³¹â_Æï³Ë_¶¯Îï²¶×½";
            action[26] = "Ì«ê»_Æï³Ë_¶¯Îï²¶×½";
            action[27] = "Ìì»ª_Æï³Ë_¶¯Îï²¶×½";
            action[28] = "ÁéÙí_Æï³Ë_¶¯Îï²¶×½";
            action[29] = "Ó¢ÕĞ_Æï³Ë_¶¯Îï²¶×½";
            action[30] = "0";
            rangetype = 0;
            doenchant = true;
            dobless = false;
            need_item = true;
            item_relation = 0;
            item1_id = 31754;
            item1_num = 1;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            use_proficiency = true;
            inc_proficiency = 0;
            skill_limit = 14;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill1970Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (18);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (2000);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 6000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 600000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 60, 60, 60, 60, 60, 60, 60, 60, 60, 60 };
            return array[level - 1];
        }
        int GetRequiredProficiency (int level) const
        {
            static int array[10] = { 0, 6, 48, 144, 432, 864, 2592, 5184, 9216, 14256 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (12);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 2000);

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel (), skill->GetCurproficiency (), skill->GetNeedproficiency ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetCatchpet (1);
            return true;
        }
#endif
    };
}
#endif
