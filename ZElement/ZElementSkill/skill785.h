#ifndef __CPPGEN_GNET_SKILL785
#define __CPPGEN_GNET_SKILL785
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill785:public Skill
    {
      public:
        enum
        { SKILL_ID = 785 };
          Skill785 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill785Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 500;
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
                skill->SetRatio (1 + 0.03 * skill->GetLevel () +
                                 (0.05 + 0.05 * int (skill->GetPlayer ()->GetSkilllevel (234) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (242) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (543) / 9)) *skill->GetT3 () +
                                 0.08 * INT (skill->GetPlayer ()->GetMp () / (0.95 - 0.1 * skill->GetT2 ()) / skill->GetPlayer ()->GetMaxmp ()));
                skill->SetPlus ((750 + 22 * skill->GetLevel () + 8 * skill->GetLevel () * skill->GetLevel ()) *
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
                return 500;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.03 * skill->GetLevel () +
                                 (0.05 + 0.05 * int (skill->GetPlayer ()->GetSkilllevel (234) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (242) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (543) / 9)) *skill->GetT3 () +
                                 0.08 * INT (skill->GetPlayer ()->GetMp () / (0.95 - 0.1 * skill->GetT2 ()) / skill->GetPlayer ()->GetMaxmp ()));
                skill->SetPlus ((750 + 22 * skill->GetLevel () + 8 * skill->GetLevel () * skill->GetLevel ()) *
                                ((100 + (zrand (10 + 15 * skill->GetT0 ()) - 10) * norm (skill->GetT0 () + 0.1)) / 100.0));
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
                return 500;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.03 * skill->GetLevel () +
                                 (0.05 + 0.05 * int (skill->GetPlayer ()->GetSkilllevel (234) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (242) / 9) +
                                  0.05 * int (skill->GetPlayer ()->GetSkilllevel (543) / 9)) *skill->GetT3 () +
                                 0.08 * INT (skill->GetPlayer ()->GetMp () / (0.95 - 0.1 * skill->GetT2 ()) / skill->GetPlayer ()->GetMaxmp ()));
                skill->SetPlus ((750 + 22 * skill->GetLevel () + 8 * skill->GetLevel () * skill->GetLevel ()) *
                                ((100 + (zrand (10 + 15 * skill->GetT0 ()) - 10) * norm (skill->GetT0 () + 0.1)) / 100.0));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill785Stub ():SkillStub (785)
        {
            occupation = 20;
            name = L"太极玄天真诀";
            nativename = "太极玄天真诀";
            icon = "太极玄天真诀.dds";
            maxlevel = 5;
            maxlearn = 4;
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
            talent[2] = 612;
            talent[3] = 617;
            talent_size = 4;
            action[0] = "青云门_太极玄天真诀";
            action[1] = "青云门_太极玄天真诀";
            action[2] = "青云门_太极玄天真诀";
            action[3] = "青云门_太极玄天真诀";
            action[4] = "青云门_太极玄天真诀";
            action[5] = "青云门_太极玄天真诀";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "青云门_骑乘_太极玄天真诀";
            action[16] = "青云门_骑乘_太极玄天真诀";
            action[17] = "青云门_骑乘_太极玄天真诀";
            action[18] = "青云门_骑乘_太极玄天真诀";
            action[19] = "青云门_骑乘_太极玄天真诀";
            action[20] = "青云门_骑乘_太极玄天真诀";
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
            statestub.push_back (new State4 ());
#endif
        }
        virtual ~ Skill785Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 784 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (4 * skill->GetLevel () * skill->GetLevel () - 5 * skill->GetLevel () + 2230);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 4000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[4] = { 135, 135, 136, 138 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (23);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (22);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (15);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (4 * skill->GetLevel () * skill->GetLevel () - 5 * skill->GetLevel () + 2230),
                               40,
                               22,
                               3 * skill->GetLevel (),
                               INT (750 + 22 * skill->GetLevel () + 8 * skill->GetLevel () * skill->GetLevel ()), 5 * skill->GetLevel (), 3);

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
            skill->GetVictim ()->SetProbability (1.0 * (5 * skill->GetLevel () + 3 * skill->GetT1 ()));
            skill->GetVictim ()->SetTime (3000 + 100);
            skill->GetVictim ()->SetRatio (0.2);
            skill->GetVictim ()->SetValue (0.2);
            skill->GetVictim ()->SetRandcurse (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetBuffid (INT (skill->GetPlayer ()->GetMp () / (0.95 - 0.1 * skill->GetT2 ()) / skill->GetPlayer ()->GetMaxmp ()));
            skill->GetVictim ()->SetUniqprompt (1);
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
