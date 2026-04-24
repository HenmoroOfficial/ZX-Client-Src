#ifndef __CPPGEN_GNET_SKILL3572
#define __CPPGEN_GNET_SKILL3572
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3572:public Skill
    {
      public:
        enum
        { SKILL_ID = 3572 };
          Skill3572 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3572Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 3000;
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
                return 5000;
            }
            void Calculate (Skill * skill) const
            {
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
                return 2000;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3572Stub ():SkillStub (3572)
        {
            occupation = 167;
            name = L"°ËÐ×Ðþ»ð<Ðþ>";
            nativename = "°ËÐ×Ðþ»ð<Ðþ>";
            icon = "ÏÉ¡¤°ËÐ×Ðþ»ð.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 2;
            eventflag = 0;
            spcost = 0;
            timetype = 3;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            clearmask = 1;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 1;
            cyclegfx = "·¨Õó°ËÐ×";
            cyclemode = 1;
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
            talent[0] = 3421;
            talent[1] = 3422;
            talent[2] = 326;
            talent[3] = 3408;
            talent[4] = 327;
            talent[5] = 3758;
            talent_size = 6;
            action[0] = "·ÙÏã¹È_Õó·¨¾«ÑÐ¡¾°ËÐ×¡¿";
            action[1] = "·ÙÏã¹È_Õó·¨¾«ÑÐ¡¾°ËÐ×¡¿";
            action[2] = "·ÙÏã¹È_Õó·¨¾«ÑÐ¡¾°ËÐ×¡¿";
            action[3] = "·ÙÏã¹È_Õó·¨¾«ÑÐ¡¾°ËÐ×¡¿";
            action[4] = "·ÙÏã¹È_Õó·¨¾«ÑÐ¡¾°ËÐ×¡¿";
            action[5] = "·ÙÏã¹È_Õó·¨¾«ÑÐ¡¾°ËÐ×¡¿";
            action[6] = "·ÙÏã¹È_Õó·¨¾«ÑÐ¡¾°ËÐ×¡¿";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "·ÙÏã¹È_Æï³Ë_Õó·¨¾«ÑÐ¡¾°ËÐ×¡¿";
            action[16] = "·ÙÏã¹È_Æï³Ë_Õó·¨¾«ÑÐ¡¾°ËÐ×¡¿";
            action[17] = "·ÙÏã¹È_Æï³Ë_Õó·¨¾«ÑÐ¡¾°ËÐ×¡¿";
            action[18] = "·ÙÏã¹È_Æï³Ë_Õó·¨¾«ÑÐ¡¾°ËÐ×¡¿";
            action[19] = "·ÙÏã¹È_Æï³Ë_Õó·¨¾«ÑÐ¡¾°ËÐ×¡¿";
            action[20] = "·ÙÏã¹È_Æï³Ë_Õó·¨¾«ÑÐ¡¾°ËÐ×¡¿";
            action[21] = "·ÙÏã¹È_Æï³Ë_Õó·¨¾«ÑÐ¡¾°ËÐ×¡¿";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
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
#endif
        }
        virtual ~ Skill3572Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (300 + 22 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3454 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 200, 500, 500, 600, 900, 2400, 3500, 5600, 8000, 9100, 21700, 36400, 51500, 77500, 79800, 179300, 290600, 437200, 872700, 1620700 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 1, 3, 5, 7, 9, 12, 15, 18, 22, 26, 30, 35, 40, 45, 50, 56, 62, 68, 74, 80 };
            return array[level - 1];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (3500 + 10 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 10000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 180000 - 15000 * skill->GetT1 () - 180000 * 0.02 * skill->GetT5 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (28);
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
                               3500 + 10 * skill->GetLevel () * skill->GetLevel (),
                               300 + 22 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel (),
                               180, (10 + skill->GetLevel ()), 5 + 2 * skill->GetLevel (), 10);

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
            skill->GetVictim ()->SetProbability (1.0 * (20 * skill->GetT3 ()));
            skill->GetVictim ()->SetBreakcasting (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (0);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount (3373);
            skill->GetVictim ()->SetValue (2);
            skill->GetVictim ()->SetCycle (1);
            skill->GetVictim ()->SetRatio (zrand (100) < skill->GetPlayer ()->GetSkilllevel (3759) * 2 ? 3449 : 218);
            skill->GetVictim ()->SetAmount (218);
            skill->GetVictim ()->SetValue (218);
            skill->GetVictim ()->SetClearcooldown (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (50);
            skill->GetVictim ()->SetRatio ((skill->GetCharging () > 4500 ? 1 : 0) * (skill->GetPlayer ()->GetCyclemembercnt () == 6 ? 1 : 0) * 100 +
                                           (skill->GetCharging () > 2500 ? 1 : 0) * (skill->GetPlayer ()->GetCyclemembercnt () >
                                                                                     3 ? 1 : 0) * 100 + (skill->GetCharging () >=
                                                                                                         0 ? 1 : 0) *
                                           (skill->GetPlayer ()->GetCyclemembercnt () >=
                                            1 ? 1 : 0) * 100 + 10 + skill->GetLevel () + skill->GetPlayer ()->GetCyclemembercnt () * skill->GetT0 () +
                                           10 * int (skill->GetCharging () > 4800));
              skill->GetVictim ()->SetBuffid (1);
              skill->GetVictim ()->SetValue (3860);
              skill->GetVictim ()->SetDelaycast (1);
              skill->GetVictim ()->SetTime (500);
              skill->GetVictim ()->SetRatio (0);
              skill->GetVictim ()->SetAmount (0);
              skill->GetVictim ()->SetValue (0);
              skill->GetVictim ()->SetEvilaura (1);
              return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (40);
        }
#endif
    };
}
#endif
