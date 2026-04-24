#ifndef __CPPGEN_GNET_SKILL230
#define __CPPGEN_GNET_SKILL230
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill230:public Skill
    {
      public:
        enum
        { SKILL_ID = 230 };
          Skill230 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill230Stub:public SkillStub
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
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.05 * skill->GetT2 ());
                skill->
                    SetPlus ((INT (1.42 - 0.05 * skill->GetLevel ()) * (6 * skill->GetLevel () + 50) +
                              INT (0.05 * skill->GetLevel () + 0.55) * (0.32 * skill->GetLevel () * skill->GetLevel () + 5 * skill->GetLevel () +
                                                                        58)) * (1 + 0.1 * skill->GetT1 ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill230Stub ():SkillStub (230)
        {
            occupation = 8;
            name = L"Ğş±ù´Ì";
            nativename = "Ğş±ù´Ì";
            icon = "Ğş±ù´Ì.dds";
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
            talent[0] = 571;
            talent[1] = 602;
            talent[2] = 617;
            talent_size = 3;
            action[0] = "ÇàÔÆÃÅ_Ğş±ù´Ì";
            action[1] = "ÇàÔÆÃÅ_Ğş±ù´Ì";
            action[2] = "ÇàÔÆÃÅ_Ğş±ù´Ì";
            action[3] = "ÇàÔÆÃÅ_Ğş±ù´Ì";
            action[4] = "ÇàÔÆÃÅ_Ğş±ù´Ì";
            action[5] = "ÇàÔÆÃÅ_Ğş±ù´Ì";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_Ğş±ù´Ì";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_Ğş±ù´Ì";
            action[17] = "ÇàÔÆÃÅ_Æï³Ë_Ğş±ù´Ì";
            action[18] = "ÇàÔÆÃÅ_Æï³Ë_Ğş±ù´Ì";
            action[19] = "ÇàÔÆÃÅ_Æï³Ë_Ğş±ù´Ì";
            action[20] = "ÇàÔÆÃÅ_Æï³Ë_Ğş±ù´Ì";
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
            rangetype = 4;
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
        virtual ~ Skill230Stub ()
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
        float GetMpcost (Skill * skill) const
        {
            return (float) (0.35 * skill->GetLevel () * skill->GetLevel () + 5.1 * skill->GetLevel () + 89);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 1000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 45, 47, 49, 51, 53, 55, 57, 59, 61 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (6.3 + 0.2 * skill->GetLevel ());
        }
        float GetAngle (Skill * skill) const
        {
            return (float) (1 - 0.0111111 * (120));
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (6.3 + 0.2 * skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (3);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (0.35 * skill->GetLevel () * skill->GetLevel () + 5.1 * skill->GetLevel () + 89),
                               24,
                               5.8 + 0.2 * skill->GetLevel (),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) * (6 * skill->GetLevel () + 50) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (0.32 * skill->GetLevel () * skill->GetLevel () +
                                                                              5 * skill->GetLevel () + 58)), 18 + 4 * skill->GetLevel (),
                               2.5 + 0.5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (40 + 5 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (2500 + 100 + 500 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio (0.18 + 0.04 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetSlow (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (4000 * norm (skill->GetT0 ()) + 2000 * skill->GetT0 () + 100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (15 * skill->GetT0 ());
            skill->GetVictim ()->SetSubattack (1);
            skill->GetVictim ()->SetProbability (1.0 * (33));
            skill->GetVictim ()->SetValue (10);
            skill->GetVictim ()->SetRepel (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (24);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
