#ifndef __CPPGEN_GNET_SKILL255
#define __CPPGEN_GNET_SKILL255
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill255:public Skill
    {
      public:
        enum
        { SKILL_ID = 255 };
          Skill255 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill255Stub:public SkillStub
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
                skill->SetCrit (0.01 * skill->GetT2 ());
                skill->
                    SetPlus ((INT (1.42 - 0.05 * skill->GetLevel ()) *
                              (299 + 33 * skill->GetLevel () - 1.5 * skill->GetLevel () * skill->GetLevel ()) + INT (0.05 * skill->GetLevel () +
                                                                                                                     0.55) * (0.95 *
                                                                                                                              skill->GetLevel () *
                                                                                                                              skill->GetLevel () +
                                                                                                                              21 *
                                                                                                                              skill->GetLevel () +
                                                                                                                              221)) * (1 +
                                                                                                                                       0.08 *
                                                                                                                                       skill->
                                                                                                                                       GetT3 ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill255Stub ():SkillStub (255)
        {
            occupation = 3;
            name = L"ÊÉÑªÕ¶";
            nativename = "ÊÉÑªÕ¶";
            icon = "ÊÉÑªÕ¶.dds";
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
            talent[0] = 621;
            talent[1] = 627;
            talent[2] = 628;
            talent[3] = 630;
            talent_size = 4;
            action[0] = "¹íÍõ×Ú_ÊÉÑªÕ¶";
            action[1] = "¹íÍõ×Ú_ÊÉÑªÕ¶";
            action[2] = "¹íÍõ×Ú_ÊÉÑªÕ¶";
            action[3] = "¹íÍõ×Ú_ÊÉÑªÕ¶";
            action[4] = "¹íÍõ×Ú_ÊÉÑªÕ¶";
            action[5] = "¹íÍõ×Ú_ÊÉÑªÕ¶";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_ÊÉÑªÕ¶";
            action[16] = "¹íÍõ×Ú_Æï³Ë_ÊÉÑªÕ¶";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ÊÉÑªÕ¶";
            action[18] = "¹íÍõ×Ú_Æï³Ë_ÊÉÑªÕ¶";
            action[19] = "¹íÍõ×Ú_Æï³Ë_ÊÉÑªÕ¶";
            action[20] = "¹íÍõ×Ú_Æï³Ë_ÊÉÑªÕ¶";
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
#endif
        }
        virtual ~ Skill255Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 463 };
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
            return (float) (1.2 * skill->GetLevel () * skill->GetLevel () + 1 * skill->GetLevel () + 413);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 20000 - 3000 * skill->GetT1 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 85, 87, 89, 91, 93, 95, 97, 99, 101 };
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
                               INT (1.2 * skill->GetLevel () * skill->GetLevel () + 1 * skill->GetLevel () + 413),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (299 + 33 * skill->GetLevel () - 1.5 * skill->GetLevel () * skill->GetLevel ()) + INT (0.05 * skill->GetLevel () +
                                                                                                                           0.55) * (0.95 *
                                                                                                                                    skill->
                                                                                                                                    GetLevel () *
                                                                                                                                    skill->
                                                                                                                                    GetLevel () +
                                                                                                                                    21 *
                                                                                                                                    skill->
                                                                                                                                    GetLevel () +
                                                                                                                                    221)),
                               15 + 25 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (15 * skill->GetT2 ()));
            skill->GetVictim ()->SetTime (3050 * skill->GetT2 ());
            skill->GetVictim ()->SetRatio (0.05 * skill->GetT2 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecaccuracy (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->
                SetValue (((0.15 +
                            0.25 * skill->GetLevel ()) * (INT (1.42 - 0.05 * skill->GetLevel ()) * (299 + 33 * skill->GetLevel () -
                                                                                                    1.5 * skill->GetLevel () * skill->GetLevel ()) +
                                                          INT (0.05 * skill->GetLevel () + 0.55) * (0.95 * skill->GetLevel () * skill->GetLevel () +
                                                                                                    21 * skill->GetLevel () + 221)) +
                           25 * skill->GetLevel () * skill->GetT0 ()) * (1 + 0.15 * skill->GetT1 ()));
            skill->GetVictim ()->SetHeal (1);
            return true;
        }
#endif
    };
}
#endif
