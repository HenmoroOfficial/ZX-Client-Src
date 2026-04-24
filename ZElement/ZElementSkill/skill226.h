#ifndef __CPPGEN_GNET_SKILL226
#define __CPPGEN_GNET_SKILL226
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill226:public Skill
    {
      public:
        enum
        { SKILL_ID = 226 };
          Skill226 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill226Stub:public SkillStub
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
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.05 * skill->GetT2 ());
                skill->SetPlus (4 * skill->GetLevel () * skill->GetT1 () +
                                (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                 (0.32 * skill->GetLevel () * skill->GetLevel () + 2.6 * skill->GetLevel () + 44) + INT (0.05 * skill->GetLevel () +
                                                                                                                         0.55) * (0.78 *
                                                                                                                                  skill->GetLevel () *
                                                                                                                                  skill->GetLevel () -
                                                                                                                                  4.6 *
                                                                                                                                  skill->GetLevel () +
                                                                                                                                  76)) * (1 +
                                                                                                                                          0.1 *
                                                                                                                                          skill->
                                                                                                                                          GetT0 ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill226Stub ():SkillStub (226)
        {
            occupation = 7;
            name = L"寒霜剑气";
            nativename = "寒霜剑气";
            icon = "寒霜剑气.dds";
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
            talent[0] = 602;
            talent[1] = 605;
            talent[2] = 617;
            talent_size = 3;
            action[0] = "青云门_霜刃决";
            action[1] = "青云门_霜刃决";
            action[2] = "青云门_霜刃决";
            action[3] = "青云门_霜刃决";
            action[4] = "青云门_霜刃决";
            action[5] = "青云门_霜刃决";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "青云门_骑乘_霜刃决";
            action[16] = "青云门_骑乘_霜刃决";
            action[17] = "青云门_骑乘_霜刃决";
            action[18] = "青云门_骑乘_霜刃决";
            action[19] = "青云门_骑乘_霜刃决";
            action[20] = "青云门_骑乘_霜刃决";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "技能攻击1";
            rangetype = 1;
            doenchant = true;
            dobless = false;
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
        virtual ~ Skill226Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 223 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (20);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 2000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 20, 22, 24, 26, 28, 30, 32, 34, 36 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (1.5);
        }
        float GetAttackdistance (Skill * skill) const
        {
            return (float) (17);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (16);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               20,
                               15,
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (0.32 * skill->GetLevel () * skill->GetLevel () + 2.6 * skill->GetLevel () + 44) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (0.78 * skill->GetLevel () * skill->GetLevel () -
                                                                              4.6 * skill->GetLevel () + 76)), 8 + 2 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (35 + 2 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (3000 + 100 + 3000 * skill->GetT1 ());
            skill->GetVictim ()->SetRatio (0.08 + 0.02 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetSlow (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (15);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.3);
        }
#endif
    };
}
#endif
