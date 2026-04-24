#ifndef __CPPGEN_GNET_SKILL246
#define __CPPGEN_GNET_SKILL246
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill246:public Skill
    {
      public:
        enum
        { SKILL_ID = 246 };
          Skill246 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill246Stub:public SkillStub
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
      Skill246Stub ():SkillStub (246)
        {
            occupation = 2;
            name = L"Ê§»êÕ¶";
            nativename = "Ê§»êÕ¶";
            icon = "Ê§»êÕ¶.dds";
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
        virtual ~ Skill246Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 254 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 6 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0.27 * skill->GetLevel () * skill->GetLevel () + 5.1 * skill->GetLevel () + 142);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 8000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 70, 72, 74, 76, 78, 80, 82, 84, 86 };
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
                               INT (0.27 * skill->GetLevel () * skill->GetLevel () + 5.1 * skill->GetLevel () + 142),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) * (199 + 16 * skill->GetLevel ()) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (0.93 * skill->GetLevel () * skill->GetLevel () +
                                                                              11 * skill->GetLevel () + 188)), 48 + 2 * skill->GetLevel (),
                               100 + 100 * skill->GetLevel (), INT (40 + 3.3 * skill->GetLevel ()));

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
            skill->GetVictim ()->SetProbability (1.0 * ((48 + 2 * skill->GetLevel ()) * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetTime (4000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (100 + 100 * skill->GetLevel ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetMpleak (1);
            skill->GetVictim ()->SetTime (6000 + 100 + 3000 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (40 + 3.3 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetT0 ());
            skill->GetVictim ()->SetSubantidizzy (1);
            skill->GetVictim ()->SetProbability (1.0 * (INT (zrand (100) / (100 - 4 * skill->GetT0 ())) * 600));
            skill->GetVictim ()->SetTime (1050 * skill->GetT0 ());
            skill->GetVictim ()->SetDizzy (1);
            skill->GetVictim ()->SetProbability (1.0 * (2 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (100);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (0.12 * skill->GetT2 () * skill->GetPlayer ()->GetHp ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak1 (1);
            return true;
        }
#endif
    };
}
#endif
