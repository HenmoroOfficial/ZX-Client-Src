#ifndef __CPPGEN_GNET_SKILL240
#define __CPPGEN_GNET_SKILL240
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill240:public Skill
    {
      public:
        enum
        { SKILL_ID = 240 };
          Skill240 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill240Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1600;
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrithurt (0.3 * skill->GetT2 () * INT (skill->GetLevel () / 9));
                skill->SetCrit (0.01 * skill->GetT2 () * INT (skill->GetLevel () / 9) + 0.01 * skill->GetT2 ());
                skill->SetRatio (1 + 0.05 * skill->GetT2 () +
                                 ((skill->GetPlayer ()->GetSkilllevel (222) > 8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (237) >
                                                                                            8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (232) >
                                                                                                          8 ? 1 : 0) +
                                  (skill->GetPlayer ()->GetSkilllevel (228) > 8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (312) >
                                                                                            8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (240) >
                                                                                                          8 ? 1 : 0) +
                                  (skill->GetPlayer ()->GetSkilllevel (541) > 8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (545) >
                                                                                            8 ? 1 : 0)) * 0.05 * skill->GetT2 () *
                                 norm (skill->GetT2 ()));
                skill->SetPlus (30 * skill->GetLevel () * skill->GetT0 () +
                                INT (1.42 - 0.05 * skill->GetLevel ()) * (1520 - 6.2 * skill->GetLevel () * skill->GetLevel () +
                                                                          126 * skill->GetLevel ()) + INT (0.05 * skill->GetLevel () +
                                                                                                           0.55) * (330 * skill->GetLevel () - 360 -
                                                                                                                    5.2 * skill->GetLevel () *
                                                                                                                    skill->GetLevel ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill240Stub ():SkillStub (240)
        {
            occupation = 9;
            name = L"天外飞仙";
            nativename = "天外飞仙";
            icon = "天外飞仙.dds";
            maxlevel = 20;
            maxlearn = 9;
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
            talent[0] = 577;
            talent[1] = 579;
            talent[2] = 583;
            talent_size = 3;
            action[0] = "青云门_天外飞仙";
            action[1] = "青云门_天外飞仙";
            action[2] = "青云门_天外飞仙";
            action[3] = "青云门_天外飞仙";
            action[4] = "青云门_天外飞仙";
            action[5] = "青云门_天外飞仙";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "青云门_骑乘_天外飞仙";
            action[16] = "青云门_骑乘_天外飞仙";
            action[17] = "青云门_骑乘_天外飞仙";
            action[18] = "青云门_骑乘_天外飞仙";
            action[19] = "青云门_骑乘_天外飞仙";
            action[20] = "青云门_骑乘_天外飞仙";
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
            rangetype = 0;
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
        virtual ~ Skill240Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 237 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 6 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1.4 * skill->GetLevel () * skill->GetLevel () + 25 * skill->GetLevel () + 616);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 16000 - 1000 * skill->GetT1 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 100, 102, 104, 106, 108, 110, 112, 114, 116 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (18);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (1.4 * skill->GetLevel () * skill->GetLevel () + 25 * skill->GetLevel () + 616),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (1520 - 6.2 * skill->GetLevel () * skill->GetLevel () + 126 * skill->GetLevel ()) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (330 * skill->GetLevel () - 360 -
                                                                              5.2 * skill->GetLevel () * skill->GetLevel ())),
                               37 + 3 * skill->GetLevel (), 3.5 + skill->GetLevel () * 0.3, 16 - skill->GetT1 ());

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
            skill->GetVictim ()->SetProbability (1.0 * (45 + 16 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (3500 + skill->GetLevel () * 300);
            skill->GetVictim ()->SetRatio (0.37 + 0.03 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetSlow (1);
            return true;
        }
#endif
    };
}
#endif
