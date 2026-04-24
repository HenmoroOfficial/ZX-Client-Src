#ifndef __CPPGEN_GNET_SKILL3448
#define __CPPGEN_GNET_SKILL3448
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3448:public Skill
    {
      public:
        enum
        { SKILL_ID = 3448 };
          Skill3448 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3448Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1300;
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
                return 900;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (1 + 0.03 * skill->GetT0 ());
                skill->SetPlus (247 + 14 * skill->GetLevel () + 20 * skill->GetT0 ());
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
                return 900;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (1 + 0.03 * skill->GetT0 ());
                skill->SetPlus (247 + 14 * skill->GetLevel () + 20 * skill->GetT0 ());
                skill->GetPlayer ()->SetPerform (0);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State4:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 900;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (1 + 0.03 * skill->GetT0 ());
                skill->SetPlus (247 + 14 * skill->GetLevel () + 20 * skill->GetT0 ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill3448Stub ():SkillStub (3448)
        {
            occupation = 67;
            name = L"Á¶Óü½Ù²¨";
            nativename = "Á¶Óü½Ù²¨";
            icon = "Á¶Óü½Ù²¨.dds";
            maxlevel = 20;
            maxlearn = 9;
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
            talent[0] = 3418;
            talent[1] = 3420;
            talent[2] = 3751;
            talent[3] = 3752;
            talent_size = 4;
            action[0] = "·ÙÏã¹È_ÕÙ»½Ìì»ð";
            action[1] = "·ÙÏã¹È_ÕÙ»½Ìì»ð";
            action[2] = "·ÙÏã¹È_ÕÙ»½Ìì»ð";
            action[3] = "·ÙÏã¹È_ÕÙ»½Ìì»ð";
            action[4] = "·ÙÏã¹È_ÕÙ»½Ìì»ð";
            action[5] = "·ÙÏã¹È_ÕÙ»½Ìì»ð";
            action[6] = "·ÙÏã¹È_ÕÙ»½Ìì»ð";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "·ÙÏã¹È_Æï³Ë_ÕÙ»½Ìì»ð";
            action[16] = "·ÙÏã¹È_Æï³Ë_ÕÙ»½Ìì»ð";
            action[17] = "·ÙÏã¹È_Æï³Ë_ÕÙ»½Ìì»ð";
            action[18] = "·ÙÏã¹È_Æï³Ë_ÕÙ»½Ìì»ð";
            action[19] = "·ÙÏã¹È_Æï³Ë_ÕÙ»½Ìì»ð";
            action[20] = "·ÙÏã¹È_Æï³Ë_ÕÙ»½Ìì»ð";
            action[21] = "·ÙÏã¹È_Æï³Ë_ÕÙ»½Ìì»ð";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "¼¼ÄÜ¹¥»÷3";
            rangetype = 3;
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
            statestub.push_back (new State4 ());
#endif
        }
        virtual ~ Skill3448Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3442 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (18);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1150 + 20 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 4000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 12000 - 12000 * 0.03 * skill->GetT3 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 106, 108, 110, 112, 114, 116, 118, 120, 122 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (12 + 0.5 * skill->GetLevel ());
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (15.7);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (14);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               1150 + 20 * skill->GetLevel (),
                               20 + 2 * skill->GetLevel (),
                               (12 + 0.5 * skill->GetLevel ()), (247 + 14 * skill->GetLevel ()), (6 + 2 * skill->GetLevel ()));

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
            skill->GetVictim ()->SetProbability (1.0 * (6 + 2 * skill->GetLevel () + skill->GetT2 ()));
            skill->GetVictim ()->SetTime (12100 + 2000 * skill->GetT1 ());
            skill->GetVictim ()->SetRatio (6);
            skill->GetVictim ()->SetBuffid (4);
            skill->GetVictim ()->SetAmount (8);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetVar1 () * 0.25 * (1 + 0.5 * skill->GetT1 ()) * (1 + 0.1 * skill->GetT2 ()));
            skill->GetVictim ()->SetFiring (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (20 + 2 * skill->GetLevel ());
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
