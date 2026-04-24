#ifndef __CPPGEN_GNET_SKILL3303
#define __CPPGEN_GNET_SKILL3303
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3303:public Skill
    {
      public:
        enum
        { SKILL_ID = 3303 };
          Skill3303 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3303Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1225;
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
                return 225;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrit (0.01 * 3);
                skill->SetCrithurt (0.1 * 3);
                skill->SetRatio (2 + 0.04 * 3 + 0.05 * 3 + 0.03 * 3);
                skill->SetPlus (850 + 29 * 3 + 4 * 3 * 3);
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
                return 225;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrit (0.01 * 3);
                skill->SetCrithurt (0.1 * 3);
                skill->SetRatio (2 + 0.04 * 3 + 0.05 * 3 + 0.03 * 3);
                skill->SetPlus (850 + 29 * 3 + 4 * 3 * 3);
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
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrit (0.01 * 3);
                skill->SetCrithurt (0.1 * 3);
                skill->SetRatio (2 + 0.04 * 3 + 0.05 * 3 + 0.03 * 3);
                skill->SetPlus (850 + 29 * 3 + 4 * 3 * 3);
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
                return 125;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrit (0.01 * 3);
                skill->SetCrithurt (0.1 * 3);
                skill->SetRatio (2 + 0.04 * 3 + 0.05 * 3 + 0.03 * 3);
                skill->SetPlus (850 + 29 * 3 + 4 * 3 * 3);
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill3303Stub ():SkillStub (3303)
        {
            occupation = 128;
            name = L"风花雪月";
            nativename = "风花雪月";
            icon = "风花雪月.dds";
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
            action[0] = "合欢_风花雪月";
            action[1] = "合欢_风花雪月";
            action[2] = "合欢_风花雪月";
            action[3] = "合欢_风花雪月";
            action[4] = "合欢_风花雪月";
            action[5] = "合欢_风花雪月";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "合欢_骑乘_风花雪月";
            action[16] = "合欢_骑乘_风花雪月";
            action[17] = "合欢_骑乘_风花雪月";
            action[18] = "合欢_骑乘_风花雪月";
            action[19] = "合欢_骑乘_风花雪月";
            action[20] = "合欢_骑乘_风花雪月";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
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
        virtual ~ Skill3303Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (13);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (500);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 16000 - 3200 * 3;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (9);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 500, 112, INT (850 + 29 * 3 + 4 * 3 * 3), 120, 2, 3, 9, 220 + 16 * 3, 12, 33, 25, 25, 5);

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * ((220 + 16 * 3 + 4 * 3 * 3) * (1 + norm (2 * 3 / (zrand (100) + 0.01)))));
            skill->GetVictim ()->SetTime (9000 + 100);
            skill->GetVictim ()->SetSilent (1);
            skill->GetVictim ()->SetProbability (1.0 * (100 * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetTime (120000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0.01 * skill->GetPlayer ()->GetMaxmp () * 60 * 3);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetMpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (33));
            skill->GetVictim ()->SetTime (12000 + 100);
            skill->GetVictim ()->SetRatio (0.25);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetSlow (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (25));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetValue (0.05 * skill->GetPlayer ()->GetMp ());
            skill->GetVictim ()->SetHeal (1);
            return true;
        }
#endif
    };
}
#endif
