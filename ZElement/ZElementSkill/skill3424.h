#ifndef __CPPGEN_GNET_SKILL3424
#define __CPPGEN_GNET_SKILL3424
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3424:public Skill
    {
      public:
        enum
        { SKILL_ID = 3424 };
          Skill3424 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3424Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1200;
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
                skill->GetPlayer ()->SetVar1 (1);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (1 + 0.01 * skill->GetLevel () + 0.02 * skill->GetT0 ());
                skill->SetPlus (124 + skill->GetLevel () * skill->GetLevel () + 25 * skill->GetT0 ());
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (2);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (1 + 0.01 * skill->GetLevel () + 0.02 * skill->GetT0 ());
                skill->SetPlus (124 + skill->GetLevel () * skill->GetLevel () + 25 * skill->GetT0 ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill3424Stub ():SkillStub (3424)
        {
            occupation = 65;
            name = L"ÁÒÑæÏ®";
            nativename = "ÁÒÑæÏ®";
            icon = "ÁÒÑæÏ®.dds";
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
            talent[0] = 3378;
            talent[1] = 3380;
            talent[2] = 3751;
            talent_size = 3;
            action[0] = "·ÙÏã¹È_ÖĞ¼¶»ğ¹¥";
            action[1] = "·ÙÏã¹È_ÖĞ¼¶»ğ¹¥";
            action[2] = "·ÙÏã¹È_ÖĞ¼¶»ğ¹¥";
            action[3] = "·ÙÏã¹È_ÖĞ¼¶»ğ¹¥";
            action[4] = "·ÙÏã¹È_ÖĞ¼¶»ğ¹¥";
            action[5] = "·ÙÏã¹È_ÖĞ¼¶»ğ¹¥";
            action[6] = "·ÙÏã¹È_ÖĞ¼¶»ğ¹¥";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "·ÙÏã¹È_Æï³Ë_ÖĞ¼¶»ğ¹¥";
            action[16] = "·ÙÏã¹È_Æï³Ë_ÖĞ¼¶»ğ¹¥";
            action[17] = "·ÙÏã¹È_Æï³Ë_ÖĞ¼¶»ğ¹¥";
            action[18] = "·ÙÏã¹È_Æï³Ë_ÖĞ¼¶»ğ¹¥";
            action[19] = "·ÙÏã¹È_Æï³Ë_ÖĞ¼¶»ğ¹¥";
            action[20] = "·ÙÏã¹È_Æï³Ë_ÖĞ¼¶»ğ¹¥";
            action[21] = "·ÙÏã¹È_Æï³Ë_ÖĞ¼¶»ğ¹¥";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "";
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
        virtual ~ Skill3424Stub ()
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
            return (float) (20);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 4000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 45, 47, 49, 51, 53, 55, 57, 59, 61 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (11);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (9);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 20, (124 + skill->GetLevel () * skill->GetLevel ()), skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (10 + 45 * int (skill->GetPlayer ()->GetVar1 () == 2) * skill->GetT1 () + skill->GetT2 ()));
            skill->GetVictim ()->SetTime (8100 + 1000 * skill->GetT1 ());
            skill->GetVictim ()->SetRatio (5);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (5);
            skill->GetVictim ()->SetValue ((skill->GetPlayer ()->GetVar2 () * 0.15 + 100 * skill->GetT1 ()) * (1 + 0.1 * skill->GetT2 ()));
            skill->GetVictim ()->SetFiring (1);
            return true;
        }
#endif
    };
}
#endif
