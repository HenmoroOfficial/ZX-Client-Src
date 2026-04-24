#ifndef __CPPGEN_GNET_SKILL537
#define __CPPGEN_GNET_SKILL537
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill537:public Skill
    {
      public:
        enum
        { SKILL_ID = 537 };
          Skill537 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill537Stub:public SkillStub
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
                skill->SetRatio (1 + 0.05 * skill->GetT3 ());
                skill->SetPlus ((1 + 0.1 * skill->GetT0 ()) *
                                (INT (1.42 - 0.05 * skill->GetLevel ()) * 0.9 *
                                 (0.072 * skill->GetLevel () * skill->GetLevel () + 14 * skill->GetLevel () + 791) + INT (0.05 * skill->GetLevel () +
                                                                                                                          0.55) * 0.9 * (2.6 *
                                                                                                                                         skill->
                                                                                                                                         GetLevel () *
                                                                                                                                         skill->
                                                                                                                                         GetLevel () -
                                                                                                                                         2.9 *
                                                                                                                                         skill->
                                                                                                                                         GetLevel () +
                                                                                                                                         854)) *
                                ((100 + (zrand (10 + 15 * skill->GetT1 ()) - 10) * norm (skill->GetT1 () + 0.1)) / 100.0));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill537Stub ():SkillStub (537)
        {
            occupation = 19;
            name = L"ËªÌìÑ©Îè";
            nativename = "ËªÌìÑ©Îè";
            icon = "ËªÌìÑ©Îè.dds";
            maxlevel = 18;
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
            talent[0] = 602;
            talent[1] = 606;
            talent[2] = 610;
            talent[3] = 617;
            talent_size = 4;
            action[0] = "ÇàÔÆÃÅ_ËªÌìÑ©Îè";
            action[1] = "ÇàÔÆÃÅ_ËªÌìÑ©Îè";
            action[2] = "ÇàÔÆÃÅ_ËªÌìÑ©Îè";
            action[3] = "ÇàÔÆÃÅ_ËªÌìÑ©Îè";
            action[4] = "ÇàÔÆÃÅ_ËªÌìÑ©Îè";
            action[5] = "ÇàÔÆÃÅ_ËªÌìÑ©Îè";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_ËªÌìÑ©Îè";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_ËªÌìÑ©Îè";
            action[17] = "ÇàÔÆÃÅ_Æï³Ë_ËªÌìÑ©Îè";
            action[18] = "ÇàÔÆÃÅ_Æï³Ë_ËªÌìÑ©Îè";
            action[19] = "ÇàÔÆÃÅ_Æï³Ë_ËªÌìÑ©Îè";
            action[20] = "ÇàÔÆÃÅ_Æï³Ë_ËªÌìÑ©Îè";
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
            rangetype = 3;
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
        virtual ~ Skill537Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 0 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 24 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (21);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1.1 * skill->GetLevel () * skill->GetLevel () - 3.1 * skill->GetLevel () + 330);
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
            static int array[9] = { 105, 107, 109, 111, 113, 115, 117, 119, 122 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (16);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (16);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (13);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (1.1 * skill->GetLevel () * skill->GetLevel () - 3.1 * skill->GetLevel () + 330),
                               36,
                               15,
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) * 0.9 *
                                    (0.072 * skill->GetLevel () * skill->GetLevel () + 14 * skill->GetLevel () + 791) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * 0.9 * (2.6 * skill->GetLevel () * skill->GetLevel () -
                                                                                    2.9 * skill->GetLevel () + 854)), 32 + 3 * skill->GetLevel (), 25,
                               4, 20 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (10 + 32 + 3 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (4000 + 100 + 10000 * skill->GetT1 ());
            skill->GetVictim ()->SetRatio (0.25);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetSlow (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (2000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0.2 * skill->GetLevel () * skill->GetPlayer ()->GetLevel ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (5 * skill->GetT2 ()));
            skill->GetVictim ()->SetTime (3050 * skill->GetT2 ());
            skill->GetVictim ()->SetRatio (0.05 * skill->GetT2 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecdefence (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (36);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
