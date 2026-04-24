#ifndef __CPPGEN_GNET_SKILL541
#define __CPPGEN_GNET_SKILL541
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill541:public Skill
    {
      public:
        enum
        { SKILL_ID = 541 };
          Skill541 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill541Stub:public SkillStub
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
                                 0.16 * INT (skill->GetPlayer ()->GetMp () / (0.95 - 0.1 * skill->GetT3 ()) / skill->GetPlayer ()->GetMaxmp ()) +
                                 ((skill->GetPlayer ()->GetSkilllevel (222) > 8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (232) >
                                                                                            8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (228) >
                                                                                                          8 ? 1 : 0) +
                                  (skill->GetPlayer ()->GetSkilllevel (312) > 8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (240) >
                                                                                            8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (237) >
                                                                                                          8 ? 1 : 0) +
                                  (skill->GetPlayer ()->GetSkilllevel (541) > 8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (545) >
                                                                                            8 ? 1 : 0)) * 0.05 * skill->GetT2 () *
                                 norm (skill->GetT2 ()));
                skill->SetPlus (50 * skill->GetT1 () * skill->GetLevel () +
                                INT (1.42 - 0.05 * skill->GetLevel ()) * (2060 + 164 * skill->GetLevel () -
                                                                          9.6 * skill->GetLevel () * skill->GetLevel ()) +
                                INT (0.05 * skill->GetLevel () + 0.55) * (8.9 * skill->GetLevel () * skill->GetLevel () - 46 * skill->GetLevel () +
                                                                          2580) + 0.01 * skill->GetLevel () * skill->GetPlayer ()->GetMaxmp ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill541Stub ():SkillStub (541)
        {
            occupation = 19;
            name = L"天诛剑气";
            nativename = "天诛剑气";
            icon = "天诛剑气.dds";
            maxlevel = 18;
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
            talent[0] = 579;
            talent[1] = 580;
            talent[2] = 583;
            talent[3] = 612;
            talent_size = 4;
            action[0] = "青云门_天诛剑气";
            action[1] = "青云门_天诛剑气";
            action[2] = "青云门_天诛剑气";
            action[3] = "青云门_天诛剑气";
            action[4] = "青云门_天诛剑气";
            action[5] = "青云门_天诛剑气";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "青云门_骑乘_天诛剑气";
            action[16] = "青云门_骑乘_天诛剑气";
            action[17] = "青云门_骑乘_天诛剑气";
            action[18] = "青云门_骑乘_天诛剑气";
            action[19] = "青云门_骑乘_天诛剑气";
            action[20] = "青云门_骑乘_天诛剑气";
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
            rangetype = 0;
            doenchant = false;
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
        virtual ~ Skill541Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 538 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (19);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1.3 * skill->GetLevel () * skill->GetLevel () + 9.7 * skill->GetLevel () + 389);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6000 - 1000 * skill->GetT0 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 115, 117, 119, 121, 123, 125, 125, 125, 125 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (16);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10.7 + 0.5 * skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (1.3 * skill->GetLevel () * skill->GetLevel () + 9.7 * skill->GetLevel () + 389),
                               skill->GetLevel (),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (2060 + 164 * skill->GetLevel () - 9.6 * skill->GetLevel () * skill->GetLevel ()) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (8.9 * skill->GetLevel () * skill->GetLevel () -
                                                                              46 * skill->GetLevel () + 2580)), skill->GetLevel (),
                               0.5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetBuffid (INT (skill->GetPlayer ()->GetMp () / (0.95 - 0.1 * skill->GetT3 ()) / skill->GetPlayer ()->GetMaxmp ()));
            skill->GetVictim ()->SetUniqprompt (1);
            return true;
        }
#endif
    };
}
#endif
