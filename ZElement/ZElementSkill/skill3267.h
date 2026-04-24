#ifndef __CPPGEN_GNET_SKILL3267
#define __CPPGEN_GNET_SKILL3267
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3267:public Skill
    {
      public:
        enum
        { SKILL_ID = 3267 };
          Skill3267 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3267Stub:public SkillStub
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
      Skill3267Stub ():SkillStub (3267)
        {
            occupation = 128;
            name = L"ÌìÒô-Äé»¨ÃîÚĞ";
            nativename = "ÌìÒô-Äé»¨ÃîÚĞ";
            icon = "Äé»¨ÃîÚĞ.dds";
            maxlevel = 1;
            maxlearn = 1;
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
            talent[0] = 697;
            talent[1] = 702;
            talent[2] = 708;
            talent[3] = 713;
            talent_size = 4;
            action[0] = "ÌìÒôËÂ_Äé»¨ÃîÚĞ";
            action[1] = "ÌìÒôËÂ_Äé»¨ÃîÚĞ";
            action[2] = "ÌìÒôËÂ_Äé»¨ÃîÚĞ";
            action[3] = "ÌìÒôËÂ_Äé»¨ÃîÚĞ";
            action[4] = "ÌìÒôËÂ_Äé»¨ÃîÚĞ";
            action[5] = "ÌìÒôËÂ_Äé»¨ÃîÚĞ";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_Äé»¨ÃîÚĞ";
            action[16] = "ÌìÒôËÂ_Æï³Ë_Äé»¨ÃîÚĞ";
            action[17] = "ÌìÒôËÂ_Æï³Ë_Äé»¨ÃîÚĞ";
            action[18] = "ÌìÒôËÂ_Æï³Ë_Äé»¨ÃîÚĞ";
            action[19] = "ÌìÒôËÂ_Æï³Ë_Äé»¨ÃîÚĞ";
            action[20] = "ÌìÒôËÂ_Æï³Ë_Äé»¨ÃîÚĞ";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "¼¼ÄÜ¹¥»÷2";
            rangetype = 3;
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
        virtual ~ Skill3267Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
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
        float GetRadius (Skill * skill) const
        {
            return (float) (20);
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
            skill->GetVictim ()->SetProbability (1.0 * (750));
            skill->GetVictim ()->SetTime (12000);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetSleep (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetValue (zrand (50000) + 100000);
            skill->GetVictim ()->SetJuqi (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (20);
        }
#endif
    };
}
#endif
