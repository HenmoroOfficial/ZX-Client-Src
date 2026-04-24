#ifndef __CPPGEN_GNET_SKILL306
#define __CPPGEN_GNET_SKILL306
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill306:public Skill
    {
      public:
        enum
        { SKILL_ID = 306 };
          Skill306 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill306Stub:public SkillStub
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
                skill->SetRatio (1 + 0.05 * skill->GetT1 ());
                skill->SetCrit (0.05 + 0.02 * skill->GetT0 ());
                skill->
                    SetPlus ((INT (1.42 - 0.05 * skill->GetLevel ()) *
                              (0.26 * skill->GetLevel () * skill->GetLevel () + 6.4 * skill->GetLevel () + 297) + INT (0.05 * skill->GetLevel () +
                                                                                                                       0.55) * (0.6 *
                                                                                                                                skill->GetLevel () *
                                                                                                                                skill->GetLevel () +
                                                                                                                                20 *
                                                                                                                                skill->GetLevel () +
                                                                                                                                192)) * (1 +
                                                                                                                                         0.12 *
                                                                                                                                         skill->
                                                                                                                                         GetT2 ()));
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
                skill->SetRatio (1 + 0.05 * skill->GetT1 ());
                skill->SetCrit (0.05 + 0.02 * skill->GetT0 ());
                skill->
                    SetPlus ((INT (1.42 - 0.05 * skill->GetLevel ()) *
                              (0.26 * skill->GetLevel () * skill->GetLevel () + 6.4 * skill->GetLevel () + 297) + INT (0.05 * skill->GetLevel () +
                                                                                                                       0.55) * (0.6 *
                                                                                                                                skill->GetLevel () *
                                                                                                                                skill->GetLevel () +
                                                                                                                                20 *
                                                                                                                                skill->GetLevel () +
                                                                                                                                192)) * (1 +
                                                                                                                                         0.12 *
                                                                                                                                         skill->
                                                                                                                                         GetT2 ()));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill306Stub ():SkillStub (306)
        {
            occupation = 6;
            name = L"µûÎè";
            nativename = "µûÎè";
            icon = "µûÎè.dds";
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
            talent[0] = 729;
            talent[1] = 732;
            talent[2] = 747;
            talent_size = 3;
            action[0] = "ºÏ»¶ÅÉ_µûÎè";
            action[1] = "ºÏ»¶ÅÉ_µûÎè";
            action[2] = "ºÏ»¶ÅÉ_µûÎè";
            action[3] = "ºÏ»¶ÅÉ_µûÎè";
            action[4] = "ºÏ»¶ÅÉ_µûÎè";
            action[5] = "ºÏ»¶ÅÉ_µûÎè";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_µûÎè";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_µûÎè";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_µûÎè";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_µûÎè";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_µûÎè";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_µûÎè";
            action[21] = "0";
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
        virtual ~ Skill306Stub ()
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
            return (float) (14);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0.82 * skill->GetLevel () * skill->GetLevel () + 14 * skill->GetLevel () + 302);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 0;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 75, 77, 78, 80, 83, 85, 87, 89, 91 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (8);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (0.82 * skill->GetLevel () * skill->GetLevel () + 14 * skill->GetLevel () + 302),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (0.26 * skill->GetLevel () * skill->GetLevel () + 6.4 * skill->GetLevel () + 297) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (0.6 * skill->GetLevel () * skill->GetLevel () +
                                                                              20 * skill->GetLevel () + 192)), 5);

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
            skill->GetVictim ()->SetProbability (1.0 * (20 * skill->GetT0 () * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetTime (4000 * norm (skill->GetT0 ()));
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount ((0.82 * skill->GetLevel () * skill->GetLevel () + 14 * skill->GetLevel () + 302) * skill->GetT0 ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetMpleak (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120 * skill->GetT2 ()));
            skill->GetVictim ()->SetTime (4000);
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->
                SetValue ((INT (1.42 - 0.05 * skill->GetLevel ()) *
                           (0.26 * skill->GetLevel () * skill->GetLevel () + 6.4 * skill->GetLevel () + 297) + INT (0.05 * skill->GetLevel () +
                                                                                                                    0.55) * (0.6 *
                                                                                                                             skill->GetLevel () *
                                                                                                                             skill->GetLevel () +
                                                                                                                             20 * skill->GetLevel () +
                                                                                                                             192)) * (1 +
                                                                                                                                      0.12 *
                                                                                                                                      skill->
                                                                                                                                      GetT2 ()) *
                          0.07 * skill->GetT2 ());
            skill->GetVictim ()->SetHpgen (1);
            return true;
        }
#endif
    };
}
#endif
