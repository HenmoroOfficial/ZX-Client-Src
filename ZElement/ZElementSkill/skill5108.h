#ifndef __CPPGEN_GNET_SKILL5108
#define __CPPGEN_GNET_SKILL5108
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill5108:public Skill
    {
      public:
        enum
        { SKILL_ID = 5108 };
          Skill5108 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill5108Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 400;
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
                skill->GetPlayer ()->SetVar1 (1);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetRes1 ());
                skill->GetPlayer ()->SetVar3 ((skill->GetPlayer ()->GetPuppetid () == 4 ? skill->GetPlayer ()->GetRes4 () : 0));
                skill->GetPlayer ()->SetVar4 (skill->GetPlayer ()->GetPuppetid () == 5 ? 1 : 0);
                skill->GetPlayer ()->SetVar5 ((skill->GetPlayer ()->GetPuppetid () == 3 ? skill->GetPlayer ()->GetRes5 () : 0));
                skill->GetPlayer ()->SetVar6 (skill->GetPlayer ()->GetSkilllevel (5166));
                skill->GetPlayer ()->SetVar7 (skill->GetPlayer ()->GetSkilllevel (5167));
                skill->GetPlayer ()->SetVar8 (skill->GetPlayer ()->GetSkilllevel (5168));
                skill->SetCrit ((skill->GetPlayer ()->GetPuppetid () == 2 ? 0.05 : 0));
                skill->SetRatio (1 + 0.02 * 6 + (skill->GetPlayer ()->GetPuppetid () == 1 ? 0.1 : 0));
                skill->SetPlus (2500 + 320 * 6);
                skill->GetPlayer ()->SetPerform (1);
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
                skill->SetCrit ((skill->GetPlayer ()->GetPuppetid () == 2 ? 0.05 : 0));
                skill->SetRatio (1 + 0.02 * 6 + (skill->GetPlayer ()->GetPuppetid () == 1 ? 0.1 : 0));
                skill->SetPlus (2500 + 320 * 6);
                skill->GetPlayer ()->SetPerform (0);
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
                skill->SetCrit ((skill->GetPlayer ()->GetPuppetid () == 2 ? 0.05 : 0));
                skill->SetRatio (1 + 0.02 * 6 + (skill->GetPlayer ()->GetPuppetid () == 1 ? 0.1 : 0));
                skill->SetPlus (2500 + 320 * 6);
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State5:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (0);
                skill->SetCrit ((skill->GetPlayer ()->GetPuppetid () == 2 ? 0.05 : 0));
                skill->SetRatio (1 + 0.02 * 6 + (skill->GetPlayer ()->GetPuppetid () == 1 ? 0.1 : 0));
                skill->SetPlus (2500 + 320 * 6);
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill5108Stub ():SkillStub (5108)
        {
            occupation = 128;
            name = L"Ä¾¼×Êõ£ºÉ½ºé";
            nativename = "Ä¾¼×Êõ£ºÉ½ºé";
            icon = "¿şÀÜÊõ£ºÉ½ºé.dds";
            maxlevel = 1;
            maxlearn = 1;
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
            talent[0] = 5174;
            talent_size = 1;
            action[0] = "¿şÀÜ5-2";
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
            action[13] = "¿şÀÜ5-2";
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
            action[28] = "0";
            action[29] = "0";
            action[30] = "0";
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
            statestub.push_back (new State3 ());
            statestub.push_back (new State4 ());
            statestub.push_back (new State5 ());
#endif
        }
        virtual ~ Skill5108Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (500 + 20 * 6);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 3000;
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
            return (float) (10);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (8);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 500 + 20 * 6, 2500 + 320 * 6, 2 * 6);

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
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetVar2 () * (skill->GetPlayer ()->GetBufflevel (4267) * 0.5 * 6 + 1) *
                                           (skill->GetPlayer ()->GetVar4 () == 1 ? 2 : 1) * (1 + 2));
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
