#ifndef __CPPGEN_GNET_SKILL546
#define __CPPGEN_GNET_SKILL546
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill546:public Skill
    {
      public:
        enum
        { SKILL_ID = 546 };
          Skill546 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill546Stub:public SkillStub
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
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.07 * skill->GetT1 ());
                skill->
                    SetPlus ((INT (1.42 - 0.05 * skill->GetLevel ()) *
                              (0.071 * skill->GetLevel () * skill->GetLevel () + 13 * skill->GetLevel () + 623) + INT (0.05 * skill->GetLevel () +
                                                                                                                       0.55) * (2.1 *
                                                                                                                                skill->GetLevel () *
                                                                                                                                skill->GetLevel () -
                                                                                                                                0.043 *
                                                                                                                                skill->GetLevel () +
                                                                                                                                663)) * (1 +
                                                                                                                                         0.15 *
                                                                                                                                         skill->
                                                                                                                                         GetT0 ()) *
                             (1 + INT (zrand (99 + 7 * skill->GetT2 ()) / 100)));
                skill->GetPlayer ()->SetPerform (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State3:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.07 * skill->GetT1 ());
                skill->
                    SetPlus ((INT (1.42 - 0.05 * skill->GetLevel ()) *
                              (0.071 * skill->GetLevel () * skill->GetLevel () + 13 * skill->GetLevel () + 623) + INT (0.05 * skill->GetLevel () +
                                                                                                                       0.55) * (2.1 *
                                                                                                                                skill->GetLevel () *
                                                                                                                                skill->GetLevel () -
                                                                                                                                0.043 *
                                                                                                                                skill->GetLevel () +
                                                                                                                                663)) * (1 +
                                                                                                                                         0.15 *
                                                                                                                                         skill->
                                                                                                                                         GetT0 ()) *
                             (1 + INT (zrand (99 + 7 * skill->GetT2 ()) / 100)));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill546Stub ():SkillStub (546)
        {
            occupation = 22;
            name = L"Äé»¨ÃîÚÐ";
            nativename = "Äé»¨ÃîÚÐ";
            icon = "Äé»¨ÃîÚÐ.dds";
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
            talent[0] = 697;
            talent[1] = 702;
            talent[2] = 708;
            talent[3] = 713;
            talent_size = 4;
            action[0] = "ÌìÒôËÂ_Äé»¨ÃîÚÐ";
            action[1] = "ÌìÒôËÂ_Äé»¨ÃîÚÐ";
            action[2] = "ÌìÒôËÂ_Äé»¨ÃîÚÐ";
            action[3] = "ÌìÒôËÂ_Äé»¨ÃîÚÐ";
            action[4] = "ÌìÒôËÂ_Äé»¨ÃîÚÐ";
            action[5] = "ÌìÒôËÂ_Äé»¨ÃîÚÐ";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_Äé»¨ÃîÚÐ";
            action[16] = "ÌìÒôËÂ_Æï³Ë_Äé»¨ÃîÚÐ";
            action[17] = "ÌìÒôËÂ_Æï³Ë_Äé»¨ÃîÚÐ";
            action[18] = "ÌìÒôËÂ_Æï³Ë_Äé»¨ÃîÚÐ";
            action[19] = "ÌìÒôËÂ_Æï³Ë_Äé»¨ÃîÚÐ";
            action[20] = "ÌìÒôËÂ_Æï³Ë_Äé»¨ÃîÚÐ";
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
            statestub.push_back (new State3 ());
#endif
        }
        virtual ~ Skill546Stub ()
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
            return (float) (19 + skill->GetLevel ());
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1.3 * skill->GetLevel () * skill->GetLevel () + 6 * skill->GetLevel () + 910);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 7000;
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
        float GetPraydistance (Skill * skill) const
        {
            return (float) (16 + skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10.7 + skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (1.3 * skill->GetLevel () * skill->GetLevel () + 6 * skill->GetLevel () + 910),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (0.071 * skill->GetLevel () * skill->GetLevel () + 13 * skill->GetLevel () + 623) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (2.1 * skill->GetLevel () * skill->GetLevel () -
                                                                              0.043 * skill->GetLevel () + 663)), 5 + 2 * skill->GetLevel (),
                               24 * skill->GetLevel (), skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (4000 + 100 + 2000 * skill->GetT3 ());
            skill->GetVictim ()->SetRatio (0.05 + (0.02 + 0.01 * skill->GetT3 ()) * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecaccuracy (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetValue (24 * skill->GetLevel () * (1 + skill->GetT2 ()));
            skill->GetVictim ()->SetJuqi (1);
            return true;
        }
#endif
    };
}
#endif
