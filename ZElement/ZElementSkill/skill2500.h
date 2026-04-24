#ifndef __CPPGEN_GNET_SKILL2500
#define __CPPGEN_GNET_SKILL2500
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2500:public Skill
    {
      public:
        enum
        { SKILL_ID = 2500 };
          Skill2500 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2500Stub:public SkillStub
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
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrit (0.01 * skill->GetJ1 () + 0.02 * skill->GetT0 ());
                skill->SetRatio (1 + 0.15 * skill->GetT0 () + 0.01 * skill->GetJ2 () +
                                 0.05 * (skill->GetT3 () >=
                                         skill->GetT4 ()? (skill->GetT3 () >=
                                                           skill->GetT5 ()? skill->GetT3 () : skill->GetT5 ()) : (skill->GetT4 () >=
                                                                                                                  skill->GetT5 ()? skill->
                                                                                                                  GetT4 () : skill->GetT5 ())));
                skill->SetPlus (800 + 50 * skill->GetLevel () + 15 * skill->GetLevel () * skill->GetLevel () + 90 * skill->GetJ0 ());
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
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrit (0.01 * skill->GetJ1 () + 0.02 * skill->GetT0 ());
                skill->SetRatio (1 + 0.15 * skill->GetT0 () + 0.01 * skill->GetJ2 () +
                                 0.05 * (skill->GetT3 () >=
                                         skill->GetT4 ()? (skill->GetT3 () >=
                                                           skill->GetT5 ()? skill->GetT3 () : skill->GetT5 ()) : (skill->GetT4 () >=
                                                                                                                  skill->GetT5 ()? skill->
                                                                                                                  GetT4 () : skill->GetT5 ())));
                skill->SetPlus (800 + 50 * skill->GetLevel () + 15 * skill->GetLevel () * skill->GetLevel () + 90 * skill->GetJ0 ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State4:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrit (0.01 * skill->GetJ1 () + 0.02 * skill->GetT0 ());
                skill->SetRatio (1 + 0.15 * skill->GetT0 () + 0.01 * skill->GetJ2 () +
                                 0.05 * (skill->GetT3 () >=
                                         skill->GetT4 ()? (skill->GetT3 () >=
                                                           skill->GetT5 ()? skill->GetT3 () : skill->GetT5 ()) : (skill->GetT4 () >=
                                                                                                                  skill->GetT5 ()? skill->
                                                                                                                  GetT4 () : skill->GetT5 ())));
                skill->SetPlus (800 + 50 * skill->GetLevel () + 15 * skill->GetLevel () * skill->GetLevel () + 90 * skill->GetJ0 ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill2500Stub ():SkillStub (2500)
        {
            occupation = 55;
            name = L"神引：梅花三弄";
            nativename = "神引：梅花三弄";
            icon = "神引：梅花三弄.dds";
            maxlevel = 6;
            maxlearn = 5;
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
            comboskill = 1;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 8;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 3;
            skillelemsfactor = 3;
            preskillnum = 1;
            talent[0] = 2617;
            talent[1] = 2605;
            talent[2] = 2606;
            talent[3] = 2947;
            talent[4] = 4112;
            talent[5] = 4530;
            talent_size = 6;
            action[0] = "天华_九霄环佩";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "天华_九霄环佩";
            action[13] = "";
            action[14] = "";
            action[15] = "天华_骑乘_九霄环佩";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "天华_骑乘_九霄环佩";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 2;
            doenchant = false;
            dobless = true;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = 12;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
            statestub.push_back (new State3 ());
            statestub.push_back (new State4 ());
#endif
        }
        virtual ~ Skill2500Stub ()
        {
        }
        int GetSkillElem (int index) const
        {
            static int array[3] = { 2919, 2920, 2921 };
            return array[index - 1];
        }
        int GetComboSkillCapacity (int level) const
        {
            static int array[5] = { 1, 2, 3, 4, 5 };
            return array[level - 1];
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 2498 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (30 + 2 * skill->GetT1 ());
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (7 * skill->GetLevel () * skill->GetLevel () + 400);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 9000 - 2000 * skill->GetT2 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[5] = { 137, 138, 139, 140, 141 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (20 + 2 * skill->GetT1 ());
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (19 + 2 * skill->GetT1 ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (15 + 2 * skill->GetT1 ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               7 * skill->GetLevel () * skill->GetLevel () + 400,
                               3 * skill->GetLevel (),
                               INT (800 + 50 * skill->GetLevel () + 15 * skill->GetLevel () * skill->GetLevel ()),
                               skill->GetT3 () > 0 || skill->GetT4 () > 0 || skill->GetT5 () > 0 ? table.Find (25003) : table.Find (25004));

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (1000 *
                                          (skill->GetT3 () >=
                                           skill->GetT4 ()? (skill->GetT3 () >=
                                                             skill->GetT5 ()? skill->GetT3 () : skill->GetT5 ()) : (skill->GetT4 () >=
                                                                                                                    skill->GetT5 ()? skill->
                                                                                                                    GetT4 () : skill->GetT5 ())) * 2);
            skill->GetVictim ()->SetRatio (0.01 *
                                           ((skill->GetT3 () >=
                                             skill->GetT4 ()? (skill->GetT3 () >=
                                                               skill->GetT5 ()? skill->GetT3 () : skill->GetT5 ()) : (skill->GetT4 () >=
                                                                                                                      skill->GetT5 ()? skill->
                                                                                                                      GetT4 () : skill->GetT5 ())) +
                                            1));
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetIncmp (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (3 * skill->GetLevel ());
        }
#endif
    };
}
#endif
