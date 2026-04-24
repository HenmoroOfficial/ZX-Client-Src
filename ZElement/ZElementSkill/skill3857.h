#ifndef __CPPGEN_GNET_SKILL3857
#define __CPPGEN_GNET_SKILL3857
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3857:public Skill
    {
      public:
        enum
        { SKILL_ID = 3857 };
          Skill3857 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3857Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 2000;
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
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (1);
                skill->SetRatio (1 + 0.08 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15));
                skill->SetCrithurt (0.05 * skill->GetLevel ());
                skill->SetPlus (850 + 15 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (0);
                skill->SetRatio (1 + 0.08 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15));
                skill->SetCrithurt (0.05 * skill->GetLevel ());
                skill->SetPlus (850 + 15 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (0);
                skill->SetRatio (1 + 0.08 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15));
                skill->SetCrithurt (0.05 * skill->GetLevel ());
                skill->SetPlus (850 + 15 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State5:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (0);
                skill->SetRatio (1 + 0.08 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15));
                skill->SetCrithurt (0.05 * skill->GetLevel ());
                skill->SetPlus (850 + 15 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State6:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (0);
                skill->SetRatio (1 + 0.08 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15));
                skill->SetCrithurt (0.05 * skill->GetLevel ());
                skill->SetPlus (850 + 15 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill3857Stub ():SkillStub (3857)
        {
            occupation = 180;
            name = L"天地绝<玄>";
            nativename = "天地绝<玄>";
            icon = "仙·天地绝.dds";
            maxlevel = 20;
            maxlearn = 0;
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
            skill_class = 10;
            guide = false;
            summon_id = 0;
            trigger = true;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 0;
            action[0] = "太昊_神之罚";
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
            action[11] = "太昊_神之罚";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "太昊_骑乘_神之罚";
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
            action[26] = "太昊_骑乘_神之罚";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
            doenchant = true;
            dobless = true;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = 11;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
            statestub.push_back (new State3 ());
            statestub.push_back (new State4 ());
            statestub.push_back (new State5 ());
            statestub.push_back (new State6 ());
#endif
        }
        virtual ~ Skill3857Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (170 + 42 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1300 + 50 * skill->GetLevel () + 5 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 4000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 15000;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               1300 + 50 * skill->GetLevel () + 5 * skill->GetLevel () * skill->GetLevel (),
                               170 + 42 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel (),
                               15,
                               850 + 15 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel (),
                               INT (8 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 3 * (skill->GetLevel () - 15)),
                               5 * skill->GetLevel (), skill->GetLevel (), skill->GetLevel () / 2, skill->GetLevel () / 4, 10 + skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel (), 3 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (10000 + skill->GetLevel () * 1000);
            skill->GetVictim ()->SetBuffid (zrand (5 + skill->GetLevel () / 2));
            skill->GetVictim ()->SetAmount (skill->GetPlayer ()->GetMaxhp () * 0.005 * skill->GetLevel ());
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetMaxhp () * 0.005 * skill->GetLevel ());
            skill->GetVictim ()->SetSubhp (1);
            skill->GetVictim ()->SetProbability (1.0 * (80));
            skill->GetVictim ()->SetTime (10000 + skill->GetLevel () * 1000);
            skill->GetVictim ()->SetRatio (0.01 * INT (skill->GetLevel () / 4));
            skill->GetVictim ()->SetBuffid (zrand (5 + skill->GetLevel () / 2));
            skill->GetVictim ()->SetIncfatalratio (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () == 1 ? 120 : 0));
            skill->GetVictim ()->SetTime (10000 + skill->GetLevel () * 1000);
            skill->GetVictim ()->SetRatio (0.01 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetBloodthirsty (1);
            skill->GetVictim ()->SetRatio (skill->GetPlayer ()->GetVar1 () * (norm (zrand (100) / (99 - 3 * skill->GetLevel ())) == 0 ? 0 : 3466));
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetClearcooldown (1);
            return true;
        }
#endif
    };
}
#endif
