#ifndef __CPPGEN_GNET_SKILL248
#define __CPPGEN_GNET_SKILL248
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill248:public Skill
    {
      public:
        enum
        { SKILL_ID = 248 };
          Skill248 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill248Stub:public SkillStub
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
                skill->SetRatio (1 + 0.05 * skill->GetT3 ());
                skill->
                    SetPlus ((INT (1.42 - 0.05 * skill->GetLevel ()) *
                              (0.014 * skill->GetLevel () * skill->GetLevel () + 2.9 * skill->GetLevel () + 34) + INT (0.05 * skill->GetLevel () +
                                                                                                                       0.55) * (0.37 *
                                                                                                                                skill->GetLevel () *
                                                                                                                                skill->GetLevel () +
                                                                                                                                44)) * (1 +
                                                                                                                                        0.08 *
                                                                                                                                        skill->
                                                                                                                                        GetT2 ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill248Stub ():SkillStub (248)
        {
            occupation = 1;
            name = L"»ğÑæµ¶";
            nativename = "»ğÑæµ¶";
            icon = "»ğÑæµ¶.dds";
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
            talent[0] = 620;
            talent[1] = 621;
            talent[2] = 630;
            talent[3] = 634;
            talent_size = 4;
            action[0] = "¹íÍõ×Ú_»ğÑæµ¶";
            action[1] = "¹íÍõ×Ú_»ğÑæµ¶";
            action[2] = "¹íÍõ×Ú_»ğÑæµ¶";
            action[3] = "¹íÍõ×Ú_»ğÑæµ¶";
            action[4] = "¹íÍõ×Ú_»ğÑæµ¶";
            action[5] = "¹íÍõ×Ú_»ğÑæµ¶";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_»ğÑæµ¶";
            action[16] = "¹íÍõ×Ú_Æï³Ë_»ğÑæµ¶";
            action[17] = "¹íÍõ×Ú_Æï³Ë_»ğÑæµ¶";
            action[18] = "¹íÍõ×Ú_Æï³Ë_»ğÑæµ¶";
            action[19] = "¹íÍõ×Ú_Æï³Ë_»ğÑæµ¶";
            action[20] = "¹íÍõ×Ú_Æï³Ë_»ğÑæµ¶";
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
        virtual ~ Skill248Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 462 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (13);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (20);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 9000 - 1000 * skill->GetT0 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 25, 27, 29, 31, 33, 35, 37, 39, 41 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (8);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (2.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               20,
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (0.014 * skill->GetLevel () * skill->GetLevel () + 2.9 * skill->GetLevel () + 34) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (0.37 * skill->GetLevel () * skill->GetLevel () + 44)),
                               144 + 48 * skill->GetLevel (), INT (35 + 3.5 * skill->GetLevel ()), 1.7 + 0.3 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (12000);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount ((144 + 48 * skill->GetLevel ()) * (1 + 0.75 * skill->GetT0 ()));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak4 (1);
            skill->GetVictim ()->SetProbability (1.0 * (35 + 3.5 * skill->GetLevel () + 4 * skill->GetT1 () * skill->GetLevel ()));
            skill->GetVictim ()->SetTime ((1700 + 300 * skill->GetLevel ()) * (1 + 0.35 * skill->GetT1 ()) + 1000);
            skill->GetVictim ()->SetDizzy (1);
            skill->GetVictim ()->SetProbability (1.0 * (2 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (100);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount (0.12 * skill->GetT3 () * skill->GetPlayer ()->GetHp ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak4 (1);
            return true;
        }
#endif
    };
}
#endif
