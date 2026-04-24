#ifndef __CPPGEN_GNET_SKILL3089
#define __CPPGEN_GNET_SKILL3089
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3089:public Skill
    {
      public:
        enum
        { SKILL_ID = 3089 };
          Skill3089 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3089Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1133;
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
                return 2000;
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
                return 100;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrithurt ((skill->GetCharging () > 250 ? skill->GetCharging () * 0.0005 * (0.03 * skill->GetLevel ()) : 0));
                skill->GetPlayer ()->SetVar3 ((skill->GetCharging () > 250 ? skill->GetCharging () : 0));
                skill->SetCrit ((skill->GetCharging () > 250 ? skill->GetCharging () * 0.0005 * (0.01 * skill->GetLevel ()) : 0));
                skill->GetPlayer ()->SetVar1 (1);
                skill->SetRatio (1.02 + 0.05 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) +
                                 0.01 * skill->GetT0 () + (skill->GetCharging () >
                                                           250 ? skill->GetCharging () * 0.0005 * (0.5 + 0.05 * skill->GetLevel ()) : 0));
                skill->SetPlus (515 + 9 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
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
                return 100;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrithurt ((skill->GetCharging () > 250 ? skill->GetCharging () * 0.0005 * (0.03 * skill->GetLevel ()) : 0));
                skill->GetPlayer ()->SetVar3 ((skill->GetCharging () > 250 ? skill->GetCharging () : 0));
                skill->SetCrit ((skill->GetCharging () > 250 ? skill->GetCharging () * 0.0005 * (0.01 * skill->GetLevel ()) : 0));
                skill->GetPlayer ()->SetVar1 (2);
                skill->SetRatio (1.02 + 0.05 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) +
                                 0.01 * skill->GetT0 () + (skill->GetCharging () >
                                                           250 ? skill->GetCharging () * 0.0005 * (0.5 + 0.05 * skill->GetLevel ()) : 0));
                skill->SetPlus (515 + 9 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State5:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 667;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrithurt ((skill->GetCharging () > 250 ? skill->GetCharging () * 0.0005 * (0.03 * skill->GetLevel ()) : 0));
                skill->GetPlayer ()->SetVar3 ((skill->GetCharging () > 250 ? skill->GetCharging () : 0));
                skill->SetCrit ((skill->GetCharging () > 250 ? skill->GetCharging () * 0.0005 * (0.01 * skill->GetLevel ()) : 0) +
                                (skill->GetT1 () > 2.9 ? 0.1 : 0));
                skill->GetPlayer ()->SetVar1 (3);
                skill->SetRatio (1.02 + 0.05 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) +
                                 0.01 * skill->GetT0 () + (skill->GetCharging () >
                                                           250 ? skill->GetCharging () * 0.0005 * (0.5 + 0.05 * skill->GetLevel ()) : 0));
                skill->SetPlus (515 + 9 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill3089Stub ():SkillStub (3089)
        {
            occupation = 168;
            name = L"ÈıÔÆÁú<É·>";
            nativename = "ÈıÔÆÁú<É·>";
            icon = "Ä§¡¤ÈıÔÆÁú.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 3;
            targettype = 0;
            autoattack = 1;
            allowform = 0;
            credittype = 0;
            clearmask = 2;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 6;
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
            talent[0] = 2142;
            talent[1] = 2147;
            talent_size = 2;
            action[0] = "ºüÑı_ÈıÔÆÁú";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "ºüÑı_ÈıÔÆÁú";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ºüÑı_Æï³Ë_ÈıÔÆÁú";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "ºüÑı_Æï³Ë_ÈıÔÆÁú";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
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
            weapon_limit = 8;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
            statestub.push_back (new State3 ());
            statestub.push_back (new State4 ());
            statestub.push_back (new State5 ());
#endif
        }
        virtual ~ Skill3089Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (200 + 34 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1919 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 5 };
            return array[index];
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 400, 500, 500, 700, 1500, 3100, 3900, 6500, 9100, 10800, 20700, 26500, 39600, 52900, 84300, 119800, 229700, 389600, 525000,
 1237600 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 2, 4, 6, 8, 10, 13, 16, 19, 22, 25, 29, 33, 37, 42, 47, 53, 59, 65, 71, 77 };
            return array[level - 1];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (40 + 0.2 * skill->GetLevel () + norm (skill->GetLevel () / 19.9) * 3);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (985 + 16 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 4000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 12000 - 2000 * skill->GetT1 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (30 + 0.2 * skill->GetLevel () + norm (skill->GetLevel () / 19.9) * 3);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (20 + 0.2 * skill->GetLevel () + norm (skill->GetLevel () / 19.9) * 3);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               985 + 16 * skill->GetLevel (),
                               200 + 34 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel (),
                               12,
                               20 + 0.2 * skill->GetLevel (),
                               INT (2 + INT (5 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 3 * (skill->GetLevel () - 15))),
                               515 + 9 * skill->GetLevel (), 50 + 5 * skill->GetLevel (), skill->GetLevel (), 2 + 0.3 * skill->GetLevel (), 5);

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () == 1 ? (skill->GetT1 () > 0.9 ? 600 : 0) : 0));
            skill->GetVictim ()->SetTime (5000);
            skill->GetVictim ()->SetRatio (0.4);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetWeak (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () == 2 ? (skill->GetT1 () > 1.9 ? 600 : 0) : 0));
            skill->GetVictim ()->SetTime (5000);
            skill->GetVictim ()->SetRatio (0.4);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetSlow (1);
            skill->GetVictim ()->SetTime (2000 + 300 * skill->GetLevel () + skill->GetPlayer ()->GetVar3 () * 0.0005 * 10 * 500);
            skill->GetVictim ()->SetRatio (0.01 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecanti (1);
            return true;
        }
#endif
    };
}
#endif
