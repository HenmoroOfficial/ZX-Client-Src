#ifndef __CPPGEN_GNET_SKILL3371
#define __CPPGEN_GNET_SKILL3371
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3371:public Skill
    {
      public:
        enum
        { SKILL_ID = 3371 };
          Skill3371 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3371Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1500;
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
                return 500;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1.1 + 0.1 * skill->GetLevel ());
                skill->SetPlus (40 + 5 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3371Stub ():SkillStub (3371)
        {
            occupation = 64;
            name = L"¾ÛË¸";
            nativename = "¾ÛË¸";
            icon = "¾ÛË¸.dds";
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
            talent[0] = 3409;
            talent_size = 1;
            action[0] = "·ÙÏã¹È_¾ÛÆøÓù½£";
            action[1] = "·ÙÏã¹È_¾ÛÆøÓù½£";
            action[2] = "·ÙÏã¹È_¾ÛÆøÓù½£";
            action[3] = "·ÙÏã¹È_¾ÛÆøÓù½£";
            action[4] = "·ÙÏã¹È_¾ÛÆøÓù½£";
            action[5] = "·ÙÏã¹È_¾ÛÆøÓù½£";
            action[6] = "·ÙÏã¹È_¾ÛÆøÓù½£";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "·ÙÏã¹È_Æï³Ë_¾ÛÆøÓù½£";
            action[16] = "·ÙÏã¹È_Æï³Ë_¾ÛÆøÓù½£";
            action[17] = "·ÙÏã¹È_Æï³Ë_¾ÛÆøÓù½£";
            action[18] = "·ÙÏã¹È_Æï³Ë_¾ÛÆøÓù½£";
            action[19] = "·ÙÏã¹È_Æï³Ë_¾ÛÆøÓù½£";
            action[20] = "·ÙÏã¹È_Æï³Ë_¾ÛÆøÓù½£";
            action[21] = "·ÙÏã¹È_Æï³Ë_¾ÛÆøÓù½£";
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
#endif
        }
        virtual ~ Skill3371Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3370 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (14);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (120);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 25000 - 5000 * skill->GetT0 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 25, 27, 29, 31, 33, 35, 38, 41, 44 };
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
            return _snwprintf (buffer, length, format,
                               40 + 5 * skill->GetLevel (),
                               0.1 + 0.1 * skill->GetLevel (), (1 + 0.5 * skill->GetLevel ()), 20 + 4 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (20 + 4 * skill->GetLevel () + 5 * skill->GetT0 ()));
            skill->GetVictim ()->SetTime ((1 + 0.5 * skill->GetLevel () + skill->GetT0 ()) * 1000);
            skill->GetVictim ()->SetDizzy (1);
            return true;
        }
#endif
    };
}
#endif
