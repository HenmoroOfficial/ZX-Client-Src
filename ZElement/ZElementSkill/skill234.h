#ifndef __CPPGEN_GNET_SKILL234
#define __CPPGEN_GNET_SKILL234
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill234:public Skill
    {
      public:
        enum
        { SKILL_ID = 234 };
          Skill234 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill234Stub:public SkillStub
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
                skill->SetRatio (1 + 0.05 * skill->GetT3 () + INT (zrand (100) / (100 - 6 * skill->GetT1 ())) * 0.2 * skill->GetT1 ());
                skill->
                    SetPlus ((INT (1.42 - 0.05 * skill->GetLevel ()) * 0.9 * (8.6 * skill->GetLevel () + 324) +
                              INT (0.05 * skill->GetLevel () + 0.55) * 0.9 * (47 * skill->GetLevel () + 49)) * ((100 +
                                                                                                                 (zrand (10 + 15 * skill->GetT0 ()) -
                                                                                                                  10) * norm (skill->GetT0 () +
                                                                                                                              0.1)) / 100.0));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill234Stub ():SkillStub (234)
        {
            occupation = 8;
            name = L"À×ÔÆ·ç±©";
            nativename = "À×ÔÆ·ç±©";
            icon = "À×ÔÆ·ç±©.dds";
            maxlevel = 20;
            maxlearn = 9;
            type = 1;
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
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 2;
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
            talent[0] = 607;
            talent[1] = 609;
            talent[2] = 611;
            talent[3] = 617;
            talent_size = 4;
            action[0] = "ÇàÔÆÃÅ_À×ÔÆ·ç±©";
            action[1] = "ÇàÔÆÃÅ_À×ÔÆ·ç±©";
            action[2] = "ÇàÔÆÃÅ_À×ÔÆ·ç±©";
            action[3] = "ÇàÔÆÃÅ_À×ÔÆ·ç±©";
            action[4] = "ÇàÔÆÃÅ_À×ÔÆ·ç±©";
            action[5] = "ÇàÔÆÃÅ_À×ÔÆ·ç±©";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_À×ÔÆ·ç±©";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_À×ÔÆ·ç±©";
            action[17] = "ÇàÔÆÃÅ_Æï³Ë_À×ÔÆ·ç±©";
            action[18] = "ÇàÔÆÃÅ_Æï³Ë_À×ÔÆ·ç±©";
            action[19] = "ÇàÔÆÃÅ_Æï³Ë_À×ÔÆ·ç±©";
            action[20] = "ÇàÔÆÃÅ_Æï³Ë_À×ÔÆ·ç±©";
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
        virtual ~ Skill234Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 239 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 5 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1.2 * skill->GetLevel () * skill->GetLevel () + 2.7 * skill->GetLevel () + 100);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 2000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 60, 62, 64, 66, 68, 70, 72, 74, 76 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (12);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (9);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (4);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (1.2 * skill->GetLevel () * skill->GetLevel () + 2.7 * skill->GetLevel () + 100),
                               25,
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) * 0.9 * (8.6 * skill->GetLevel () + 324) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * 0.9 * (47 * skill->GetLevel () + 49)));

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
            skill->GetVictim ()->SetProbability (1.0 * (INT (zrand (100) / (100 - 4 * skill->GetT2 ())) * 600));
            skill->GetVictim ()->SetTime (3050 * skill->GetT2 ());
            skill->GetVictim ()->SetRatio (0.1 * skill->GetT2 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetWeak (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (25);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
