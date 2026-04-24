#ifndef __CPPGEN_GNET_SKILL4800
#define __CPPGEN_GNET_SKILL4800
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4800:public Skill
    {
      public:
        enum
        { SKILL_ID = 4800 };
          Skill4800 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4800Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 640;
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
                return 640;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (1);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetRes5 ());
                skill->GetPlayer ()->SetVar3 ((skill->GetPlayer ()->GetPuppetid () == 4 ? skill->GetPlayer ()->GetRes4 () : 0));
                skill->GetPlayer ()->SetVar4 (skill->GetPlayer ()->GetPuppetid () == 5 ? 1 : 0);
                skill->GetPlayer ()->SetVar5 ((skill->GetPlayer ()->GetPuppetid () == 3 ? skill->GetPlayer ()->GetRes5 () : 0));
                skill->GetPlayer ()->SetVar6 (skill->GetPlayer ()->GetSkilllevel (5166));
                skill->GetPlayer ()->SetVar7 (skill->GetPlayer ()->GetSkilllevel (5167));
                skill->GetPlayer ()->SetVar8 (skill->GetPlayer ()->GetSkilllevel (5168));
                skill->GetPlayer ()->SetVar9 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetCrit ((skill->GetPlayer ()->GetPuppetid () == 2 ? 0.05 : 0));
                skill->SetRatio (1 + 0.01 * (skill->GetLevel () + skill->GetT0 ()) + (skill->GetPlayer ()->GetPuppetid () == 1 ? 0.1 : 0));
                skill->SetPlus (400 + 75 * skill->GetLevel ());
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
                return 640;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (0);
                skill->SetCrit ((skill->GetPlayer ()->GetPuppetid () == 2 ? 0.05 : 0));
                skill->SetRatio (1 + 0.01 * (skill->GetLevel () + skill->GetT0 ()) + (skill->GetPlayer ()->GetPuppetid () == 1 ? 0.1 : 0));
                skill->SetPlus (400 + 75 * skill->GetLevel ());
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
                return 640;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (0);
                skill->SetCrit ((skill->GetPlayer ()->GetPuppetid () == 2 ? 0.05 : 0));
                skill->SetRatio (1 + 0.01 * (skill->GetLevel () + skill->GetT0 ()) + (skill->GetPlayer ()->GetPuppetid () == 1 ? 0.1 : 0));
                skill->SetPlus (400 + 75 * skill->GetLevel ());
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
                return 640;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (0);
                skill->SetCrit ((skill->GetPlayer ()->GetPuppetid () == 2 ? 0.05 : 0));
                skill->SetRatio (1 + 0.01 * (skill->GetLevel () + skill->GetT0 ()) + (skill->GetPlayer ()->GetPuppetid () == 1 ? 0.1 : 0));
                skill->SetPlus (400 + 75 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill4800Stub ():SkillStub (4800)
        {
            occupation = 104;
            name = L"Ä¾¼×Êõ£º¾ªÌÎ";
            nativename = "Ä¾¼×Êõ£º¾ªÌÎ";
            icon = "¿şÀÜÊõ£º¾ªÌÎ.dds";
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
            talent[0] = 5165;
            talent[1] = 5174;
            talent_size = 2;
            action[0] = "¿şÀÜ3-3";
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
            action[13] = "¿şÀÜ3-3";
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
            action[28] = "¿şÀÜ3-3";
            action[29] = "0";
            action[30] = "0";
            rangetype = 3;
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
            statestub.push_back (new State5 ());
#endif
        }
        virtual ~ Skill4800Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 4799 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 4 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetLevel () + 10 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3200;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 3000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[5] = { 90, 94, 98, 102, 104 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (5);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (18);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (15);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               skill->GetPlayer ()->GetLevel () + 10 * skill->GetLevel (), 400 + 75 * skill->GetLevel (), skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () == 1 && skill->GetPlayer ()->GetHasbuff (4267) ? 100 : 0));
            skill->GetVictim ()->
                SetValue ((skill->GetLevel () * skill->GetPlayer ()->GetVar2 () +
                           skill->GetPlayer ()->GetBufflevel (4267) * skill->GetLevel () * skill->GetPlayer ()->GetVar9 () * 0.02) *
                          (skill->GetPlayer ()->GetVar4 () == 1 ? 1.1 : 1) * (1 + 0.1 * skill->GetT1 ()));
            skill->GetVictim ()->SetDirecthurt (1);
            skill->GetVictim ()->SetProbability (1.0 * (0));
            skill->GetVictim ()->SetRatio (skill->GetPlayer ()->GetVar1 () == 0 ? 4267 : 0);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetDispel (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetVar1 () ==
                                                  1 ? skill->GetPlayer ()->GetVar3 () + skill->GetPlayer ()->GetVar7 () * 10 : 0));
            skill->GetVictim ()->SetTime (3000 + skill->GetPlayer ()->GetVar8 () * 1000);
            skill->GetVictim ()->SetSilent (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () == 1 ? skill->GetPlayer ()->GetVar5 () : 0));
            skill->GetVictim ()->SetTime (3000 + skill->GetPlayer ()->GetVar6 () * 2000);
            skill->GetVictim ()->SetRatio (0.5);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetSlow (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (40);
        }
#endif
    };
}
#endif
