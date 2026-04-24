#ifndef __CPPGEN_GNET_SKILL221
#define __CPPGEN_GNET_SKILL221
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill221:public Skill
    {
      public:
        enum
        { SKILL_ID = 221 };
          Skill221 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill221Stub:public SkillStub
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill221Stub ():SkillStub (221)
        {
            occupation = 0;
            name = L"ÁÆÉË";
            nativename = "ÁÆÉË";
            icon = "ÁÆÉË.dds";
            maxlevel = 15;
            maxlearn = 6;
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
            skill_class = -1;
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
            talent[0] = 585;
            talent_size = 1;
            action[0] = "¹íÍõ×Ú_°üÔú";
            action[1] = "ÇàÔÆÃÅ_°üÔú";
            action[2] = "¹íÍõ×Ú_°üÔú";
            action[3] = "ºÏ»¶ÅÉ_°üÔú";
            action[4] = "ÌìÒôËÂ_°üÔú";
            action[5] = "¹íµÀ_°üÔú";
            action[6] = "ÇàÔÆÃÅ_°üÔú";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_°üÔú";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_°üÔú";
            action[17] = "¹íÍõ×Ú_Æï³Ë_°üÔú";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_°üÔú";
            action[19] = "ÌìÒôËÂ_Æï³Ë_°üÔú";
            action[20] = "¹íµÀ_Æï³Ë_°üÔú";
            action[21] = "ÇàÔÆÃÅ_Æï³Ë_°üÔú";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
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
        virtual ~ Skill221Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 219 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (15 + 10 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 25000 - 1000 * skill->GetLevel ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[6] = { 6, 8, 10, 12, 14, 15 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               15 + 10 * skill->GetLevel (),
                               15 + 10 * skill->GetLevel (), 8 + 8 * skill->GetLevel (), 64 + 64 * skill->GetLevel (), 25 - skill->GetLevel (), 10);

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
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetValue ((15 + 10 * skill->GetLevel ()) * (1 + 0.4 * skill->GetT0 ()));
            skill->GetVictim ()->SetHeal (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (8000 + 8000 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue ((64 + 64 * skill->GetLevel ()) * (1 + 0.4 * skill->GetT0 ()));
            skill->GetVictim ()->SetHpgen (1);
            return true;
        }
#endif
    };
}
#endif
