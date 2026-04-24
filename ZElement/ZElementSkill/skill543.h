#ifndef __CPPGEN_GNET_SKILL543
#define __CPPGEN_GNET_SKILL543
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill543:public Skill
    {
      public:
        enum
        { SKILL_ID = 543 };
          Skill543 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill543Stub:public SkillStub
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
                return 1500;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.02 * skill->GetLevel () +
                                 (0.05 + 0.05 * int (skill->GetPlayer ()->GetSkilllevel (234) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (242) / 9)) *skill->GetT3 ());
                skill->
                    SetPlus ((INT (1.42 - 0.05 * skill->GetLevel ()) * 0.9 *
                              (0.13 * skill->GetLevel () * skill->GetLevel () + 28 * skill->GetLevel () + 1820) + INT (0.05 * skill->GetLevel () +
                                                                                                                       0.55) * 0.9 * (333 *
                                                                                                                                      skill->
                                                                                                                                      GetLevel () -
                                                                                                                                      8.5 *
                                                                                                                                      skill->
                                                                                                                                      GetLevel () *
                                                                                                                                      skill->
                                                                                                                                      GetLevel ())) *
                             ((100 + (zrand (10 + 15 * skill->GetT0 ()) - 10) * norm (skill->GetT0 () + 0.1)) / 100.0));
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
                return 1500;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.02 * skill->GetLevel () +
                                 (0.05 + 0.05 * int (skill->GetPlayer ()->GetSkilllevel (234) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (242) / 9)) *skill->GetT3 ());
                skill->
                    SetPlus ((INT (1.42 - 0.05 * skill->GetLevel ()) * 0.9 *
                              (0.13 * skill->GetLevel () * skill->GetLevel () + 28 * skill->GetLevel () + 1820) + INT (0.05 * skill->GetLevel () +
                                                                                                                       0.55) * 0.9 * (333 *
                                                                                                                                      skill->
                                                                                                                                      GetLevel () -
                                                                                                                                      8.5 *
                                                                                                                                      skill->
                                                                                                                                      GetLevel () *
                                                                                                                                      skill->
                                                                                                                                      GetLevel ())) *
                             ((100 + (zrand (10 + 15 * skill->GetT0 ()) - 10) * norm (skill->GetT0 () + 0.1)) / 100.0));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill543Stub ():SkillStub (543)
        {
            occupation = 19;
            name = L"天地不仁";
            nativename = "天地不仁";
            icon = "天地不仁.dds";
            maxlevel = 18;
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
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 2;
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
            talent[0] = 607;
            talent[1] = 611;
            talent[2] = 614;
            talent[3] = 617;
            talent_size = 4;
            action[0] = "青云门_天地不仁";
            action[1] = "青云门_天地不仁";
            action[2] = "青云门_天地不仁";
            action[3] = "青云门_天地不仁";
            action[4] = "青云门_天地不仁";
            action[5] = "青云门_天地不仁";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "青云门_骑乘_天地不仁";
            action[16] = "青云门_骑乘_天地不仁";
            action[17] = "青云门_骑乘_天地不仁";
            action[18] = "青云门_骑乘_天地不仁";
            action[19] = "青云门_骑乘_天地不仁";
            action[20] = "青云门_骑乘_天地不仁";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
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
        virtual ~ Skill543Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 540 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 5 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (skill->GetLevel () * skill->GetLevel () - 1.7 * skill->GetLevel () + 725);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 6000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 8000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 120, 122, 124, 125, 125, 125, 125, 125, 125 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (19);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (19);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (14);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (skill->GetLevel () * skill->GetLevel () - 1.7 * skill->GetLevel () + 725),
                               40,
                               18,
                               2 * skill->GetLevel (),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) * 0.9 *
                                    (0.13 * skill->GetLevel () * skill->GetLevel () + 27 * skill->GetLevel () + 1690) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * 0.9 * (312 * skill->GetLevel () - 11 -
                                                                                    7.9 * skill->GetLevel () * skill->GetLevel ())), 6, 75);

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
            skill->GetVictim ()->SetProbability (1.0 * ((6 + 5 * skill->GetT2 ()) * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetRatio (0.75);
            skill->GetVictim ()->SetDrainmagic (1);
            skill->GetVictim ()->SetProbability (1.0 * (INT (zrand (100) / (100 - 4 * skill->GetT1 ())) * 600));
            skill->GetVictim ()->SetTime (3050 * skill->GetT1 ());
            skill->GetVictim ()->SetRatio (0.1 * skill->GetT1 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetWeak (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (500 + 1000 + 500 * skill->GetT2 ());
            skill->GetVictim ()->SetInvincible (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (40);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
