#ifndef __CPPGEN_GNET_SKILL2264
#define __CPPGEN_GNET_SKILL2264
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2264:public Skill
    {
      public:
        enum
        { SKILL_ID = 2264 };
          Skill2264 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2264Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 5400;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 100;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.04 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State3:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 100;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.04 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State4:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 100;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.04 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State5:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 100;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.04 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State6:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 100;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.04 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State7:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 100;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.04 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill2264Stub ():SkillStub (2264)
        {
            occupation = 128;
            name = L"Éñ½£ÓùÀ×Õæ¾÷";
            nativename = "Éñ½£ÓùÀ×Õæ¾÷";
            icon = "Éñ½£ÓùÀ×Õæ¾÷.dds";
            maxlevel = 8;
            maxlearn = 2;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
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
            preskillnum = 1;
            action[0] = "ÇàÔÆÃÅ_Éñ½£ÓùÀ×Õæ¾÷";
            action[1] = "ÇàÔÆÃÅ_Éñ½£ÓùÀ×Õæ¾÷";
            action[2] = "ÇàÔÆÃÅ_Éñ½£ÓùÀ×Õæ¾÷";
            action[3] = "ÇàÔÆÃÅ_Éñ½£ÓùÀ×Õæ¾÷";
            action[4] = "ÇàÔÆÃÅ_Éñ½£ÓùÀ×Õæ¾÷";
            action[5] = "ÇàÔÆÃÅ_Éñ½£ÓùÀ×Õæ¾÷";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_Éñ½£ÓùÀ×Õæ¾÷";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_Éñ½£ÓùÀ×Õæ¾÷";
            action[17] = "ÇàÔÆÃÅ_Æï³Ë_Éñ½£ÓùÀ×Õæ¾÷";
            action[18] = "ÇàÔÆÃÅ_Æï³Ë_Éñ½£ÓùÀ×Õæ¾÷";
            action[19] = "ÇàÔÆÃÅ_Æï³Ë_Éñ½£ÓùÀ×Õæ¾÷";
            action[20] = "ÇàÔÆÃÅ_Æï³Ë_Éñ½£ÓùÀ×Õæ¾÷";
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
            rangetype = 2;
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
            statestub.push_back (new State3 ());
            statestub.push_back (new State4 ());
            statestub.push_back (new State5 ());
            statestub.push_back (new State6 ());
            statestub.push_back (new State7 ());
#endif
        }
        virtual ~ Skill2264Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 783 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (30 * skill->GetLevel () * skill->GetLevel () + 2560);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 6000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 16000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[2] = { 139, 141 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (26);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (18);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (30 * skill->GetLevel () * skill->GetLevel () + 2560),
                               40,
                               25,
                               4 * skill->GetLevel (),
                               INT (800 + 50 * skill->GetLevel () + 15 * skill->GetLevel () * skill->GetLevel ()),
                               4 * skill->GetLevel (), 4 * skill->GetLevel (), 4 * skill->GetLevel (), 6, 6, 180 + 17 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (INT (zrand (100) / (100 - 5 * skill->GetLevel ())) * 600));
            skill->GetVictim ()->SetTime (15000 + 100);
            skill->GetVictim ()->SetSilent (1);
            skill->GetVictim ()->SetProbability (1.0 * (180 + 17 * skill->GetLevel () + 6 * skill->GetT0 () * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (15000 + 100 + 500 * skill->GetT0 ());
            skill->GetVictim ()->SetWrap (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetRatio (220 + INT (zrand (99 + 5 * skill->GetLevel () + 5 * skill->GetT2 ()) / 100) * 566);
            skill->GetVictim ()->SetAmount (220);
            skill->GetVictim ()->SetValue (220);
            skill->GetVictim ()->SetClearcooldown (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (40);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0);
        }
#endif
    };
}
#endif
