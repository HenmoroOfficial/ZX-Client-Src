#ifndef __CPPGEN_GNET_SKILL3179
#define __CPPGEN_GNET_SKILL3179
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3179:public Skill
    {
      public:
        enum
        { SKILL_ID = 3179 };
          Skill3179 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3179Stub:public SkillStub
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
      Skill3179Stub ():SkillStub (3179)
        {
            occupation = 129;
            name = L"¸£ĞÇ¸ßÕÕ";
            nativename = "¸£ĞÇ¸ßÕÕ";
            icon = "";
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
            action[0] = "¹íÍõ×Ú_»¢Ğ¥";
            action[1] = "ÇàÔÆÃÅ_»¢Ğ¥";
            action[2] = "¹íÍõ×Ú_»¢Ğ¥";
            action[3] = "ºÏ»¶ÅÉ_»¢Ğ¥";
            action[4] = "ÌìÒôËÂ_»¢Ğ¥";
            action[5] = "¹íµÀ_»¢Ğ¥";
            action[6] = "0";
            action[7] = "ÊŞÉñ_ºáĞĞ";
            action[8] = "ºüÑı_·çĞĞÕß";
            action[9] = "0";
            action[10] = "»³¹â_Ç³Ï®";
            action[11] = "0";
            action[12] = "Ìì»ª_³õÔªÒôÈĞ";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_»¢Ğ¥";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_»¢Ğ¥";
            action[17] = "¹íÍõ×Ú_Æï³Ë_»¢Ğ¥";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_»¢Ğ¥";
            action[19] = "ÌìÒôËÂ_Æï³Ë_»¢Ğ¥";
            action[20] = "¹íµÀ_Æï³Ë_»¢Ğ¥";
            action[21] = "0";
            action[22] = "ÊŞÉñ_Æï³Ë_ºáĞĞ";
            action[23] = "ºüÑı_Æï³Ë_·çĞĞÕß";
            action[24] = "0";
            action[25] = "»³¹â_Æï³Ë_Ç³Ï®";
            action[26] = "0";
            action[27] = "Ìì»ª_Æï³Ë_³õÔªÒôÈĞ";
            action[28] = "";
            action[29] = "";
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
            skill_limit = 14;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill3179Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 200;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 1000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (14);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (13);
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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetTmplid () == 42170 ? 100 : 0));
            skill->GetVictim ()->SetValue ((skill->GetPlayer ()->GetTmplid () == 42170) ? (skill->GetPlayer ()->GetMaxhp () + zrand (500)) : 0);
            skill->GetVictim ()->SetDirecthurt (1);
            return true;
        }
#endif
    };
}
#endif
