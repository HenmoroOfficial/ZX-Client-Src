#ifndef __CPPGEN_GNET_SKILL1484
#define __CPPGEN_GNET_SKILL1484
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1484:public Skill
    {
      public:
        enum
        { SKILL_ID = 1484 };
          Skill1484 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1484Stub:public SkillStub
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
                skill->SetRatio (0);
                skill->SetPlus (1);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1484Stub ():SkillStub (1484)
        {
            occupation = 129;
            name = L"Ãé×¼Éä»÷";
            nativename = "Ãé×¼Éä»÷";
            icon = "Ãé×¼Éä»÷.dds";
            maxlevel = 1;
            maxlearn = 10;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
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
            action[0] = "ÇàÔÆÃÅ_Å­½£¿ñ»¨";
            action[1] = "ÇàÔÆÃÅ_Å­½£¿ñ»¨";
            action[2] = "ÇàÔÆÃÅ_Å­½£¿ñ»¨";
            action[3] = "ÇàÔÆÃÅ_Å­½£¿ñ»¨";
            action[4] = "ÇàÔÆÃÅ_Å­½£¿ñ»¨";
            action[5] = "ÇàÔÆÃÅ_Å­½£¿ñ»¨";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_Å­½£¿ñ»¨";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_Å­½£¿ñ»¨";
            action[17] = "ÇàÔÆÃÅ_Æï³Ë_Å­½£¿ñ»¨";
            action[18] = "ÇàÔÆÃÅ_Æï³Ë_Å­½£¿ñ»¨";
            action[19] = "ÇàÔÆÃÅ_Æï³Ë_Å­½£¿ñ»¨";
            action[20] = "ÇàÔÆÃÅ_Æï³Ë_Å­½£¿ñ»¨";
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
        virtual ~ Skill1484Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (40);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (100);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 5000;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (35);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (30);
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
            skill->GetVictim ()->SetRatio (skill->GetPlayer ()->GetTmplid () == 19195 ? 99999 : 0);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetTmplid () == 19195 ? 1 : 0);
            skill->GetVictim ()->SetSecondattack (1);
            return true;
        }
#endif
    };
}
#endif
