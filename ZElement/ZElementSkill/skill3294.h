#ifndef __CPPGEN_GNET_SKILL3294
#define __CPPGEN_GNET_SKILL3294
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3294:public Skill
    {
      public:
        enum
        { SKILL_ID = 3294 };
          Skill3294 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3294Stub:public SkillStub
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
                skill->SetRatio (1 + 0.03 * 4 + 0.05 * 3 +
                                 0.08 * INT (skill->GetPlayer ()->GetMp () / (0.95 - 0.1 * 3) / skill->GetPlayer ()->GetMaxmp ()));
                skill->SetPlus ((750 + 22 * 4 + 8 * 4 * 4) * ((100 + (zrand (10 + 15 * 3) - 10) * norm (3 + 0.1)) / 100.0));
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
                skill->SetRatio (1 + 0.03 * 4 + 0.05 * 3 +
                                 0.08 * INT (skill->GetPlayer ()->GetMp () / (0.95 - 0.1 * 3) / skill->GetPlayer ()->GetMaxmp ()));
                skill->SetPlus ((750 + 22 * 4 + 8 * 4 * 4) * ((100 + (zrand (10 + 15 * 3) - 10) * norm (3 + 0.1)) / 100.0));
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
                skill->SetRatio (1 + 0.03 * 4 + 0.05 * 3 +
                                 0.08 * INT (skill->GetPlayer ()->GetMp () / (0.95 - 0.1 * 3) / skill->GetPlayer ()->GetMaxmp ()));
                skill->SetPlus ((750 + 22 * 4 + 8 * 4 * 4) * ((100 + (zrand (10 + 15 * 3) - 10) * norm (3 + 0.1)) / 100.0));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill3294Stub ():SkillStub (3294)
        {
            occupation = 128;
            name = L"太极玄天真诀";
            nativename = "太极玄天真诀";
            icon = "太极玄天真诀.dds";
            maxlevel = 1;
            maxlearn = 1;
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
            skill_class = -2;
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
            preskillnum = 0;
            action[0] = "青云门_太极玄天真诀";
            action[1] = "青云门_太极玄天真诀";
            action[2] = "青云门_太极玄天真诀";
            action[3] = "青云门_太极玄天真诀";
            action[4] = "青云门_太极玄天真诀";
            action[5] = "青云门_太极玄天真诀";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "青云门_骑乘_太极玄天真诀";
            action[16] = "青云门_骑乘_太极玄天真诀";
            action[17] = "青云门_骑乘_太极玄天真诀";
            action[18] = "青云门_骑乘_太极玄天真诀";
            action[19] = "青云门_骑乘_太极玄天真诀";
            action[20] = "青云门_骑乘_太极玄天真诀";
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
        virtual ~ Skill3294Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1000);
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
            return _snwprintf (buffer, length, format, 1000, 15, 22, 3 * 4, INT (750 + 22 * 4 + 8 * 4 * 4), 5 * 4, 3);

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (5 * 4 + 3 * 3));
            skill->GetVictim ()->SetTime (3000 + 100);
            skill->GetVictim ()->SetRatio (0.2);
            skill->GetVictim ()->SetValue (0.2);
            skill->GetVictim ()->SetRandcurse (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetBuffid (INT (skill->GetPlayer ()->GetMp () / (0.95 - 0.1 * 3) / skill->GetPlayer ()->GetMaxmp ()));
            skill->GetVictim ()->SetUniqprompt (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (15);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
