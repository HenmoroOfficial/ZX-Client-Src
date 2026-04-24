#ifndef __CPPGEN_GNET_SKILL525
#define __CPPGEN_GNET_SKILL525
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill525:public Skill
    {
      public:
        enum
        { SKILL_ID = 525 };
          Skill525 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill525Stub:public SkillStub
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
                skill->SetRatio (1.08);
                skill->
                    SetPlus ((INT
                              (INT (1.42 - 0.05 * skill->GetLevel ()) *
                               (0.078 * skill->GetLevel () * skill->GetLevel () + 13 * skill->GetLevel () + 490) + INT (0.05 * skill->GetLevel () +
                                                                                                                        0.55) * (118 *
                                                                                                                                 skill->GetLevel () -
                                                                                                                                 144 -
                                                                                                                                 3 *
                                                                                                                                 skill->GetLevel () *
                                                                                                                                 skill->
                                                                                                                                 GetLevel ()))) * (1 +
                                                                                                                                                   0.07
                                                                                                                                                   *
                                                                                                                                                   skill->
                                                                                                                                                   GetT0
                                                                                                                                                   ()));
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
                skill->SetRatio (1.08);
                skill->
                    SetPlus ((INT
                              (INT (1.42 - 0.05 * skill->GetLevel ()) *
                               (0.078 * skill->GetLevel () * skill->GetLevel () + 13 * skill->GetLevel () + 490) + INT (0.05 * skill->GetLevel () +
                                                                                                                        0.55) * (118 *
                                                                                                                                 skill->GetLevel () -
                                                                                                                                 144 -
                                                                                                                                 3 *
                                                                                                                                 skill->GetLevel () *
                                                                                                                                 skill->
                                                                                                                                 GetLevel ()))) * (1 +
                                                                                                                                                   0.07
                                                                                                                                                   *
                                                                                                                                                   skill->
                                                                                                                                                   GetT0
                                                                                                                                                   ()));
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
                skill->SetRatio (1.08);
                skill->
                    SetPlus ((INT
                              (INT (1.42 - 0.05 * skill->GetLevel ()) *
                               (0.078 * skill->GetLevel () * skill->GetLevel () + 13 * skill->GetLevel () + 490) + INT (0.05 * skill->GetLevel () +
                                                                                                                        0.55) * (118 *
                                                                                                                                 skill->GetLevel () -
                                                                                                                                 144 -
                                                                                                                                 3 *
                                                                                                                                 skill->GetLevel () *
                                                                                                                                 skill->
                                                                                                                                 GetLevel ()))) * (1 +
                                                                                                                                                   0.07
                                                                                                                                                   *
                                                                                                                                                   skill->
                                                                                                                                                   GetT0
                                                                                                                                                   ()));
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
                skill->SetRatio (1.08);
                skill->
                    SetPlus ((INT
                              (INT (1.42 - 0.05 * skill->GetLevel ()) *
                               (0.078 * skill->GetLevel () * skill->GetLevel () + 13 * skill->GetLevel () + 490) + INT (0.05 * skill->GetLevel () +
                                                                                                                        0.55) * (118 *
                                                                                                                                 skill->GetLevel () -
                                                                                                                                 144 -
                                                                                                                                 3 *
                                                                                                                                 skill->GetLevel () *
                                                                                                                                 skill->
                                                                                                                                 GetLevel ()))) * (1 +
                                                                                                                                                   0.07
                                                                                                                                                   *
                                                                                                                                                   skill->
                                                                                                                                                   GetT0
                                                                                                                                                   ()));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill525Stub ():SkillStub (525)
        {
            occupation = 13;
            name = L"ÐýÁú»ÃÉ±";
            nativename = "ÐýÁú»ÃÉ±";
            icon = "ÐýÁú»ÃÉ±.dds";
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
            skill_class = 0;
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
            talent[0] = 661;
            talent[1] = 665;
            talent_size = 2;
            action[0] = "¹íÍõ×Ú_ÐýÁú»ÃÉ±";
            action[1] = "¹íÍõ×Ú_ÐýÁú»ÃÉ±";
            action[2] = "¹íÍõ×Ú_ÐýÁú»ÃÉ±";
            action[3] = "¹íÍõ×Ú_ÐýÁú»ÃÉ±";
            action[4] = "¹íÍõ×Ú_ÐýÁú»ÃÉ±";
            action[5] = "¹íÍõ×Ú_ÐýÁú»ÃÉ±";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_ÐýÁú»ÃÉ±";
            action[16] = "¹íÍõ×Ú_Æï³Ë_ÐýÁú»ÃÉ±";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ÐýÁú»ÃÉ±";
            action[18] = "¹íÍõ×Ú_Æï³Ë_ÐýÁú»ÃÉ±";
            action[19] = "¹íÍõ×Ú_Æï³Ë_ÐýÁú»ÃÉ±";
            action[20] = "¹íÍõ×Ú_Æï³Ë_ÐýÁú»ÃÉ±";
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
        virtual ~ Skill525Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 522 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 5 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (14);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (2.4 * skill->GetLevel () * skill->GetLevel () - 1.6 * skill->GetLevel () + 920);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 4000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 10000 - 1500 * skill->GetT0 ();
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
                               INT (2.4 * skill->GetLevel () * skill->GetLevel () - 1.6 * skill->GetLevel () + 920),
                               8,
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (0.078 * skill->GetLevel () * skill->GetLevel () + 13 * skill->GetLevel () + 490) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (118 * skill->GetLevel () - 144 -
                                                                              3 * skill->GetLevel () * skill->GetLevel ())),
                               3 + 3 * skill->GetLevel (), 3, 9 + 3 * skill->GetLevel (), 8, skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (3 + 3 * skill->GetLevel () + 7 * skill->GetT1 ()));
            skill->GetVictim ()->SetTime (3000 + 100 + 3000 * skill->GetT1 ());
            skill->GetVictim ()->SetBarehanded (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (9 + 3 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (8000);
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0.01 * skill->GetLevel () * skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetHpgen (1);
            return true;
        }
#endif
    };
}
#endif
