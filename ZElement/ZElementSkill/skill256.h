#ifndef __CPPGEN_GNET_SKILL256
#define __CPPGEN_GNET_SKILL256
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill256:public Skill
    {
      public:
        enum
        { SKILL_ID = 256 };
          Skill256 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill256Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
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
                skill->SetRatio (1.05 + 0.02 * skill->GetT1 ());
                skill->
                    SetPlus ((INT (1.42 - 0.05 * skill->GetLevel ()) * (9.3 * skill->GetLevel () + 365) +
                              INT (0.05 * skill->GetLevel () + 0.55) * (3.5 * skill->GetLevel () * skill->GetLevel () - 45 * skill->GetLevel () +
                                                                        634)) * (1 + 0.07 * skill->GetT0 ()));
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
                skill->SetRatio (1.05 + 0.03 * skill->GetT1 ());
                skill->
                    SetPlus ((INT (1.42 - 0.05 * skill->GetLevel ()) * (9.3 * skill->GetLevel () + 365) +
                              INT (0.05 * skill->GetLevel () + 0.55) * (3.5 * skill->GetLevel () * skill->GetLevel () - 45 * skill->GetLevel () +
                                                                        634)) * (1 + 0.07 * skill->GetT0 ()));
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
                skill->SetRatio (1.05 + 0.04 * skill->GetT1 ());
                skill->
                    SetPlus ((INT (1.42 - 0.05 * skill->GetLevel ()) * (9.3 * skill->GetLevel () + 365) +
                              INT (0.05 * skill->GetLevel () + 0.55) * (3.5 * skill->GetLevel () * skill->GetLevel () - 45 * skill->GetLevel () +
                                                                        634)) * (1 + 0.07 * skill->GetT0 ()));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill256Stub ():SkillStub (256)
        {
            occupation = 3;
            name = L"ÌìÄ§Îè";
            nativename = "ÌìÄ§Îè";
            icon = "ÌìÄ§Îè.dds";
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
            talent[1] = 663;
            talent_size = 2;
            action[0] = "¹íÍõ×Ú_ÌìÄ§Îè";
            action[1] = "¹íÍõ×Ú_ÌìÄ§Îè";
            action[2] = "¹íÍõ×Ú_ÌìÄ§Îè";
            action[3] = "¹íÍõ×Ú_ÌìÄ§Îè";
            action[4] = "¹íÍõ×Ú_ÌìÄ§Îè";
            action[5] = "¹íÍõ×Ú_ÌìÄ§Îè";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_ÌìÄ§Îè";
            action[16] = "¹íÍõ×Ú_Æï³Ë_ÌìÄ§Îè";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ÌìÄ§Îè";
            action[18] = "¹íÍõ×Ú_Æï³Ë_ÌìÄ§Îè";
            action[19] = "¹íÍõ×Ú_Æï³Ë_ÌìÄ§Îè";
            action[20] = "¹íÍõ×Ú_Æï³Ë_ÌìÄ§Îè";
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
            doenchant = false;
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
            statestub.push_back (new State3 ());
            statestub.push_back (new State4 ());
#endif
        }
        virtual ~ Skill256Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 314 };
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
            return (float) (18 * skill->GetLevel () + 557);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 5000 - 750 * skill->GetT1 () - 750 * skill->GetT0 ();
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
                               INT (18 * skill->GetLevel () + 557),
                               5,
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) * (9.3 * skill->GetLevel () + 365) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (3.5 * skill->GetLevel () * skill->GetLevel () -
                                                                              45 * skill->GetLevel () + 634)));

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
#endif
    };
}
#endif
