#ifndef __CPPGEN_GNET_SKILL299
#define __CPPGEN_GNET_SKILL299
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill299:public Skill
    {
      public:
        enum
        { SKILL_ID = 299 };
          Skill299 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill299Stub:public SkillStub
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
      Skill299Stub ():SkillStub (299)
        {
            occupation = 5;
            name = L"¼¤Àø";
            nativename = "¼¤Àø";
            icon = "¼¤Àø.dds";
            maxlevel = 20;
            maxlearn = 9;
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
            talent[0] = 761;
            talent[1] = 763;
            talent[2] = 764;
            talent[3] = 758;
            talent[4] = 300;
            talent[5] = 292;
            talent[6] = 766;
            talent_size = 7;
            action[0] = "ºÏ»¶ÅÉ_¼¤Àø";
            action[1] = "ºÏ»¶ÅÉ_¼¤Àø";
            action[2] = "ºÏ»¶ÅÉ_¼¤Àø";
            action[3] = "ºÏ»¶ÅÉ_¼¤Àø";
            action[4] = "ºÏ»¶ÅÉ_¼¤Àø";
            action[5] = "ºÏ»¶ÅÉ_¼¤Àø";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "0";
            action[14] = "0";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_¼¤Àø";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_¼¤Àø";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_¼¤Àø";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_¼¤Àø";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_¼¤Àø";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_¼¤Àø";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "0";
            action[29] = "0";
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
        virtual ~ Skill299Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 302 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (9 + skill->GetLevel ());
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0.56 * skill->GetLevel () * skill->GetLevel () + 19 * skill->GetLevel () + 100);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 120000 - 9600 * skill->GetT1 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 65, 69, 73, 77, 81, 85, 89, 93, 98 };
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
                               INT (0.56 * skill->GetLevel () * skill->GetLevel () + 19 * skill->GetLevel () + 100),
                               19 + 11 * skill->GetLevel (), 8 + 2 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetClearwrap (1);
            skill->GetVictim ()->SetTime ((8000 + 2000 * skill->GetLevel ()) * (1 + 0.1 * skill->GetT2 ()) + 100);
            skill->GetVictim ()->SetImmunewrap (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (15000 * (1 + 0.1 * skill->GetT2 ()) + 100);
            skill->GetVictim ()->SetRatio (0.19 + 0.11 * skill->GetLevel () + 0.03 * skill->GetLevel () * skill->GetT0 ());
            skill->GetVictim ()->SetNingjin (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime ((15000 * (1 + 0.1 * skill->GetT2 ()) + 100) * norm (skill->GetT2 ()));
            skill->GetVictim ()->SetRatio (0.05001 * skill->GetT2 ());
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetIncskillaccu (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetRatio (5216 * int (skill->GetT5 () / 7.9) * norm (skill->GetT2 ()));
              skill->GetVictim ()->SetAmount (4100 * norm (skill->GetT5 () / 7.9));
              skill->GetVictim ()->SetValue (0);
              skill->GetVictim ()->SetDispel (1);
              skill->GetVictim ()->SetTime (((14000 + 2000 * skill->GetLevel ()) * (1 + 0.1 * skill->GetT2 ()) + 100) * norm (skill->GetT5 () / 7.9));
              skill->GetVictim ()->SetRatio (20 + skill->GetLevel () * skill->GetLevel () * skill->GetLevel ());
              skill->GetVictim ()->SetAmount (30 * skill->GetLevel ());
              skill->GetVictim ()->SetValue ((50 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ()) * (1 + 0.1 * skill->GetT6 ()) *
                                             (INT (zrand (100) / (100 - 6 * skill->GetT6 ())) + 1));
              skill->GetVictim ()->SetCrazy (1);
              skill->GetVictim ()->SetProbability (1.0 * (120 * int (skill->GetT5 () / 7.9)));
              skill->GetVictim ()->SetTime (norm (skill->GetT2 ()) * (14000 + 8000 * skill->GetT2 ()) * (1 + 0.1 * skill->GetT2 ()) + 100);
              skill->GetVictim ()->SetRatio (0.2 * skill->GetT2 () * int (skill->GetLevel () / 8));
              skill->GetVictim ()->SetBuffid (6);
              skill->GetVictim ()->SetInccrithurt (1);
              skill->GetVictim ()->SetProbability (1.0 * (120 * norm (skill->GetT4 () / 7.9)));
              skill->GetVictim ()->SetTime ((14000 + 2000 * skill->GetLevel ()) * (1 + 0.1 * skill->GetT2 ()) + 100);
              skill->GetVictim ()->SetRatio ((0.04 + 0.01 * skill->GetLevel () + 0.01 * skill->GetT3 ()) *
                                             (INT (zrand (100) / (100 - 6 * skill->GetT3 ())) + 1));
              skill->GetVictim ()->SetBuffid (1);
              skill->GetVictim ()->SetIncattack3 (1);
              return true;
        }
#endif
    };
}
#endif
