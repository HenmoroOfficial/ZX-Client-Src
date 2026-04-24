#ifndef __CPPGEN_GNET_SKILL554
#define __CPPGEN_GNET_SKILL554
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill554:public Skill
    {
      public:
        enum
        { SKILL_ID = 554 };
          Skill554 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill554Stub:public SkillStub
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
                skill->SetCrit (0.01 * skill->GetT2 ());
                skill->SetRatio (1 + 0.05 * skill->GetT0 () + 0.1 * skill->GetT2 ());
                skill->SetPlus (INT (1.42 - 0.05 * skill->GetLevel ()) * 0.9 * (280 + 13 * skill->GetLevel ()) +
                                INT (0.05 * skill->GetLevel () + 0.55) * 0.9 * (15 * skill->GetLevel () + 300));
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
                skill->SetCrit (0.01 * skill->GetT2 ());
                skill->SetRatio (1 + 0.05 * skill->GetT0 () + 0.1 * skill->GetT2 ());
                skill->SetPlus (INT (1.42 - 0.05 * skill->GetLevel ()) * 0.9 * (280 + 13 * skill->GetLevel ()) +
                                INT (0.05 * skill->GetLevel () + 0.55) * 0.9 * (15 * skill->GetLevel () + 300));
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
                skill->SetCrit (0.01 * skill->GetT2 ());
                skill->SetRatio (1 + 0.05 * skill->GetT0 () + 0.1 * skill->GetT2 ());
                skill->SetPlus (INT (1.42 - 0.05 * skill->GetLevel ()) * 0.9 * (280 + 13 * skill->GetLevel ()) +
                                INT (0.05 * skill->GetLevel () + 0.55) * 0.9 * (15 * skill->GetLevel () + 300));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill554Stub ():SkillStub (554)
        {
            occupation = 22;
            name = L"·ðÍÓ¾»ÊÀÔ¸";
            nativename = "·ðÍÓ¾»ÊÀÔ¸";
            icon = "·ðÍÓ¾»ÊÀÔ¸.dds";
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
            talent[0] = 681;
            talent[1] = 684;
            talent[2] = 716;
            talent_size = 3;
            action[0] = "ÌìÒôËÂ_·ðÍÓ¾»ÊÀÔ¸";
            action[1] = "ÌìÒôËÂ_·ðÍÓ¾»ÊÀÔ¸";
            action[2] = "ÌìÒôËÂ_·ðÍÓ¾»ÊÀÔ¸";
            action[3] = "ÌìÒôËÂ_·ðÍÓ¾»ÊÀÔ¸";
            action[4] = "ÌìÒôËÂ_·ðÍÓ¾»ÊÀÔ¸";
            action[5] = "ÌìÒôËÂ_·ðÍÓ¾»ÊÀÔ¸";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_·ðÍÓ¾»ÊÀÔ¸";
            action[16] = "ÌìÒôËÂ_Æï³Ë_·ðÍÓ¾»ÊÀÔ¸";
            action[17] = "ÌìÒôËÂ_Æï³Ë_·ðÍÓ¾»ÊÀÔ¸";
            action[18] = "ÌìÒôËÂ_Æï³Ë_·ðÍÓ¾»ÊÀÔ¸";
            action[19] = "ÌìÒôËÂ_Æï³Ë_·ðÍÓ¾»ÊÀÔ¸";
            action[20] = "ÌìÒôËÂ_Æï³Ë_·ðÍÓ¾»ÊÀÔ¸";
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
        virtual ~ Skill554Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 550 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 6 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (22);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1.7 * skill->GetLevel () * skill->GetLevel () + 1.5 * skill->GetLevel () + 500);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 8000 - 1200 * skill->GetT1 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 125, 125, 125, 125, 125, 125, 125, 125, 125 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (12);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (18);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (1.7 * skill->GetLevel () * skill->GetLevel () + 1.5 * skill->GetLevel () + 500),
                               30,
                               12,
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) * 0.9 * (280 + 13 * skill->GetLevel ()) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * 0.9 * (15 * skill->GetLevel () + 300)));

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        int GetCoverage (Skill * skill) const
        {
            return (int) (30);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
