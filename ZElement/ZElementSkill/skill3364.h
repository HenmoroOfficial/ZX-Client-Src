#ifndef __CPPGEN_GNET_SKILL3364
#define __CPPGEN_GNET_SKILL3364
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3364:public Skill
    {
      public:
        enum
        { SKILL_ID = 3364 };
          Skill3364 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3364Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (1.03 + 0.005 * skill->GetLevel ());
                skill->SetPlus (6 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetT0 () +
                                norm (skill->GetT0 ()) * (100 + 10 * skill->GetLevel ()) * INT (skill->GetPlayer ()->GetOccupation () == 64));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3364Stub ():SkillStub (3364)
        {
            occupation = 64;
            name = L"ÐÇ»ð";
            nativename = "ÐÇ»ð";
            icon = "ÐÇ»ð.dds";
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
            talent[0] = 3373;
            talent[1] = 3376;
            talent[2] = 3751;
            talent_size = 3;
            action[0] = "·ÙÏã¹È_³õ¼¶»ð¹¥";
            action[1] = "·ÙÏã¹È_³õ¼¶»ð¹¥";
            action[2] = "·ÙÏã¹È_³õ¼¶»ð¹¥";
            action[3] = "·ÙÏã¹È_³õ¼¶»ð¹¥";
            action[4] = "·ÙÏã¹È_³õ¼¶»ð¹¥";
            action[5] = "·ÙÏã¹È_³õ¼¶»ð¹¥";
            action[6] = "·ÙÏã¹È_³õ¼¶»ð¹¥";
            action[7] = "";
            action[8] = "";
            action[9] = "0";
            action[10] = "";
            action[11] = "0";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "·ÙÏã¹È_Æï³Ë_³õ¼¶»ð¹¥";
            action[16] = "·ÙÏã¹È_Æï³Ë_³õ¼¶»ð¹¥";
            action[17] = "·ÙÏã¹È_Æï³Ë_³õ¼¶»ð¹¥";
            action[18] = "·ÙÏã¹È_Æï³Ë_³õ¼¶»ð¹¥";
            action[19] = "·ÙÏã¹È_Æï³Ë_³õ¼¶»ð¹¥";
            action[20] = "·ÙÏã¹È_Æï³Ë_³õ¼¶»ð¹¥";
            action[21] = "·ÙÏã¹È_Æï³Ë_³õ¼¶»ð¹¥";
            action[22] = "";
            action[23] = "";
            action[24] = "0";
            action[25] = "";
            action[26] = "0";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
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
        virtual ~ Skill3364Stub ()
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
            static int array[1] = { 8 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (7);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (10);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 4000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 15, 17, 19, 21, 23, 25, 27, 29, 31 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (5.3);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (4.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 10, (3 + 0.5 * skill->GetLevel ()), 6 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (5 + 5 * skill->GetT1 () + skill->GetT2 ()));
            skill->GetVictim ()->SetTime (6100);
            skill->GetVictim ()->SetRatio (4);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (4);
            skill->GetVictim ()->SetValue (0.1 * skill->GetPlayer ()->GetVar1 () * (1 + 0.1 * skill->GetT2 ()));
            skill->GetVictim ()->SetFiring (1);
            return true;
        }
#endif
    };
}
#endif
