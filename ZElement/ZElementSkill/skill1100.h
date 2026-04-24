#ifndef __CPPGEN_GNET_SKILL1100
#define __CPPGEN_GNET_SKILL1100
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1100:public Skill
    {
      public:
        enum
        { SKILL_ID = 1100 };
          Skill1100 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1100Stub:public SkillStub
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
                skill->SetRatio (1 + 0.05 * skill->GetT1 ());
                skill->SetCrit (0.01 * skill->GetT3 ());
                skill->SetCrithurt (0.10 * skill->GetT3 ());
                skill->SetPlus (INT (1.42 - 0.05 * skill->GetLevel ()) * (45 + 3 * skill->GetLevel ()) +
                                INT (0.05 * skill->GetLevel () + 0.55) * (45 - 1.2 * skill->GetLevel () +
                                                                          0.66 * skill->GetLevel () * skill->GetLevel ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1100Stub ():SkillStub (1100)
        {
            occupation = 25;
            name = L"½þµØ¶¾Ã¢";
            nativename = "½þµØ¶¾Ã¢";
            icon = "½þµØ¶¾Ã¢.dds";
            maxlevel = 20;
            maxlearn = 9;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 1;
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
            skill_class = 4;
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
            talent[0] = 1142;
            talent[1] = 1143;
            talent[2] = 1151;
            talent[3] = 1155;
            talent_size = 4;
            action[0] = "¹íµÀ_½þµØ¶¾Ã¢";
            action[1] = "¹íµÀ_½þµØ¶¾Ã¢";
            action[2] = "¹íµÀ_½þµØ¶¾Ã¢";
            action[3] = "¹íµÀ_½þµØ¶¾Ã¢";
            action[4] = "¹íµÀ_½þµØ¶¾Ã¢";
            action[5] = "¹íµÀ_½þµØ¶¾Ã¢";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íµÀ_Æï³Ë_½þµØ¶¾Ã¢";
            action[16] = "¹íµÀ_Æï³Ë_½þµØ¶¾Ã¢";
            action[17] = "¹íµÀ_Æï³Ë_½þµØ¶¾Ã¢";
            action[18] = "¹íµÀ_Æï³Ë_½þµØ¶¾Ã¢";
            action[19] = "¹íµÀ_Æï³Ë_½þµØ¶¾Ã¢";
            action[20] = "¹íµÀ_Æï³Ë_½þµØ¶¾Ã¢";
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
            rangetype = 2;
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
        virtual ~ Skill1100Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1096 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 5 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (50 + 8 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6000 - 900 * skill->GetT2 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 30, 32, 34, 36, 38, 40, 42, 44, 47 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (5.7 + 0.3 * skill->GetLevel ());
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (5.7 + 0.3 * skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (3 + 0.3 * skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               (50 + 8 * skill->GetLevel ()),
                               12,
                               INT (4.7 + 0.3 * skill->GetLevel ()),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) * (45 + 3 * skill->GetLevel ()) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (45 - 1.2 * skill->GetLevel () +
                                                                              0.66 * skill->GetLevel () * skill->GetLevel ())), 8,
                               96 + 24 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (100 * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetTime (8000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount ((96 + 24 * skill->GetLevel ()) * (1 + 0.2 * skill->GetT0 ()));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (norm (zrand (100) / (100 - 3 * skill->GetT1 ())) * 600));
            skill->GetVictim ()->SetTime (3030 * skill->GetT1 ());
            skill->GetVictim ()->SetRatio (0.15);
            skill->GetVictim ()->SetBuffid (5);
            skill->GetVictim ()->SetWeak (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (12);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.24);
        }
#endif
    };
}
#endif
