#ifndef __CPPGEN_GNET_SKILL261
#define __CPPGEN_GNET_SKILL261
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill261:public Skill
    {
      public:
        enum
        { SKILL_ID = 261 };
          Skill261 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill261Stub:public SkillStub
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
                skill->SetCrit (0.01 * skill->GetT3 ());
                skill->SetRatio (1 + 0.05 * skill->GetT1 ());
                skill->SetPlus (3 * skill->GetT3 () * skill->GetLevel () +
                                INT (1.42 - 0.05 * skill->GetLevel ()) * (0.022 * skill->GetLevel () * skill->GetLevel () + 4.4 * skill->GetLevel () +
                                                                          74) + INT (0.05 * skill->GetLevel () +
                                                                                     0.55) * (0.42 * skill->GetLevel () * skill->GetLevel () +
                                                                                              8 * skill->GetLevel () + 17));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill261Stub ():SkillStub (261)
        {
            occupation = 1;
            name = L"ÄæË®Öä";
            nativename = "ÄæË®Öä";
            icon = "ÄæË®Öä.dds";
            maxlevel = 12;
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
            talent[0] = 622;
            talent[1] = 634;
            talent[2] = 638;
            talent[3] = 656;
            talent_size = 4;
            action[0] = "¹íÍõ×Ú_ÄæË®Öä";
            action[1] = "¹íÍõ×Ú_ÄæË®Öä";
            action[2] = "¹íÍõ×Ú_ÄæË®Öä";
            action[3] = "¹íÍõ×Ú_ÄæË®Öä";
            action[4] = "¹íÍõ×Ú_ÄæË®Öä";
            action[5] = "¹íÍõ×Ú_ÄæË®Öä";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_ÄæË®Öä";
            action[16] = "¹íÍõ×Ú_Æï³Ë_ÄæË®Öä";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ÄæË®Öä";
            action[18] = "¹íÍõ×Ú_Æï³Ë_ÄæË®Öä";
            action[19] = "¹íÍõ×Ú_Æï³Ë_ÄæË®Öä";
            action[20] = "¹íÍõ×Ú_Æï³Ë_ÄæË®Öä";
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
#endif
        }
        virtual ~ Skill261Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 251 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 5 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0.2 * skill->GetLevel () * skill->GetLevel () + 0.57 * skill->GetLevel () + 46);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 30, 32, 34, 36, 38, 40, 42, 44, 46 };
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
                               INT (0.2 * skill->GetLevel () * skill->GetLevel () + 0.57 * skill->GetLevel () + 46),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (0.022 * skill->GetLevel () * skill->GetLevel () + 4.4 * skill->GetLevel () + 74) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (0.42 * skill->GetLevel () * skill->GetLevel () +
                                                                              8 * skill->GetLevel () + 17)), 35 + 3 * skill->GetLevel (),
                               9.5 + 0.5 * skill->GetLevel (), 10 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (80 + 3 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (9500 + 500 * skill->GetLevel () + 3000 * skill->GetT2 () + 100);
            skill->GetVictim ()->SetRatio (0.35 + 0.03 * skill->GetLevel () + 0.1 * skill->GetT2 ());
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetSlow (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0.90 * skill->GetT0 () * skill->GetPlayer ()->GetLevel ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (2 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (100);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (0.12 * skill->GetT1 () * skill->GetPlayer ()->GetHp ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak1 (1);
            skill->GetVictim ()->SetProbability (1.0 * (10 * skill->GetLevel ()));
            skill->GetVictim ()->SetRatio (11);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetDispel (1);
            return true;
        }
#endif
    };
}
#endif
