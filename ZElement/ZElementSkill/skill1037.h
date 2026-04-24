#ifndef __CPPGEN_GNET_SKILL1037
#define __CPPGEN_GNET_SKILL1037
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1037:public Skill
    {
      public:
        enum
        { SKILL_ID = 1037 };
          Skill1037 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1037Stub:public SkillStub
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
                skill->SetRatio (0);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1037Stub ():SkillStub (1037)
        {
            occupation = 129;
            name = L"Éñ»úÃÔ¾Ö";
            nativename = "Éñ»úÃÔ¾Ö";
            icon = "Éñ»úÃÔ¾Ö.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 3;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
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
            action[0] = "ÇàÔÆÃÅ_º®±ùÖä";
            action[1] = "ÇàÔÆÃÅ_º®±ùÖä";
            action[2] = "ÇàÔÆÃÅ_º®±ùÖä";
            action[3] = "ÇàÔÆÃÅ_º®±ùÖä";
            action[4] = "ÇàÔÆÃÅ_º®±ùÖä";
            action[5] = "0";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_º®±ùÖä";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_º®±ùÖä";
            action[17] = "ÇàÔÆÃÅ_Æï³Ë_º®±ùÖä";
            action[18] = "ÇàÔÆÃÅ_Æï³Ë_º®±ùÖä";
            action[19] = "ÇàÔÆÃÅ_Æï³Ë_º®±ùÖä";
            action[20] = "0";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
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
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill1037Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (16);
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
            return 100000;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (13);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (9);
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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetTmplid () == 13869 ? 2000 : 0));
            skill->GetVictim ()->SetTime (9500);
            skill->GetVictim ()->SetSilent (1);
            return true;
        }
#endif
    };
}
#endif
