#ifndef __CPPGEN_GNET_SKILL1113
#define __CPPGEN_GNET_SKILL1113
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1113:public Skill
    {
      public:
        enum
        { SKILL_ID = 1113 };
          Skill1113 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1113Stub:public SkillStub
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
                skill->SetRatio (1 + 0.04 * skill->GetT0 () + 0.02 * skill->GetT4 () + norm (zrand (130) / 100.0));
                skill->SetCrit (0.01 * skill->GetT3 () + 0.03 * norm (skill->GetT4 () / 9.9));
                skill->SetCrithurt (0.10 * skill->GetT3 ());
                skill->SetPlus (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                (1.4 * skill->GetLevel () * skill->GetLevel () + 7.9 * skill->GetLevel () + 677) + INT (0.05 * skill->GetLevel () +
                                                                                                                        0.55) * (0.72 *
                                                                                                                                 skill->GetLevel () *
                                                                                                                                 skill->GetLevel () +
                                                                                                                                 62 *
                                                                                                                                 skill->GetLevel () +
                                                                                                                                 348));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1113Stub ():SkillStub (1113)
        {
            occupation = 27;
            name = L"±©É³Âş¾í";
            nativename = "±©É³Âş¾í";
            icon = "±©É³Âş¾í.dds";
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
            talent[0] = 1148;
            talent[1] = 1149;
            talent[2] = 1151;
            talent[3] = 1155;
            talent[4] = 1543;
            talent_size = 5;
            action[0] = "¹íµÀ_±©É³Âş¾í";
            action[1] = "¹íµÀ_±©É³Âş¾í";
            action[2] = "¹íµÀ_±©É³Âş¾í";
            action[3] = "¹íµÀ_±©É³Âş¾í";
            action[4] = "¹íµÀ_±©É³Âş¾í";
            action[5] = "¹íµÀ_±©É³Âş¾í";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íµÀ_Æï³Ë_±©É³Âş¾í";
            action[16] = "¹íµÀ_Æï³Ë_±©É³Âş¾í";
            action[17] = "¹íµÀ_Æï³Ë_±©É³Âş¾í";
            action[18] = "¹íµÀ_Æï³Ë_±©É³Âş¾í";
            action[19] = "¹íµÀ_Æï³Ë_±©É³Âş¾í";
            action[20] = "¹íµÀ_Æï³Ë_±©É³Âş¾í";
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
        virtual ~ Skill1113Stub ()
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
            return (float) (1.1 * skill->GetLevel () * skill->GetLevel () + 14 * skill->GetLevel () + 315);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6000 - 900 * skill->GetT2 () - 300 * skill->GetT4 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 75, 77, 79, 81, 83, 85, 87, 89, 92 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (6.8 + 0.2 * skill->GetLevel ());
        }
        float GetAngle (Skill * skill) const
        {
            return (float) (1 - 0.0111111 * (120));
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (6.8 + 0.2 * skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (3.8 + 0.2 * skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (1.1 * skill->GetLevel () * skill->GetLevel () + 14 * skill->GetLevel () + 315),
                               20,
                               6.8 + 0.2 * skill->GetLevel (),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (1.4 * skill->GetLevel () * skill->GetLevel () + 7.9 * skill->GetLevel () + 677) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (0.72 * skill->GetLevel () * skill->GetLevel () +
                                                                              62 * skill->GetLevel () + 348)), 33, 5 + 10 * skill->GetLevel (), 8);

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
            skill->GetVictim ()->SetProbability (1.0 * (33 + 5 * skill->GetT1 ()));
            skill->GetVictim ()->SetTime (8000 + 100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue ((5 + 10 * skill->GetLevel ()) * (1 + 0.3 * skill->GetT1 ()));
            skill->GetVictim ()->SetSubdefence (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (20);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.4);
        }
#endif
    };
}
#endif
