#ifndef __CPPGEN_GNET_SKILL4774
#define __CPPGEN_GNET_SKILL4774
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4774:public Skill
    {
      public:
        enum
        { SKILL_ID = 4774 };
          Skill4774 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4774Stub:public SkillStub
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes2 ());
                skill->GetPlayer ()->SetVar2 (1);
                skill->GetPlayer ()->SetVar3 ((skill->GetPlayer ()->GetPuppetid () == 4 ? skill->GetPlayer ()->GetRes4 () : 0));
                skill->GetPlayer ()->SetVar4 (skill->GetPlayer ()->GetPuppetid () == 5 ? 1 : 0);
                skill->GetPlayer ()->SetVar5 ((skill->GetPlayer ()->GetPuppetid () == 3 ? skill->GetPlayer ()->GetRes5 () : 0));
                skill->GetPlayer ()->SetVar6 (skill->GetPlayer ()->GetSkilllevel (5166));
                skill->GetPlayer ()->SetVar7 (skill->GetPlayer ()->GetSkilllevel (5167));
                skill->GetPlayer ()->SetVar8 (skill->GetPlayer ()->GetSkilllevel (5168));
                skill->SetCrit ((skill->GetPlayer ()->GetPuppetid () == 2 ? 0.05 : 0));
                skill->SetRatio (1 + 0.01 * skill->GetLevel () + 0.01 * skill->GetT0 () + (skill->GetPlayer ()->GetPuppetid () == 1 ? 0.1 : 0));
                skill->SetPlus (80 + 20 * skill->GetLevel ());
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
                skill->GetPlayer ()->SetVar2 (0);
                skill->SetCrit ((skill->GetPlayer ()->GetPuppetid () == 2 ? 0.05 : 0));
                skill->SetRatio (1 + 0.01 * skill->GetLevel () + 0.01 * skill->GetT0 () + (skill->GetPlayer ()->GetPuppetid () == 1 ? 0.1 : 0));
                skill->SetPlus (80 + 20 * skill->GetLevel ());
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
                skill->GetPlayer ()->SetVar2 (0);
                skill->SetCrit ((skill->GetPlayer ()->GetPuppetid () == 2 ? 0.05 : 0));
                skill->SetRatio (1 + 0.01 * skill->GetLevel () + 0.01 * skill->GetT0 () + (skill->GetPlayer ()->GetPuppetid () == 1 ? 0.1 : 0));
                skill->SetPlus (80 + 20 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill4774Stub ():SkillStub (4774)
        {
            occupation = 102;
            name = L"木甲术：连环";
            nativename = "木甲术：连环";
            icon = "傀儡术：连环.dds";
            maxlevel = 7;
            maxlearn = 5;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 64;
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
            skill_class = 12;
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
            talent[0] = 5162;
            talent[1] = 5174;
            talent_size = 2;
            action[0] = "傀儡1-4";
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
            action[11] = "0";
            action[12] = "0";
            action[13] = "傀儡1-4";
            action[14] = "0";
            action[15] = "0";
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
            action[26] = "0";
            action[27] = "0";
            action[28] = "傀儡1-4";
            action[29] = "0";
            action[30] = "0";
            rangetype = 2;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = 13;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
            statestub.push_back (new State3 ());
            statestub.push_back (new State4 ());
#endif
        }
        virtual ~ Skill4774Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 4773 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (105);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 5000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[5] = { 25, 31, 37, 43, 49 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (5);
        }
        float GetAttackdistance (Skill * skill) const
        {
            return (float) (10);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (8);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (5);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 105, 80 + 20 * skill->GetLevel (), skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar2 () == 1 && skill->GetPlayer ()->GetHasbuff (4267) ? 100 : 0));
            skill->GetVictim ()->
                SetValue ((skill->GetLevel () * skill->GetPlayer ()->GetVar1 () +
                           skill->GetPlayer ()->GetBufflevel (4267) * skill->GetLevel () * skill->GetPlayer ()->GetVar1 () * 0.1) *
                          (skill->GetPlayer ()->GetVar4 () == 1 ? 1.1 : 1) * (1 + 0.1 * skill->GetT1 ()));
            skill->GetVictim ()->SetDirecthurt (1);
            skill->GetVictim ()->SetProbability (1.0 * (0));
            skill->GetVictim ()->SetRatio (skill->GetPlayer ()->GetVar2 () == 0 ? 4267 : 0);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetDispel (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetVar2 () ==
                                                  1 ? skill->GetPlayer ()->GetVar3 () + skill->GetPlayer ()->GetVar7 () * 10 : 0));
            skill->GetVictim ()->SetTime (3000 + skill->GetPlayer ()->GetVar8 () * 1000);
            skill->GetVictim ()->SetSilent (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar5 ()));
            skill->GetVictim ()->SetTime (3000 + skill->GetPlayer ()->GetVar6 () * 2000);
            skill->GetVictim ()->SetRatio (0.5);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetSlow (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (20);
        }
#endif
    };
}
#endif
