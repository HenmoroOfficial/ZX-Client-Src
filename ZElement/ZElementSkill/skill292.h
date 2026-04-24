#ifndef __CPPGEN_GNET_SKILL292
#define __CPPGEN_GNET_SKILL292
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill292:public Skill
    {
      public:
        enum
        { SKILL_ID = 292 };
          Skill292 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill292Stub:public SkillStub
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill292Stub ():SkillStub (292)
        {
            occupation = 6;
            name = L"¾ö¾ø";
            nativename = "¾ö¾ø";
            icon = "¾ö¾ø.dds";
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
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 1;
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
            talent[0] = 763;
            talent[1] = 764;
            talent[2] = 766;
            talent_size = 3;
            action[0] = "ºÏ»¶ÅÉ_¾ö¾ø";
            action[1] = "ºÏ»¶ÅÉ_¾ö¾ø";
            action[2] = "ºÏ»¶ÅÉ_¾ö¾ø";
            action[3] = "ºÏ»¶ÅÉ_¾ö¾ø";
            action[4] = "ºÏ»¶ÅÉ_¾ö¾ø";
            action[5] = "ºÏ»¶ÅÉ_¾ö¾ø";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_¾ö¾ø";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_¾ö¾ø";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_¾ö¾ø";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_¾ö¾ø";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_¾ö¾ø";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_¾ö¾ø";
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
            rangetype = 5;
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
        virtual ~ Skill292Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 304 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 5 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (39 * skill->GetLevel () + 65 - 0.79 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 120000 - 9600 * skill->GetT0 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[8] = { 85, 89, 93, 97, 101, 105, 109, 113 };
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
                               INT (39 * skill->GetLevel () + 65 - 0.79 * skill->GetLevel () * skill->GetLevel ()),
                               14 + 2 * skill->GetLevel (),
                               50 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel (),
                               20 + skill->GetLevel () * skill->GetLevel () * skill->GetLevel (), 30 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime ((14000 + 2000 * skill->GetLevel ()) * (1 + 0.1 * skill->GetT1 ()) + 100);
            skill->GetVictim ()->SetRatio (20 + skill->GetLevel () * skill->GetLevel () * skill->GetLevel ());
            skill->GetVictim ()->SetAmount (30 * skill->GetLevel ());
            skill->GetVictim ()->SetValue ((50 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ()) * (1 + 0.1 * skill->GetT2 ()) *
                                           (INT (zrand (100) / (100 - 6 * skill->GetT2 ())) + 1));
            skill->GetVictim ()->SetCrazy (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120 * int (skill->GetLevel () / 8)));
            skill->GetVictim ()->SetTime (norm (skill->GetT1 ()) * (14000 + 8000 * skill->GetT1 ()) * (1 + 0.1 * skill->GetT1 ()) + 100);
            skill->GetVictim ()->SetRatio (0.2 * skill->GetT1 () * int (skill->GetLevel () / 8));
              skill->GetVictim ()->SetBuffid (6);
              skill->GetVictim ()->SetInccrithurt (1);
              return true;
        }
#endif
    };
}
#endif
