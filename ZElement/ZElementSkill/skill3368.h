#ifndef __CPPGEN_GNET_SKILL3368
#define __CPPGEN_GNET_SKILL3368
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3368:public Skill
    {
      public:
        enum
        { SKILL_ID = 3368 };
          Skill3368 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3368Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
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
                skill->GetPlayer ()->SetVar1 (int (skill->GetPlayer ()->GetRes2 () / 10) > 999 ? 999 : int (skill->GetPlayer ()->GetRes2 () / 10));
                  skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3368Stub ():SkillStub (3368)
        {
            occupation = 64;
            name = L"Ã÷¾õ";
            nativename = "Ã÷¾õ";
            icon = "Ã÷¾õ.dds";
            maxlevel = 20;
            maxlearn = 8;
            type = 2;
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
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 9;
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
            talent[0] = 3392;
            talent[1] = 3395;
            talent[2] = 3399;
            talent_size = 3;
            action[0] = "·ÙÏã¹È_ÁéÁ¦¾«ÑĞ";
            action[1] = "·ÙÏã¹È_ÁéÁ¦¾«ÑĞ";
            action[2] = "·ÙÏã¹È_ÁéÁ¦¾«ÑĞ";
            action[3] = "·ÙÏã¹È_ÁéÁ¦¾«ÑĞ";
            action[4] = "·ÙÏã¹È_ÁéÁ¦¾«ÑĞ";
            action[5] = "·ÙÏã¹È_ÁéÁ¦¾«ÑĞ";
            action[6] = "·ÙÏã¹È_ÁéÁ¦¾«ÑĞ";
            action[7] = "";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "·ÙÏã¹È_Æï³Ë_ÁéÁ¦¾«ÑĞ";
            action[16] = "·ÙÏã¹È_Æï³Ë_ÁéÁ¦¾«ÑĞ";
            action[17] = "·ÙÏã¹È_Æï³Ë_ÁéÁ¦¾«ÑĞ";
            action[18] = "·ÙÏã¹È_Æï³Ë_ÁéÁ¦¾«ÑĞ";
            action[19] = "·ÙÏã¹È_Æï³Ë_ÁéÁ¦¾«ÑĞ";
            action[20] = "·ÙÏã¹È_Æï³Ë_ÁéÁ¦¾«ÑĞ";
            action[21] = "·ÙÏã¹È_Æï³Ë_ÁéÁ¦¾«ÑĞ";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "";
            rangetype = 5;
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
        virtual ~ Skill3368Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3367 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 4 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (120);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 30000 - 8000 * skill->GetT0 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[8] = { 25, 27, 29, 31, 33, 35, 37, 40 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               4 * skill->GetLevel (),
                               (2 + 0.5 * skill->GetLevel ()), (1 + 0.5 * skill->GetLevel ()), (5 + skill->GetLevel ()), 10 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (2 + 0.5 * skill->GetLevel () + skill->GetT2 ()));
            skill->GetVictim ()->SetRatio (4 * skill->GetLevel () * (1 + 0.25 * skill->GetT1 ()));
            skill->GetVictim ()->SetAmount (skill->GetPlayer ()->GetVar1 () * 10 + skill->GetLevel ());
            skill->GetVictim ()->SetValue (3599);
            skill->GetVictim ()->SetAddweak (1);
            return true;
        }
#endif
    };
}
#endif
