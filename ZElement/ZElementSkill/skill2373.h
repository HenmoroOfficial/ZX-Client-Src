#ifndef __CPPGEN_GNET_SKILL2373
#define __CPPGEN_GNET_SKILL2373
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2373:public Skill
    {
      public:
        enum
        { SKILL_ID = 2373 };
          Skill2373 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2373Stub:public SkillStub
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
      Skill2373Stub ():SkillStub (2373)
        {
            occupation = 151;
            name = L"°Ë¼«»ÍÀ×ÉÁ";
            nativename = "°Ë¼«»ÍÀ×ÉÁ";
            icon = "ÓêÊ¦.dds";
            maxlevel = 2;
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
            action[0] = "ÇàÔÆÃÅ_Á¶Æø»¹Éñ";
            action[1] = "ÇàÔÆÃÅ_Á¶Æø»¹Éñ";
            action[2] = "¹íÍõ×Ú_¿İÄ¾Öä";
            action[3] = "ºÏ»¶ÅÉ_¼¤Àø";
            action[4] = "ÌìÒôËÂ_Ò×½î¾­";
            action[5] = "ÄıÉñÑªÖä";
            action[6] = "0";
            action[7] = "ÊŞÉñ_ºáĞĞ";
            action[8] = "ºüÑı_¹ÌÔªÊõ";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "ÁéÙí_¼á¶Ü";
            action[14] = "Ó¢ÕĞ_¼á¶Ü";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_Á¶Æø»¹Éñ";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_Á¶Æø»¹Éñ";
            action[17] = "¹íÍõ×Ú_Æï³Ë_¿İÄ¾Öä";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_¼¤Àø";
            action[19] = "ÌìÒôËÂ_Æï³Ë_Ò×½î¾­";
            action[20] = "ÄıÉñÑªÖä";
            action[21] = "0";
            action[22] = "ÊŞÉñ_Æï³Ë_ºáĞĞ";
            action[23] = "ºüÑı_Æï³Ë_¹ÌÔªÊõ";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "ÁéÙí_Æï³Ë_¼á¶Ü";
            action[29] = "Ó¢ÕĞ_Æï³Ë_¼á¶Ü";
            action[30] = "0";
            rangetype = 5;
            doenchant = true;
            dobless = false;
            need_item = true;
            item_relation = 0;
            item1_id = 33140;
            item1_num = 1;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill2373Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1000);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 1000 * (840 - 240 * skill->GetLevel ());
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
            return _snwprintf (buffer, length, format, 840 - 240 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetValue (8275);
            skill->GetVictim ()->SetAdddefence (1);
            return true;
        }
#endif
    };
}
#endif
