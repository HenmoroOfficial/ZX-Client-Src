#ifndef __CPPGEN_GNET_SKILL4939
#define __CPPGEN_GNET_SKILL4939
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4939:public Skill
    {
      public:
        enum
        { SKILL_ID = 4939 };
          Skill4939 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4939Stub:public SkillStub
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
                skill->SetRatio (1 + 0.01 * skill->GetLevel () + 0.01 * INT (skill->GetPlayer ()->GetLevel () / (12 - skill->GetLevel ())));
                skill->SetPlus (10 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4939Stub ():SkillStub (4939)
        {
            occupation = 130;
            name = L"承·五灵归一";
            nativename = "承·五灵归一";
            icon = "五灵归一.dds";
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
            action[0] = "1";
            action[1] = "1";
            action[2] = "01";
            action[3] = "01";
            action[4] = "01";
            action[5] = "01";
            action[6] = "01";
            action[7] = "兽神_法宝_五色石";
            action[8] = "狐妖_法宝_五色石";
            action[9] = "轩辕_法宝_五色石";
            action[10] = "怀光_法宝_五色石";
            action[11] = "兽神_法宝_五色石";
            action[12] = "天华_法宝_五色石";
            action[13] = "灵夙_法宝_囚火灯";
            action[14] = "英招_法宝_囚火灯";
            action[15] = "01";
            action[16] = "01";
            action[17] = "01";
            action[18] = "01";
            action[19] = "01";
            action[20] = "01";
            action[21] = "01";
            action[22] = "兽神_骑乘_法宝_五色石";
            action[23] = "狐妖_骑乘_法宝_五色石";
            action[24] = "轩辕_骑乘_法宝_五色石";
            action[25] = "怀光_骑乘_法宝_五色石";
            action[26] = "兽神_骑乘_法宝_五色石";
            action[27] = "天华_骑乘_法宝_五色石";
            action[28] = "灵夙_骑乘_法宝_囚火灯";
            action[29] = "英招_骑乘_法宝_囚火灯";
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
        virtual ~ Skill4939Stub ()
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
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05 + 10);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (32 - 2 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6300 - 300 * skill->GetLevel ();
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
                               32 - 2 * skill->GetLevel (),
                               6.3 - 0.3 * skill->GetLevel (),
                               skill->GetLevel (),
                               10 * skill->GetLevel (),
                               13 - skill->GetLevel (),
                               30 + 4 * skill->GetLevel (),
                               25 * skill->GetLevel (), 5 + 2 * skill->GetLevel (), 5 + 5 * skill->GetLevel (), 2 + skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (30 + 4 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (1000 * (5 + 2 * skill->GetLevel ()));
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (25 * skill->GetLevel ());
            skill->GetVictim ()->SetSubdefence (1);
            skill->GetVictim ()->SetProbability (1.0 * (zrand (100) < 5 + 5 * skill->GetLevel ()? 650 : 0));
            skill->GetVictim ()->SetTime (1000 * (2 + skill->GetLevel ()));
            skill->GetVictim ()->SetWrap (1);
            return true;
        }
#endif
    };
}
#endif
