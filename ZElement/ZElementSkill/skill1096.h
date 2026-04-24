#ifndef __CPPGEN_GNET_SKILL1096
#define __CPPGEN_GNET_SKILL1096
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1096:public Skill
    {
      public:
        enum
        { SKILL_ID = 1096 };
          Skill1096 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1096Stub:public SkillStub
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
                skill->SetRatio (1);
                skill->SetCrit (0.01 * skill->GetT2 ());
                skill->SetCrithurt (0.10 * skill->GetT2 ());
                skill->
                    SetPlus ((INT (1.42 - 0.05 * skill->GetLevel ()) *
                              (0.35 * skill->GetLevel () * skill->GetLevel () + 2.4 * skill->GetLevel () + 42) + INT (0.05 * skill->GetLevel () +
                                                                                                                      0.55) * (skill->GetLevel () *
                                                                                                                               skill->GetLevel () -
                                                                                                                               7.4 *
                                                                                                                               skill->GetLevel () +
                                                                                                                               76)) * (1 +
                                                                                                                                       0.15 *
                                                                                                                                       skill->
                                                                                                                                       GetT0 ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1096Stub ():SkillStub (1096)
        {
            occupation = 25;
            name = L"»¯¹Ç¶¾É°";
            nativename = "»¯¹Ç¶¾É°";
            icon = "»¯¹Ç¶¾É°.dds";
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
            talent[0] = 1141;
            talent[1] = 1151;
            talent[2] = 1155;
            talent_size = 3;
            action[0] = "¹íµÀ_»¯¹Ç¶¾ÕÎ";
            action[1] = "¹íµÀ_»¯¹Ç¶¾ÕÎ";
            action[2] = "¹íµÀ_»¯¹Ç¶¾ÕÎ";
            action[3] = "¹íµÀ_»¯¹Ç¶¾ÕÎ";
            action[4] = "¹íµÀ_»¯¹Ç¶¾ÕÎ";
            action[5] = "¹íµÀ_»¯¹Ç¶¾ÕÎ";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íµÀ_Æï³Ë_»¯¹Ç¶¾ÕÎ";
            action[16] = "¹íµÀ_Æï³Ë_»¯¹Ç¶¾ÕÎ";
            action[17] = "¹íµÀ_Æï³Ë_»¯¹Ç¶¾ÕÎ";
            action[18] = "¹íµÀ_Æï³Ë_»¯¹Ç¶¾ÕÎ";
            action[19] = "¹íµÀ_Æï³Ë_»¯¹Ç¶¾ÕÎ";
            action[20] = "¹íµÀ_Æï³Ë_»¯¹Ç¶¾ÕÎ";
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
        virtual ~ Skill1096Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1093 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0.2 * skill->GetLevel () * skill->GetLevel () + 5 * skill->GetLevel () + 40);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6000 - 900 * skill->GetT1 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 20, 22, 24, 26, 28, 30, 32, 34, 36 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (5.8 + 0.2 * skill->GetLevel ());
        }
        float GetAngle (Skill * skill) const
        {
            return (float) (1 - 0.0111111 * (120));
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (5.8 + 0.2 * skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (3);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (0.2 * skill->GetLevel () * skill->GetLevel () + 5 * skill->GetLevel () + 40),
                               8,
                               4.8 + 0.2 * skill->GetLevel (),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (0.35 * skill->GetLevel () * skill->GetLevel () + 2.4 * skill->GetLevel () + 42) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (skill->GetLevel () * skill->GetLevel () - 7.4 * skill->GetLevel () +
                                                                              76)));

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
            return (int) (8);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.16);
        }
#endif
    };
}
#endif
