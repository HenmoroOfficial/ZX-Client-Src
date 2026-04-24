#ifndef __CPPGEN_GNET_SKILL1120
#define __CPPGEN_GNET_SKILL1120
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1120:public Skill
    {
      public:
        enum
        { SKILL_ID = 1120 };
          Skill1120 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1120Stub:public SkillStub
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
                skill->SetRatio (1 + 0.05 * skill->GetT3 ());
                skill->SetPlus (0.03 * (skill->GetPlayer ()->GetHp () + skill->GetPlayer ()->GetMp ()) + 15 * skill->GetT1 () * skill->GetLevel () +
                                INT (1.42 - 0.05 * skill->GetLevel ()) * (3.2 * skill->GetLevel () * skill->GetLevel () + 4.4 * skill->GetLevel () +
                                                                          633) + INT (0.05 * skill->GetLevel () +
                                                                                      0.55) * (13 * skill->GetLevel () * skill->GetLevel () -
                                                                                               240 * skill->GetLevel () + 2070));
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
                skill->SetRatio (1 + 0.05 * skill->GetT3 ());
                skill->SetPlus (0.03 * (skill->GetPlayer ()->GetHp () + skill->GetPlayer ()->GetMp ()) + 15 * skill->GetT1 () * skill->GetLevel () +
                                INT (1.42 - 0.05 * skill->GetLevel ()) * (3.2 * skill->GetLevel () * skill->GetLevel () + 4.4 * skill->GetLevel () +
                                                                          633) + INT (0.05 * skill->GetLevel () +
                                                                                      0.55) * (13 * skill->GetLevel () * skill->GetLevel () -
                                                                                               240 * skill->GetLevel () + 2070));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill1120Stub ():SkillStub (1120)
        {
            occupation = 27;
            name = L"Ëé»êÌìÏ®";
            nativename = "Ëé»êÌìÏ®";
            icon = "Ëé»êÌìÏ®.dds";
            maxlevel = 20;
            maxlearn = 10;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 2;
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
            talent[0] = 1163;
            talent[1] = 1166;
            talent[2] = 1167;
            talent[3] = 1172;
            talent_size = 4;
            action[0] = "Ëé»êÌìÏ®";
            action[1] = "Ëé»êÌìÏ®";
            action[2] = "Ëé»êÌìÏ®";
            action[3] = "Ëé»êÌìÏ®";
            action[4] = "Ëé»êÌìÏ®";
            action[5] = "Ëé»êÌìÏ®";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "Ëé»êÌìÏ®";
            action[16] = "Ëé»êÌìÏ®";
            action[17] = "Ëé»êÌìÏ®";
            action[18] = "Ëé»êÌìÏ®";
            action[19] = "Ëé»êÌìÏ®";
            action[20] = "Ëé»êÌìÏ®";
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
#endif
        }
        virtual ~ Skill1120Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1118 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (13);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (650 + 30 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 95, 97, 99, 101, 103, 105, 107, 109, 112, 116 };
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
                               INT (650 + 30 * skill->GetLevel ()),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (3.2 * skill->GetLevel () * skill->GetLevel () + 4.4 * skill->GetLevel () + 633) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (13 * skill->GetLevel () * skill->GetLevel () -
                                                                              240 * skill->GetLevel () + 2070)),
                               INT (100 + 14.1 * skill->GetLevel () - 0.34 * skill->GetLevel () * skill->GetLevel ()), 2);

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
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (100 + 14.1 * skill->GetLevel () - 0.34 * skill->GetLevel () * skill->GetLevel () +
                                                  3 * skill->GetT2 () * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (2000 + 100 + 2000 * skill->GetT0 ());
            skill->GetVictim ()->SetDizzy (1);
            return true;
        }
#endif
    };
}
#endif
