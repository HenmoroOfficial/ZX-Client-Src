#ifndef __CPPGEN_GNET_SKILL3888
#define __CPPGEN_GNET_SKILL3888
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3888:public Skill
    {
      public:
        enum
        { SKILL_ID = 3888 };
          Skill3888 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3888Stub:public SkillStub
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
      Skill3888Stub ():SkillStub (3888)
        {
            occupation = 128;
            name = L"Éîº£Ðþ±ùÊõ";
            nativename = "Éîº£Ðþ±ùÊõ";
            icon = "±ù·âÊõ.dds";
            maxlevel = 1;
            maxlearn = 0;
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
            action[0] = "ÏÉ_±ù±¬";
            action[1] = "ÏÉ_±ù±¬";
            action[2] = "ÏÉ_±ù±¬";
            action[3] = "ÏÉ_±ù±¬";
            action[4] = "ÏÉ_±ù±¬";
            action[5] = "ÏÉ_±ù±¬";
            action[6] = "ÏÉ_±ù±¬";
            action[7] = "ÏÉ_±ù±¬";
            action[8] = "ÏÉ_±ù±¬";
            action[9] = "ÏÉ_±ù±¬";
            action[10] = "ÏÉ_±ù±¬";
            action[11] = "ÏÉ_±ù±¬";
            action[12] = "ÏÉ_±ù±¬";
            action[13] = "";
            action[14] = "";
            action[15] = "Æï³Ë_ÏÉ_±ù±¬";
            action[16] = "Æï³Ë_ÏÉ_±ù±¬";
            action[17] = "Æï³Ë_ÏÉ_±ù±¬";
            action[18] = "Æï³Ë_ÏÉ_±ù±¬";
            action[19] = "Æï³Ë_ÏÉ_±ù±¬";
            action[20] = "Æï³Ë_ÏÉ_±ù±¬";
            action[21] = "Æï³Ë_ÏÉ_±ù±¬";
            action[22] = "Æï³Ë_ÏÉ_±ù±¬";
            action[23] = "Æï³Ë_ÏÉ_±ù±¬";
            action[24] = "Æï³Ë_ÏÉ_±ù±¬";
            action[25] = "Æï³Ë_ÏÉ_±ù±¬";
            action[26] = "Æï³Ë_ÏÉ_±ù±¬";
            action[27] = "Æï³Ë_ÏÉ_±ù±¬";
            action[28] = "";
            action[29] = "";
            action[30] = "¼¼ÄÜ¹¥»÷1";
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
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill3888Stub ()
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
        int GetEnmity (Skill * skill) const
        {
            return 1;
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
            skill->GetVictim ()->SetProbability (1.0 *
                                                 ((skill->GetPlayer ()->GetTmplid () == 52300 || skill->GetPlayer ()->GetTmplid () == 52313
                                                   || skill->GetPlayer ()->GetTmplid () == 52314) ? 120 : 0));
            skill->GetVictim ()->SetTime (20000);
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetFrozen (1);
            return true;
        }
#endif
    };
}
#endif
