#ifndef __CPPGEN_GNET_SKILL3026
#define __CPPGEN_GNET_SKILL3026
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3026:public Skill
    {
      public:
        enum
        { SKILL_ID = 3026 };
          Skill3026 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3026Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar2 (1);
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes5 ());
                skill->SetRatio (1 + 0.06 * skill->GetT0 () + 0.05 * skill->GetLevel () +
                                 norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) + 0.05 * skill->GetT1 ());
                skill->SetPlus (0.15 * skill->GetPlayer ()->GetLevel () * skill->GetLevel () * skill->GetT0 () + 720 + 30 * skill->GetLevel () +
                                5 * skill->GetLevel () * skill->GetLevel ());
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
                skill->GetPlayer ()->SetVar2 (0);
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes5 ());
                skill->SetRatio (1 + 0.06 * skill->GetT0 () + 0.05 * skill->GetLevel () +
                                 norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) + 0.05 * skill->GetT1 ());
                skill->SetPlus (0.15 * skill->GetPlayer ()->GetLevel () * skill->GetLevel () * skill->GetT0 () + 720 + 30 * skill->GetLevel () +
                                5 * skill->GetLevel () * skill->GetLevel ());
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
                skill->GetPlayer ()->SetVar2 (0);
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes5 ());
                skill->SetCrit (0.1);
                skill->SetRatio (1 + 0.06 * skill->GetT0 () + 0.05 * skill->GetLevel () +
                                 norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) + 0.05 * skill->GetT1 ());
                skill->SetPlus (0.15 * skill->GetPlayer ()->GetLevel () * skill->GetLevel () * skill->GetT0 () + 720 + 30 * skill->GetLevel () +
                                5 * skill->GetLevel () * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill3026Stub ():SkillStub (3026)
        {
            occupation = 167;
            name = L"无量真言<玄>";
            nativename = "无量真言<玄>";
            icon = "仙·无量真言.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            clearmask = 1;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 3;
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
        virtual ~ Skill3026Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (200 + 34 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 788 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 400, 500, 500, 700, 1500, 2400, 3500, 5600, 8500, 8600, 19000, 26500, 49100, 57600, 85800, 140300, 241000, 410600, 614500,
 1344300 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 2, 4, 6, 8, 10, 12, 15, 18, 21, 24, 28, 33, 38, 43, 48, 54, 60, 66, 72, 78 };
            return array[level - 1];
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
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (15);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (14);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (13);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (3.3 * skill->GetLevel () * skill->GetLevel () + 60 * skill->GetLevel () + 1850),
                               200 + 34 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel (),
                               15,
                               INT (6 + skill->GetLevel () / 1.9),
                               15,
                               INT (5 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 3 * (skill->GetLevel () - 15)),
                               720 + 30 * skill->GetLevel () + 5 * skill->GetLevel () * skill->GetLevel (),
                               2 + 0.5 * skill->GetLevel (),
                               15, 50 + skill->GetLevel (), 3, 10 + skill->GetLevel (), 10, 2 + 0.5 * skill->GetLevel (), skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (3000 + 100);
            skill->GetVictim ()->SetRatio (0.1 + 0.01 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecanti (1);
            skill->GetVictim ()->SetProbability (1.0 * ((0.5 + 0.01 * skill->GetLevel ()) * skill->GetPlayer ()->GetVar1 ()));
            skill->GetVictim ()->SetTime (15000 + 100);
            skill->GetVictim ()->SetValue (0.1);
            skill->GetVictim ()->SetDsleep (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetRatio (0.1 * skill->GetT2 ());
            skill->GetVictim ()->SetDrainmagic (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (2050 + 500 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (0.01 * skill->GetPlayer ()->GetMaxatk ());
            skill->GetVictim ()->SetAddanti (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar2 () == 1 ? 120 : 0));
            skill->GetVictim ()->SetTime (14800 - 2250 * skill->GetT0 ());
            skill->GetVictim ()->SetRatio (788);
            skill->GetVictim ()->SetAmount (220);
            skill->GetVictim ()->SetValue (220);
            skill->GetVictim ()->SetSetcooldown (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (2050 + 500 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio (0.01 * skill->GetLevel ());
            skill->GetVictim ()->SetAttack2mp (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (6 + INT (skill->GetLevel () / 1.9));
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
