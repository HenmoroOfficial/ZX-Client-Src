#ifndef __CPPGEN_GNET_SKILL3259
#define __CPPGEN_GNET_SKILL3259
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3259:public Skill
    {
      public:
        enum
        { SKILL_ID = 3259 };
          Skill3259 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3259Stub:public SkillStub
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
                skill->SetRatio (1 + 0.05 * skill->GetT2 ());
                skill->
                    SetPlus ((INT (1.42 - 0.05 * skill->GetLevel ()) * (199 + 16 * skill->GetLevel ()) +
                              INT (0.05 * skill->GetLevel () + 0.55) * (0.93 * skill->GetLevel () * skill->GetLevel () + 11 * skill->GetLevel () +
                                                                        188)) * (1 + 0.08 * skill->GetT1 ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3259Stub ():SkillStub (3259)
        {
            occupation = 128;
            name = L"¹íÍõ-Ê§»êÕ¶";
            nativename = "¹íÍõ-Ê§»êÕ¶";
            icon = "Ê§»êÕ¶.dds";
            maxlevel = 1;
            maxlearn = 1;
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
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = -2;
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
            preskillnum = 0;
            talent[0] = 626;
            talent[1] = 630;
            talent[2] = 634;
            talent_size = 3;
            action[0] = "¹íÍõ×Ú_Ê§»êÕ¶";
            action[1] = "¹íÍõ×Ú_Ê§»êÕ¶";
            action[2] = "¹íÍõ×Ú_Ê§»êÕ¶";
            action[3] = "¹íÍõ×Ú_Ê§»êÕ¶";
            action[4] = "¹íÍõ×Ú_Ê§»êÕ¶";
            action[5] = "¹íÍõ×Ú_Ê§»êÕ¶";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_Ê§»êÕ¶";
            action[16] = "¹íÍõ×Ú_Æï³Ë_Ê§»êÕ¶";
            action[17] = "¹íÍõ×Ú_Æï³Ë_Ê§»êÕ¶";
            action[18] = "¹íÍõ×Ú_Æï³Ë_Ê§»êÕ¶";
            action[19] = "¹íÍõ×Ú_Æï³Ë_Ê§»êÕ¶";
            action[20] = "¹íÍõ×Ú_Æï³Ë_Ê§»êÕ¶";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "¼¼ÄÜ¹¥»÷2";
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
        virtual ~ Skill3259Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 0;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[1] = { 0 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (7);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (3.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (4000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (16000 + zrand (6000));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetMpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (700));
            skill->GetVictim ()->SetTime (9000);
            skill->GetVictim ()->SetDizzy (1);
            return true;
        }
#endif
    };
}
#endif
