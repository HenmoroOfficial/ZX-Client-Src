#ifndef __CPPGEN_GNET_SKILL4460
#define __CPPGEN_GNET_SKILL4460
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4460:public Skill
    {
      public:
        enum
        { SKILL_ID = 4460 };
          Skill4460 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4460Stub:public SkillStub
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
                skill->SetRatio (1 + 0.03 * skill->GetT0 () + 0.01 * skill->GetLevel () +
                                 0.01 * INT (skill->GetPlayer ()->GetLevel () / (13 - skill->GetLevel ())));
                skill->SetPlus (3 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4460Stub ():SkillStub (4460)
        {
            occupation = 130;
            name = L"承·八卦神掌";
            nativename = "承·八卦神掌";
            icon = "八卦神掌.dds";
            maxlevel = 5;
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
            talent[0] = 687;
            talent_size = 1;
            action[0] = "鬼道_法宝_八卦神掌";
            action[1] = "青云门_法宝_八卦神掌";
            action[2] = "鬼王宗_法宝_八卦神掌";
            action[3] = "合欢派_法宝_八卦神掌";
            action[4] = "天音寺_法宝_八卦神掌";
            action[5] = "鬼道_法宝_八卦神掌";
            action[6] = "焚香谷_法宝_八卦神掌";
            action[7] = "兽神_法宝_五色石";
            action[8] = "狐妖_法宝_五色石";
            action[9] = "轩辕_法宝_五色石";
            action[10] = "怀光_法宝_五色石";
            action[11] = "兽神_法宝_五色石";
            action[12] = "天华_法宝_五色石";
            action[13] = "01";
            action[14] = "01";
            action[15] = "鬼道_骑乘_法宝_八卦神掌";
            action[16] = "青云门_骑乘_法宝_八卦神掌";
            action[17] = "鬼王宗_骑乘_法宝_八卦神掌";
            action[18] = "合欢派_骑乘_法宝_八卦神掌";
            action[19] = "天音寺_骑乘_法宝_八卦神掌";
            action[20] = "鬼道_骑乘_法宝_八卦神掌";
            action[21] = "焚香谷_骑乘_法宝_八卦神掌";
            action[22] = "兽神_骑乘_法宝_五色石";
            action[23] = "狐妖_骑乘_法宝_五色石";
            action[24] = "轩辕_骑乘_法宝_五色石";
            action[25] = "怀光_骑乘_法宝_五色石";
            action[26] = "兽神_骑乘_法宝_五色石";
            action[27] = "天华_骑乘_法宝_五色石";
            action[28] = "01";
            action[29] = "01";
            action[30] = "0";
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
        virtual ~ Skill4460Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05 + 10);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (13 - skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6300 - 300 * skill->GetLevel ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05 + 5);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               13 - skill->GetLevel (),
                               6.3 - 0.3 * skill->GetLevel (),
                               skill->GetLevel (), 3 * skill->GetLevel (), 13 - skill->GetLevel (), 10 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (100 * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetTime (2000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (10 * skill->GetLevel ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            return true;
        }
#endif
    };
}
#endif
