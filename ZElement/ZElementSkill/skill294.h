#ifndef __CPPGEN_GNET_SKILL294
#define __CPPGEN_GNET_SKILL294
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill294:public Skill
    {
      public:
        enum
        { SKILL_ID = 294 };
          Skill294 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill294Stub:public SkillStub
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
                skill->SetRatio (1 + 0.05 * skill->GetT1 ());
                skill->SetCrithurt (0.1 * skill->GetT3 ());
                float hpp = (1.0 * skill->GetPlayer ()->GetHp ()) / skill->GetPlayer ()->GetMaxhp ();
                  skill->SetCrit (0.01 * skill->GetLevel () * (1.0 - hpp) + 0.01 * skill->GetT3 ());
                  skill->SetPlus (7 * skill->GetLevel () * skill->GetT0 () +
                                  (1.99 * hpp * hpp - 3.79 * hpp + 2.8) * (1 +
                                                                           0.12 * skill->GetLevel () * (0.2 * skill->GetT0 () + 1) * (1.0 -
                                                                                                                                      hpp)) *
                                  (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                   (179 + 31 * skill->GetLevel () - 1.5 * skill->GetLevel () * skill->GetLevel ()) + INT (0.05 * skill->GetLevel () +
                                                                                                                          0.55) * (0.16 *
                                                                                                                                   skill->
                                                                                                                                   GetLevel () *
                                                                                                                                   skill->
                                                                                                                                   GetLevel () +
                                                                                                                                   37 *
                                                                                                                                   skill->
                                                                                                                                   GetLevel () +
                                                                                                                                   9.2)));
                  skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill294Stub ():SkillStub (294)
        {
            occupation = 5;
            name = L"·É»¨";
            nativename = "·É»¨";
            icon = "·É»¨.dds";
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
            talent[0] = 726;
            talent[1] = 742;
            talent[2] = 749;
            talent[3] = 753;
            talent_size = 4;
            action[0] = "ºÏ»¶ÅÉ_·É»¨";
            action[1] = "ºÏ»¶ÅÉ_·É»¨";
            action[2] = "ºÏ»¶ÅÉ_·É»¨";
            action[3] = "ºÏ»¶ÅÉ_·É»¨";
            action[4] = "ºÏ»¶ÅÉ_·É»¨";
            action[5] = "ºÏ»¶ÅÉ_·É»¨";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_·É»¨";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_·É»¨";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_·É»¨";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_·É»¨";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_·É»¨";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_·É»¨";
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
#endif
        }
        virtual ~ Skill294Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 394 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0.72 * skill->GetLevel () * skill->GetLevel () - 0.55 * skill->GetLevel () + 30);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return (4100 - 100 * skill->GetLevel ()) * (100 - 20 * skill->GetT2 ()) / 100;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 55, 56, 59, 61, 63, 65, 67, 69, 71 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (7);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (0.72 * skill->GetLevel () * skill->GetLevel () - 0.55 * skill->GetLevel () + 30),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (179 + 31 * skill->GetLevel () - 1.5 * skill->GetLevel () * skill->GetLevel ()) + INT (0.05 * skill->GetLevel () +
                                                                                                                           0.55) * (0.16 *
                                                                                                                                    skill->
                                                                                                                                    GetLevel () *
                                                                                                                                    skill->
                                                                                                                                    GetLevel () +
                                                                                                                                    37 *
                                                                                                                                    skill->
                                                                                                                                    GetLevel () +
                                                                                                                                    9.2)),
                               4.1 - 0.1 * skill->GetLevel ());

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
