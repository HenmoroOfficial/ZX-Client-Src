#ifndef __CPPGEN_GNET_SKILL3439
#define __CPPGEN_GNET_SKILL3439
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3439:public Skill
    {
      public:
        enum
        { SKILL_ID = 3439 };
          Skill3439 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3439Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (0);
                skill->SetPlus ((skill->GetPlayer ()->GetAtkrate () + skill->GetPlayer ()->GetDodge ()) * 0.01 *
                                (20 + 3 * skill->GetLevel () + 15 * skill->GetT0 ()) * (5 + 0.5 * skill->GetLevel ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3439Stub ():SkillStub (3439)
        {
            occupation = 66;
            name = L"¼À½£ÄýÔª";
            nativename = "¼À½£ÄýÔª";
            icon = "¼À½£ÄýÔª.dds";
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
            talent[0] = 3416;
            talent[1] = 3751;
            talent_size = 2;
            action[0] = "·ÙÏã¹È_¼À½£ÄýÔª";
            action[1] = "·ÙÏã¹È_¼À½£ÄýÔª";
            action[2] = "·ÙÏã¹È_¼À½£ÄýÔª";
            action[3] = "·ÙÏã¹È_¼À½£ÄýÔª";
            action[4] = "·ÙÏã¹È_¼À½£ÄýÔª";
            action[5] = "·ÙÏã¹È_¼À½£ÄýÔª";
            action[6] = "·ÙÏã¹È_¼À½£ÄýÔª";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "·ÙÏã¹È_Æï³Ë_¼À½£ÄýÔª";
            action[16] = "·ÙÏã¹È_Æï³Ë_¼À½£ÄýÔª";
            action[17] = "·ÙÏã¹È_Æï³Ë_¼À½£ÄýÔª";
            action[18] = "·ÙÏã¹È_Æï³Ë_¼À½£ÄýÔª";
            action[19] = "·ÙÏã¹È_Æï³Ë_¼À½£ÄýÔª";
            action[20] = "·ÙÏã¹È_Æï³Ë_¼À½£ÄýÔª";
            action[21] = "·ÙÏã¹È_Æï³Ë_¼À½£ÄýÔª";
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
#endif
        }
        virtual ~ Skill3439Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3433 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (17);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (10 * skill->GetLevel () + 564);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 60000 - 5000 * skill->GetT0 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 76, 78, 80, 82, 84, 86, 88, 90, 92 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (14.5);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (13);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               10 * skill->GetLevel () + 564, 20 + 3 * skill->GetLevel (), 5 + 0.5 * skill->GetLevel (), (30 - skill->GetLevel ()));

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
            skill->GetVictim ()->SetProbability (1.0 * (15 + skill->GetT1 ()));
            skill->GetVictim ()->SetTime (12100);
            skill->GetVictim ()->SetRatio (5);
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetAmount (5);
            skill->GetVictim ()->SetValue (0.2 * skill->GetPlayer ()->GetVar1 () * (1 + 0.1 * skill->GetT1 ()));
            skill->GetVictim ()->SetFiring (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (1000 * (30 - skill->GetLevel ()));
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetDecaccuracy (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (1000 * (30 - skill->GetLevel ()));
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetDecdodge (1);
            return true;
        }
#endif
    };
}
#endif
