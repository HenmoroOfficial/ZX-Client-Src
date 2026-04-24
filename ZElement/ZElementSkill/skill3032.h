#ifndef __CPPGEN_GNET_SKILL3032
#define __CPPGEN_GNET_SKILL3032
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3032:public Skill
    {
      public:
        enum
        { SKILL_ID = 3032 };
          Skill3032 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3032Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1225;
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
                return 225;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar2 (1);
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes4 ());
                skill->SetCrit (0.01 * skill->GetT2 ());
                skill->SetCrithurt (0.1 * skill->GetT2 ());
                skill->SetRatio (1 + 0.05 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) +
                                 0.05 * skill->GetT0 ());
                skill->SetPlus (850 + 29 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
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
                return 225;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar2 (0);
                skill->GetPlayer ()->SetVar1 (0);
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes4 ());
                skill->SetCrit (0.01 * skill->GetT2 ());
                skill->SetCrithurt (0.1 * skill->GetT2 ());
                skill->SetRatio (1 + 0.05 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) +
                                 0.05 * skill->GetT0 ());
                skill->SetPlus (850 + 29 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
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
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar2 (0);
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes4 ());
                skill->SetCrit (0.01 * skill->GetT2 ());
                skill->SetCrithurt (0.1 * skill->GetT2 ());
                skill->SetRatio (1 + 0.05 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) +
                                 0.05 * skill->GetT0 ());
                skill->SetPlus (850 + 29 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
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
                return 125;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar2 (0);
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes4 ());
                skill->SetCrit (0.01 * skill->GetT2 ());
                skill->SetCrithurt (0.1 * skill->GetT2 ());
                skill->SetRatio (1 + 0.05 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) +
                                 0.05 * skill->GetT0 ());
                skill->SetPlus (850 + 29 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill3032Stub ():SkillStub (3032)
        {
            occupation = 167;
            name = L"风花雪月<玄>";
            nativename = "风花雪月<玄>";
            icon = "仙·风花雪月.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 0;
            credittype = 0;
            clearmask = 1;
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
            talent[0] = 742;
            talent[1] = 749;
            talent[2] = 753;
            talent_size = 3;
            action[0] = "合欢_风花雪月";
            action[1] = "合欢_风花雪月";
            action[2] = "合欢_风花雪月";
            action[3] = "合欢_风花雪月";
            action[4] = "合欢_风花雪月";
            action[5] = "合欢_风花雪月";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "合欢_骑乘_风花雪月";
            action[16] = "合欢_骑乘_风花雪月";
            action[17] = "合欢_骑乘_风花雪月";
            action[18] = "合欢_骑乘_风花雪月";
            action[19] = "合欢_骑乘_风花雪月";
            action[20] = "合欢_骑乘_风花雪月";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
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
            statestub.push_back (new State3 ());
            statestub.push_back (new State4 ());
            statestub.push_back (new State5 ());
#endif
        }
        virtual ~ Skill3032Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (200 + 34 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 782 };
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
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (13);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (3 * skill->GetLevel () * skill->GetLevel () + 21 * skill->GetLevel () + 950);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 16000 - 3200 * skill->GetT1 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (9);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (3 * skill->GetLevel () * skill->GetLevel () + 21 * skill->GetLevel () + 950),
                               200 + 34 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel (),
                               16,
                               INT (5 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 3 * (skill->GetLevel () - 15)),
                               INT (850 + 29 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ()),
                               120,
                               2,
                               skill->GetLevel (),
                               4, 60 + 2 * skill->GetLevel (), 12, 33, 25, 25, 5, 0.5 * skill->GetLevel (), 2 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () * (0.6 + 0.02 * skill->GetLevel ())));
            skill->GetVictim ()->SetTime (4000 + 100);
            skill->GetVictim ()->SetSilent (1);
            skill->GetVictim ()->SetProbability (1.0 * (100 * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetTime (120000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0.01 * skill->GetPlayer ()->GetMaxmp () * 60 * skill->GetLevel ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetMpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (norm (zrand (100) / 67) * 600));
            skill->GetVictim ()->SetTime (12000 + 100);
            skill->GetVictim ()->SetRatio (0.25);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetSlow (1);
            skill->GetVictim ()->SetProbability (1.0 * (900));
            skill->GetVictim ()->SetTime (skill->GetPlayer ()->GetHp () > 0.5 * skill->GetPlayer ()->GetMaxhp ()? 500 * skill->GetLevel () : 0);
            skill->GetVictim ()->SetRatio (0.02 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetWeak (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (25));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetValue (0.05 * skill->GetPlayer ()->GetMp ());
            skill->GetVictim ()->SetHeal (1);
            skill->GetVictim ()->SetProbability (1.0 * (120 * skill->GetPlayer ()->GetVar2 ()));
            skill->GetVictim ()->SetRatio (293);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (2000);
            skill->GetVictim ()->SetReduceskillcd (1);
            return true;
        }
#endif
    };
}
#endif
