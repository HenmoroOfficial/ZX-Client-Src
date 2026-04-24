#ifndef __CPPGEN_GNET_SKILL232
#define __CPPGEN_GNET_SKILL232
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill232:public Skill
    {
      public:
        enum
        { SKILL_ID = 232 };
          Skill232 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill232Stub:public SkillStub
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
                skill->SetCrit (0.01 * skill->GetT2 () * INT (skill->GetLevel () / 9) + 0.01 * skill->GetT2 ());
                skill->SetCrithurt (0.3 * skill->GetT2 () * INT (skill->GetLevel () / 9));
                skill->SetRatio (1 +
                                 ((skill->GetPlayer ()->GetSkilllevel (222) > 8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (237) >
                                                                                            8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (232) >
                                                                                                          8 ? 1 : 0) +
                                  (skill->GetPlayer ()->GetSkilllevel (228) > 8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (312) >
                                                                                            8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (240) >
                                                                                                          8 ? 1 : 0) +
                                  (skill->GetPlayer ()->GetSkilllevel (541) > 8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (545) >
                                                                                            8 ? 1 : 0)) * 0.05 * skill->GetT2 () *
                                 norm (skill->GetT2 ()) + 0.05 * skill->GetT2 ());
                skill->SetCrit (0.03 * skill->GetT2 ());
                skill->SetPlus (INT (1.42 - 0.05 * skill->GetLevel ()) * (137 + 14 * skill->GetLevel ()) +
                                INT (0.05 * skill->GetLevel () + 0.55) * (0.66 * skill->GetLevel () * skill->GetLevel () + 23 * skill->GetLevel () +
                                                                          11));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill232Stub ():SkillStub (232)
        {
            occupation = 7;
            name = L"归元剑气";
            nativename = "归元剑气";
            icon = "归元剑气.dds";
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
            talent[0] = 570;
            talent[1] = 579;
            talent[2] = 583;
            talent[3] = 604;
            talent_size = 4;
            action[0] = "青云门_归元剑气";
            action[1] = "青云门_归元剑气";
            action[2] = "青云门_归元剑气";
            action[3] = "青云门_归元剑气";
            action[4] = "青云门_归元剑气";
            action[5] = "青云门_归元剑气";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "青云门_骑乘_归元剑气";
            action[16] = "青云门_骑乘_归元剑气";
            action[17] = "青云门_骑乘_归元剑气";
            action[18] = "青云门_骑乘_归元剑气";
            action[19] = "青云门_骑乘_归元剑气";
            action[20] = "青云门_骑乘_归元剑气";
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
        virtual ~ Skill232Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 222 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 6 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (19);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (39 + 2 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6000 - 1000 * skill->GetT1 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 40, 42, 44, 46, 48, 50, 52, 54, 56 };
            return array[level - 1];
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
                               INT (39 + 2 * skill->GetLevel ()),
                               2,
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) * (137 + 14 * skill->GetLevel ()) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (0.66 * skill->GetLevel () * skill->GetLevel () +
                                                                              23 * skill->GetLevel () + 11)), 3, 6 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (15 * skill->GetT3 ()));
            skill->GetVictim ()->SetValue (9);
            skill->GetVictim ()->SetRepel (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (3000 + 100 + 2000 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue ((6 * skill->GetLevel () + 0.5 * skill->GetPlayer ()->GetLevel () * skill->GetT0 ()) *
                                           (1 + INT (zrand (100) / (100 - 6 * skill->GetT3 ()))));
            skill->GetVictim ()->SetAddattack (1);
            return true;
        }
#endif
    };
}
#endif
