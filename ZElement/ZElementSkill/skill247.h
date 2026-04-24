#ifndef __CPPGEN_GNET_SKILL247
#define __CPPGEN_GNET_SKILL247
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill247:public Skill
    {
      public:
        enum
        { SKILL_ID = 247 };
          Skill247 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill247Stub:public SkillStub
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
                skill->
                    SetPlus ((INT (1.42 - 0.05 * skill->GetLevel ()) *
                              (0.2 * skill->GetLevel () * skill->GetLevel () + 3.8 * skill->GetLevel () + 111) + INT (0.05 * skill->GetLevel () +
                                                                                                                      0.55) * (0.3 *
                                                                                                                               skill->GetLevel () *
                                                                                                                               skill->GetLevel () +
                                                                                                                               15 *
                                                                                                                               skill->GetLevel () +
                                                                                                                               19)) * (1 +
                                                                                                                                       0.07 *
                                                                                                                                       skill->
                                                                                                                                       GetT1 ()));
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
                skill->SetRatio (1.00 + 0.01 * skill->GetLevel ());
                skill->
                    SetPlus ((INT (1.42 - 0.05 * skill->GetLevel ()) *
                              (0.2 * skill->GetLevel () * skill->GetLevel () + 3.8 * skill->GetLevel () + 111) + INT (0.05 * skill->GetLevel () +
                                                                                                                      0.55) * (0.3 *
                                                                                                                               skill->GetLevel () *
                                                                                                                               skill->GetLevel () +
                                                                                                                               15 *
                                                                                                                               skill->GetLevel () +
                                                                                                                               19)) * (1 +
                                                                                                                                       0.07 *
                                                                                                                                       skill->
                                                                                                                                       GetT1 ()));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill247Stub ():SkillStub (247)
        {
            occupation = 2;
            name = L"Ñ¸À×Õ¶";
            nativename = "Ñ¸À×Õ¶";
            icon = "Ñ¸À×Õ¶.dds";
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
            talent[0] = 655;
            talent[1] = 661;
            talent_size = 2;
            action[0] = "¹íÍõ×Ú_Ñ¸À×Õ¶";
            action[1] = "¹íÍõ×Ú_Ñ¸À×Õ¶";
            action[2] = "¹íÍõ×Ú_Ñ¸À×Õ¶";
            action[3] = "¹íÍõ×Ú_Ñ¸À×Õ¶";
            action[4] = "¹íÍõ×Ú_Ñ¸À×Õ¶";
            action[5] = "¹íÍõ×Ú_Ñ¸À×Õ¶";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_Ñ¸À×Õ¶";
            action[16] = "¹íÍõ×Ú_Æï³Ë_Ñ¸À×Õ¶";
            action[17] = "¹íÍõ×Ú_Æï³Ë_Ñ¸À×Õ¶";
            action[18] = "¹íÍõ×Ú_Æï³Ë_Ñ¸À×Õ¶";
            action[19] = "¹íÍõ×Ú_Æï³Ë_Ñ¸À×Õ¶";
            action[20] = "¹íÍõ×Ú_Æï³Ë_Ñ¸À×Õ¶";
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
        virtual ~ Skill247Stub ()
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
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (14);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (30);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 3000 - 450 * skill->GetT1 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 45, 47, 49, 51, 53, 55, 57, 59, 61 };
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
                               30,
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (0.2 * skill->GetLevel () * skill->GetLevel () + 3.8 * skill->GetLevel () + 111) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (0.3 * skill->GetLevel () * skill->GetLevel () +
                                                                              15 * skill->GetLevel () + 19)), skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (20 * skill->GetT0 () * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetRatio (0.03 * skill->GetLevel () * skill->GetT0 ());
            skill->GetVictim ()->SetDrainmagic (1);
            skill->GetVictim ()->SetProbability (1.0 * (2 * skill->GetLevel () * skill->GetT0 ()));
            skill->GetVictim ()->SetTime (2050 * skill->GetT0 ());
            skill->GetVictim ()->SetBarehanded (1);
            return true;
        }
#endif
    };
}
#endif
