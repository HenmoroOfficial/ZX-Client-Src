#ifndef __CPPGEN_GNET_SKILL303
#define __CPPGEN_GNET_SKILL303
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill303:public Skill
    {
      public:
        enum
        { SKILL_ID = 303 };
          Skill303 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill303Stub:public SkillStub
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
            void Calculate (Skill * skill) const
            {
                skill->SetCrit (0.01 * skill->GetT3 ());
                skill->SetCrithurt (0.1 * skill->GetT3 ());
                skill->SetRatio (1.77 + 0.04 * skill->GetLevel () + 0.05 * skill->GetT0 ());
                skill->SetPlus (1.2 * skill->GetT1 () * skill->GetPlayer ()->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill303Stub ():SkillStub (303)
        {
            occupation = 6;
            name = L"²øÃà";
            nativename = "²øÃà";
            icon = "²øÃà.dds";
            maxlevel = 20;
            maxlearn = 9;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 0;
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
            skill_class = 1;
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
            preskillnum = 1;
            talent[0] = 742;
            talent[1] = 746;
            talent[2] = 749;
            talent[3] = 753;
            talent_size = 4;
            action[0] = "ºÏ»¶ÅÉ_²øÃà";
            action[1] = "ºÏ»¶ÅÉ_²øÃà";
            action[2] = "ºÏ»¶ÅÉ_²øÃà";
            action[3] = "ºÏ»¶ÅÉ_²øÃà";
            action[4] = "ºÏ»¶ÅÉ_²øÃà";
            action[5] = "ºÏ»¶ÅÉ_²øÃà";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_²øÃà";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_²øÃà";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_²øÃà";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_²øÃà";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_²øÃà";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_²øÃà";
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
            doenchant = false;
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
        virtual ~ Skill303Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 467 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0.82 * skill->GetLevel () * skill->GetLevel () + 3.2 * skill->GetLevel () + 245);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 2000 - 400 * skill->GetT2 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 85, 87, 89, 91, 93, 95, 97, 99, 101 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (7);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (0.82 * skill->GetLevel () * skill->GetLevel () + 3.2 * skill->GetLevel () + 245), 77 + 4 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
#endif
    };
}
#endif
