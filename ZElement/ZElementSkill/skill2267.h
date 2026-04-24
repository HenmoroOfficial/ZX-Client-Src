#ifndef __CPPGEN_GNET_SKILL2267
#define __CPPGEN_GNET_SKILL2267
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2267:public Skill
    {
      public:
        enum
        { SKILL_ID = 2267 };
          Skill2267 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2267Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 2700;
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
                return 100;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.1 * skill->GetLevel () + 0.05 * skill->GetT1 ());
                skill->SetPlus (0.15 * skill->GetPlayer ()->GetLevel () * skill->GetLevel () * skill->GetT0 () + 720 + 30 * skill->GetLevel () +
                                5 * skill->GetLevel () * skill->GetLevel ());
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
                return 100;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.1 * skill->GetLevel () + 0.05 * skill->GetT1 ());
                skill->SetPlus (0.15 * skill->GetPlayer ()->GetLevel () * skill->GetLevel () * skill->GetT0 () + 720 + 30 * skill->GetLevel () +
                                5 * skill->GetLevel () * skill->GetLevel ());
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
                return 100;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrit (0.1);
                skill->SetRatio (1 + 0.1 * skill->GetLevel () + 0.05 * skill->GetT1 ());
                skill->SetPlus (0.15 * skill->GetPlayer ()->GetLevel () * skill->GetLevel () * skill->GetT0 () + 720 + 30 * skill->GetLevel () +
                                5 * skill->GetLevel () * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill2267Stub ():SkillStub (2267)
        {
            occupation = 128;
            name = L"普智无量";
            nativename = "普智无量";
            icon = "无量真言.dds";
            maxlevel = 20;
            maxlearn = 20;
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
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
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
            preskillnum = 1;
            talent[0] = 675;
            talent[1] = 681;
            talent[2] = 701;
            talent_size = 3;
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
#endif
        }
        virtual ~ Skill2267Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 787 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (3.3 * skill->GetLevel () * skill->GetLevel () + 60 * skill->GetLevel () + 1850);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 15000 - 2250 * skill->GetT0 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 0 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (13);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 15);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 10);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (3.3 * skill->GetLevel () * skill->GetLevel () + 60 * skill->GetLevel () + 1850),
                               35,
                               12,
                               10,
                               720 + 30 * skill->GetLevel () + 5 * skill->GetLevel () * skill->GetLevel (),
                               8,
                               3 * skill->GetLevel (),
                               15, 180 + 15 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel (), 3, 40 + 10 * skill->GetLevel (), 10);

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
            skill->GetVictim ()->SetTime (15000 + 100);
            skill->GetVictim ()->SetRatio (0.4 + 0.1 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecanti (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (180 + 15 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel () +
                                                  5 * skill->GetT2 () * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (15000 + 100);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetSleep (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetRatio (0.1 * skill->GetT2 ());
            skill->GetVictim ()->SetDrainmagic (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (9000 + 100);
            skill->GetVictim ()->SetRatio (0.03 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetIncanti (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (35);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
