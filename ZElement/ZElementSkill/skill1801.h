#ifndef __CPPGEN_GNET_SKILL1801
#define __CPPGEN_GNET_SKILL1801
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1801:public Skill
    {
      public:
        enum
        { SKILL_ID = 1801 };
          Skill1801 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1801Stub:public SkillStub
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
            bool Quit (Skill * skill) const
            {
                return skill->GetPlayer ()->GetHp () < skill->GetPlayer ()->GetMaxhp () - 1;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
      Skill1801Stub ():SkillStub (1801)
        {
            occupation = 129;
            name = L"¾ÛÑªÄıÔª";
            nativename = "¾ÛÑªÄıÔª";
            icon = "É­ÂŞÈıĞ¦.dds";
            maxlevel = 1;
            maxlearn = 10;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 1;
            credittype = 0;
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
            action[0] = "¹íÍõ×Ú_°üÔú";
            action[1] = "ÇàÔÆÃÅ_°üÔú";
            action[2] = "¹íÍõ×Ú_°üÔú";
            action[3] = "ºÏ»¶ÅÉ_°üÔú";
            action[4] = "ÌìÒôËÂ_°üÔú";
            action[5] = "ÇàÔÆÃÅ_°üÔú";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_°üÔú";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_°üÔú";
            action[17] = "¹íÍõ×Ú_Æï³Ë_°üÔú";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_°üÔú";
            action[19] = "ÌìÒôËÂ_Æï³Ë_°üÔú";
            action[20] = "ÇàÔÆÃÅ_Æï³Ë_°üÔú";
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
            rangetype = 5;
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
        virtual ~ Skill1801Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (205);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 60;
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
            skill->GetVictim ()->SetTime (50);
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetAmount (skill->GetPlayer ()->GetMaxhp () - 1);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak1 (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetRatio (3600);
            skill->GetVictim ()->SetAmount (1);
            skill->GetVictim ()->SetValue (22457);
            skill->GetVictim ()->SetCreateitem (1);
            return true;
        }
#endif
    };
}
#endif
