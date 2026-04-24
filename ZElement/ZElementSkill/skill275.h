#ifndef __CPPGEN_GNET_SKILL275
#define __CPPGEN_GNET_SKILL275
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill275:public Skill
    {
      public:
        enum
        { SKILL_ID = 275 };
          Skill275 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill275Stub:public SkillStub
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
                skill->SetRatio (1 + 0.07 * skill->GetT2 ());
                skill->GetPlayer ()->SetHp (skill->GetPlayer ()->GetHp () * 0.9);
                skill->
                    SetPlus ((INT (1.42 - 0.05 * skill->GetLevel ()) *
                              (1690 + 31 * skill->GetLevel () - 0.29 * skill->GetLevel () * skill->GetLevel ()) + INT (0.05 * skill->GetLevel () +
                                                                                                                       0.55) * (13 *
                                                                                                                                skill->GetLevel () *
                                                                                                                                skill->GetLevel () -
                                                                                                                                180 *
                                                                                                                                skill->GetLevel () +
                                                                                                                                2740)) * (1 +
                                                                                                                                          0.12 *
                                                                                                                                          skill->
                                                                                                                                          GetT0 ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill275Stub ():SkillStub (275)
        {
            occupation = 12;
            name = L"Í¬¹é¼«ÀÖ";
            nativename = "Í¬¹é¼«ÀÖ";
            icon = "Í¬¹é¼«ÀÖ.dds";
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
            skill_class = 3;
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
            talent[0] = 696;
            talent[1] = 698;
            talent[2] = 702;
            talent_size = 3;
            action[0] = "ÌìÒôËÂ_Í¬¹é¼«ÀÖ";
            action[1] = "ÌìÒôËÂ_Í¬¹é¼«ÀÖ";
            action[2] = "ÌìÒôËÂ_Í¬¹é¼«ÀÖ";
            action[3] = "ÌìÒôËÂ_Í¬¹é¼«ÀÖ";
            action[4] = "ÌìÒôËÂ_Í¬¹é¼«ÀÖ";
            action[5] = "ÌìÒôËÂ_Í¬¹é¼«ÀÖ";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_Í¬¹é¼«ÀÖ";
            action[16] = "ÌìÒôËÂ_Æï³Ë_Í¬¹é¼«ÀÖ";
            action[17] = "ÌìÒôËÂ_Æï³Ë_Í¬¹é¼«ÀÖ";
            action[18] = "ÌìÒôËÂ_Æï³Ë_Í¬¹é¼«ÀÖ";
            action[19] = "ÌìÒôËÂ_Æï³Ë_Í¬¹é¼«ÀÖ";
            action[20] = "ÌìÒôËÂ_Æï³Ë_Í¬¹é¼«ÀÖ";
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
        virtual ~ Skill275Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 319 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 4 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (18);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (4.1 * skill->GetLevel () * skill->GetLevel () - 22 * skill->GetLevel () + 880);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 10000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 90, 92, 94, 96, 98, 100, 102, 104, 106 };
            return array[level - 1];
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
            return _snwprintf (buffer, length, format,
                               INT (4.1 * skill->GetLevel () * skill->GetLevel () - 22 * skill->GetLevel () + 880),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (1690 + 31 * skill->GetLevel () - 0.29 * skill->GetLevel () * skill->GetLevel ()) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (13 * skill->GetLevel () * skill->GetLevel () -
                                                                              180 * skill->GetLevel () + 2740)));

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
            skill->GetVictim ()->SetProbability (1.0 * (6 * skill->GetT1 ()));
            skill->GetVictim ()->SetTime (4050 * skill->GetT1 ());
            skill->GetVictim ()->SetRatio (0.03 * skill->GetT1 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecdefence (1);
            return true;
        }
#endif
    };
}
#endif
