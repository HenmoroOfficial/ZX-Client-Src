#ifndef __CPPGEN_GNET_SKILL5053
#define __CPPGEN_GNET_SKILL5053
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill5053:public Skill
    {
      public:
        enum
        { SKILL_ID = 5053 };
          Skill5053 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill5053Stub:public SkillStub
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
                skill->SetCrithurt (0.3 * skill->GetT2 () * INT (10 / 9));
                skill->SetCrit (0.01 * 2 * INT (10 / 9) + 0.01 * 2);
                skill->SetRatio (1 + 0.05 * 2 + 0.16 * INT (skill->GetPlayer ()->GetMp () / (0.95 - 0.1 * 2) / skill->GetPlayer ()->GetMaxmp ()) +
                                 ((9 > 8 ? 1 : 0) + (9 > 8 ? 1 : 0) + (9 > 8 ? 1 : 0) + (9 > 8 ? 1 : 0) + (9 > 8 ? 1 : 0) + (9 > 8 ? 1 : 0) + (9 >
                                                                                                                                               8 ? 1 :
                                                                                                                                               0) +
                                  (9 > 8 ? 1 : 0)) * 0.05 * 2 * norm (2));
                skill->SetPlus (50 * 2 * 10 + INT (1.42 - 0.05 * 10) * (2060 + 164 * 10 - 9.6 * 10 * 10) +
                                INT (0.05 * 10 + 0.55) * (8.9 * 10 * 10 - 46 * 10 + 2580) + 0.01 * 10 * skill->GetPlayer ()->GetMaxmp ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill5053Stub ():SkillStub (5053)
        {
            occupation = 128;
            name = L"天诛剑气";
            nativename = "天诛剑气";
            icon = "天诛剑气.dds";
            maxlevel = 1;
            maxlearn = 1;
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
            action[0] = "青云门_天诛剑气";
            action[1] = "青云门_天诛剑气";
            action[2] = "青云门_天诛剑气";
            action[3] = "青云门_天诛剑气";
            action[4] = "青云门_天诛剑气";
            action[5] = "青云门_天诛剑气";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "0";
            action[14] = "0";
            action[15] = "青云门_骑乘_天诛剑气";
            action[16] = "青云门_骑乘_天诛剑气";
            action[17] = "青云门_骑乘_天诛剑气";
            action[18] = "青云门_骑乘_天诛剑气";
            action[19] = "青云门_骑乘_天诛剑气";
            action[20] = "青云门_骑乘_天诛剑气";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "0";
            action[29] = "0";
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
        virtual ~ Skill5053Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetHpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetInkcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (19);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1.3 * 10 * 10 + 9.7 * 10 + 389);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6000 - 1000 * skill->GetT0 ();
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (16);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10.7 + 0.5 * 10);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (1.3 * 10 * 10 + 9.7 * 10 + 389),
                               10,
                               INT (INT (1.42 - 0.05 * 10) * (2060 + 164 * 10 - 9.6 * 10 * 10) +
                                    INT (0.05 * 10 + 0.55) * (8.9 * 10 * 10 - 46 * 10 + 2580)), 10, 0.5 * 10, 5000);

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
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetDebuffcnt () * 5000);
            skill->GetVictim ()->SetDirecthurt (1);
            return true;
        }
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
