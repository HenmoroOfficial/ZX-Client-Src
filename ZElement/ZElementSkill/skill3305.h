#ifndef __CPPGEN_GNET_SKILL3305
#define __CPPGEN_GNET_SKILL3305
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3305:public Skill
    {
      public:
        enum
        { SKILL_ID = 3305 };
          Skill3305 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3305Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 125;
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
                return 100;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (1);
                skill->SetRatio (1 + 0.1 * 3 + 0.05 * 3 + 0.04 * 3);
                skill->SetPlus (0.15 * skill->GetPlayer ()->GetLevel () * 3 * 3 + 720 + 30 * 3 + 5 * 3 * 3);
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
                return 1150;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (0);
                skill->SetRatio (1 + 0.1 * 3 + 0.05 * 3 + 0.04 * 3);
                skill->SetPlus (0.15 * skill->GetPlayer ()->GetLevel () * 3 * 3 + 720 + 30 * 3 + 5 * 3 * 3);
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
                return 1625;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (0);
                skill->SetCrit (0.1);
                skill->SetRatio (1 + 0.1 * 3 + 0.05 * 3 + 0.04 * 3);
                skill->SetPlus (0.15 * skill->GetPlayer ()->GetLevel () * 3 * 3 + 720 + 30 * 3 + 5 * 3 * 3);
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill3305Stub ():SkillStub (3305)
        {
            occupation = 128;
            name = L"无量真言";
            nativename = "无量真言";
            icon = "无量真言.dds";
            maxlevel = 1;
            maxlearn = 1;
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
            action[0] = "天音寺_无量真言";
            action[1] = "天音寺_无量真言";
            action[2] = "天音寺_无量真言";
            action[3] = "天音寺_无量真言";
            action[4] = "天音寺_无量真言";
            action[5] = "天音寺_无量真言";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "天音寺_骑乘_无量真言";
            action[16] = "天音寺_骑乘_无量真言";
            action[17] = "天音寺_骑乘_无量真言";
            action[18] = "天音寺_骑乘_无量真言";
            action[19] = "天音寺_骑乘_无量真言";
            action[20] = "天音寺_骑乘_无量真言";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "技能攻击2";
            rangetype = 3;
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
#endif
        }
        virtual ~ Skill3305Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1000);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 15000 - 2250 * 3;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (13);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (13);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               1000, 15, 12, 10, 720 + 30 * 3 + 5 * 3 * 3, 8, 3 * 3, 15, 180 + 15 * 3 + 3 * 3, 3, 40 + 10 * 3, 10);

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
            skill->GetVictim ()->SetTime (3000 + 100 + 600 * 3);
            skill->GetVictim ()->SetRatio (skill->GetPlayer ()->GetSaint () >
                                           9 ? (0.42 + 0.1 * 3 - skill->GetPlayer ()->GetSaint () * 0.002 >
                                                0.1 ? 0.42 + 0.1 * 3 - skill->GetPlayer ()->GetSaint () * 0.002 : 0.1) : 0.4 + 0.1 * 3);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecanti (1);
            skill->GetVictim ()->SetProbability (1.0 * (180 + 15 * 3 + 3 * 3 + 5 * 3 * 3));
            skill->GetVictim ()->SetTime (15000 + 100);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetSleep (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetRatio (0.1 * 3);
            skill->GetVictim ()->SetDrainmagic (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (9000 + 100 + 1200 * 3);
            skill->GetVictim ()->SetRatio (0.03 * 3 + 0.02 * 3);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetIncanti (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () == 1 ? 120 : 0));
            skill->GetVictim ()->SetTime (14800 - 2250 * 3);
            skill->GetVictim ()->SetRatio (3026);
            skill->GetVictim ()->SetAmount (3074);
            skill->GetVictim ()->SetValue (3122);
            skill->GetVictim ()->SetSetcooldown (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (15);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
