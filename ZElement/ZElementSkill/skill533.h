#ifndef __CPPGEN_GNET_SKILL533
#define __CPPGEN_GNET_SKILL533
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill533:public Skill
    {
      public:
        enum
        { SKILL_ID = 533 };
          Skill533 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill533Stub:public SkillStub
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
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrit (0.01);
                skill->SetRatio (1.12 + 0.03 * skill->GetT2 ());
                skill->
                    SetPlus ((INT (1.42 - 0.05 * skill->GetLevel ()) *
                              (0.067 * skill->GetLevel () * skill->GetLevel () + 11 * skill->GetLevel () + 434) + INT (0.05 * skill->GetLevel () +
                                                                                                                       0.55) * (94 *
                                                                                                                                skill->GetLevel () -
                                                                                                                                68 -
                                                                                                                                2.4 *
                                                                                                                                skill->GetLevel () *
                                                                                                                                skill->GetLevel ())) *
                             (1 + 0.02 * skill->GetT0 () * skill->GetLevel ()));
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
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrit (0.02);
                skill->SetRatio (1.12 + 0.03 * skill->GetT2 ());
                skill->
                    SetPlus ((INT (1.42 - 0.05 * skill->GetLevel ()) *
                              (0.067 * skill->GetLevel () * skill->GetLevel () + 11 * skill->GetLevel () + 434) + INT (0.05 * skill->GetLevel () +
                                                                                                                       0.55) * (94 *
                                                                                                                                skill->GetLevel () -
                                                                                                                                68 -
                                                                                                                                2.4 *
                                                                                                                                skill->GetLevel () *
                                                                                                                                skill->GetLevel ())) *
                             (1 + 0.02 * skill->GetT0 () * skill->GetLevel ()));
                skill->GetPlayer ()->SetPerform (0);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State4:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrit (0.03);
                skill->SetRatio (1.12 + 0.03 * skill->GetT2 ());
                skill->
                    SetPlus ((INT (1.42 - 0.05 * skill->GetLevel ()) *
                              (0.067 * skill->GetLevel () * skill->GetLevel () + 11 * skill->GetLevel () + 434) + INT (0.05 * skill->GetLevel () +
                                                                                                                       0.55) * (94 *
                                                                                                                                skill->GetLevel () -
                                                                                                                                68 -
                                                                                                                                2.4 *
                                                                                                                                skill->GetLevel () *
                                                                                                                                skill->GetLevel ())) *
                             (1 + 0.02 * skill->GetT0 () * skill->GetLevel ()));
                skill->GetPlayer ()->SetPerform (0);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State5:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrit (0.04);
                skill->SetRatio (1.12 + 0.03 * skill->GetT2 ());
                skill->
                    SetPlus ((INT (1.42 - 0.05 * skill->GetLevel ()) *
                              (0.067 * skill->GetLevel () * skill->GetLevel () + 11 * skill->GetLevel () + 434) + INT (0.05 * skill->GetLevel () +
                                                                                                                       0.55) * (94 *
                                                                                                                                skill->GetLevel () -
                                                                                                                                68 -
                                                                                                                                2.4 *
                                                                                                                                skill->GetLevel () *
                                                                                                                                skill->GetLevel ())) *
                             (1 + 0.02 * skill->GetT0 () * skill->GetLevel ()));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill533Stub ():SkillStub (533)
        {
            occupation = 16;
            name = L"江南";
            nativename = "江南";
            icon = "江南.dds";
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
            talent[0] = 735;
            talent[1] = 750;
            talent[2] = 761;
            talent_size = 3;
            action[0] = "合欢派_江南";
            action[1] = "合欢派_江南";
            action[2] = "合欢派_江南";
            action[3] = "合欢派_江南";
            action[4] = "合欢派_江南";
            action[5] = "合欢派_江南";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "合欢派_骑乘_江南";
            action[16] = "合欢派_骑乘_江南";
            action[17] = "合欢派_骑乘_江南";
            action[18] = "合欢派_骑乘_江南";
            action[19] = "合欢派_骑乘_江南";
            action[20] = "合欢派_骑乘_江南";
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
            statestub.push_back (new State4 ());
            statestub.push_back (new State5 ());
#endif
        }
        virtual ~ Skill533Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 531 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 5 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (2.4 * skill->GetLevel () * skill->GetLevel () + 2.2 * skill->GetLevel () + 840);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 4000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 8000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 120, 122, 124, 125, 125, 125, 125, 125, 125 };
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
                               INT (2.4 * skill->GetLevel () * skill->GetLevel () + 2.2 * skill->GetLevel () + 840),
                               12,
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (0.067 * skill->GetLevel () * skill->GetLevel () + 11 * skill->GetLevel () + 434) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (94 * skill->GetLevel () - 68 -
                                                                              2.4 * skill->GetLevel () * skill->GetLevel ())), 25);

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
            skill->GetVictim ()->SetProbability (1.0 * (15 * skill->GetT1 ()));
            skill->GetVictim ()->SetTime (8000 + 100);
            skill->GetVictim ()->SetRatio (0.2 * skill->GetT1 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetSlow (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (8000 + 100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (35 * skill->GetT1 ());
            skill->GetVictim ()->SetSubattack (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (25 + 15 * skill->GetT0 ()));
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (99);
            skill->GetVictim ()->SetValue (99);
            skill->GetVictim ()->SetExorcism (1);
            return true;
        }
#endif
    };
}
#endif
