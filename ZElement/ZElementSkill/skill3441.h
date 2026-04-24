#ifndef __CPPGEN_GNET_SKILL3441
#define __CPPGEN_GNET_SKILL3441
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3441:public Skill
    {
      public:
        enum
        { SKILL_ID = 3441 };
          Skill3441 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3441Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 3000;
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
                return 5000;
            }
            void Calculate (Skill * skill) const
            {
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
                return 2000;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3441Stub ():SkillStub (3441)
        {
            occupation = 66;
            name = L"劫火明尊";
            nativename = "劫火明尊";
            icon = "劫火明尊·阵法.dds";
            maxlevel = 20;
            maxlearn = 5;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 3;
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
            cycle = 1;
            cyclegfx = "法阵连数";
            cyclemode = 1;
            skill_class = 9;
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
            talent[0] = 3418;
            talent[1] = 3421;
            talent[2] = 3422;
            talent[3] = 3408;
            talent[4] = 3758;
            talent_size = 5;
            action[0] = "焚香谷_阵法精研【连数】";
            action[1] = "焚香谷_阵法精研【连数】";
            action[2] = "焚香谷_阵法精研【连数】";
            action[3] = "焚香谷_阵法精研【连数】";
            action[4] = "焚香谷_阵法精研【连数】";
            action[5] = "焚香谷_阵法精研【连数】";
            action[6] = "焚香谷_阵法精研【连数】";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "焚香谷_骑乘_阵法精研【连数】";
            action[16] = "焚香谷_骑乘_阵法精研【连数】";
            action[17] = "焚香谷_骑乘_阵法精研【连数】";
            action[18] = "焚香谷_骑乘_阵法精研【连数】";
            action[19] = "焚香谷_骑乘_阵法精研【连数】";
            action[20] = "焚香谷_骑乘_阵法精研【连数】";
            action[21] = "焚香谷_骑乘_阵法精研【连数】";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "";
            rangetype = 2;
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
#endif
        }
        virtual ~ Skill3441Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3440 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 4 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1500);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 5000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 120000 - 15000 * skill->GetT2 () - 120000 * 0.02 * skill->GetT4 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[5] = { 100, 105, 110, 115, 120 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (28);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 5 * skill->GetLevel () + 5, 5, 2 * skill->GetLevel (), 10);

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
            skill->GetVictim ()->SetProbability (1.0 * (20 * skill->GetT3 ()));
            skill->GetVictim ()->SetBreakcasting (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->
                SetTime ((5 * skill->GetLevel () + skill->GetPlayer ()->GetCyclemembercnt () * 5 +
                          skill->GetPlayer ()->GetCyclemembercnt () * skill->GetT1 ()) * 1000 + 10000 * int (skill->GetCharging () / 4800) +
                         1000 * skill->GetT4 ());
              skill->GetVictim ()->SetRatio (skill->GetLevel ());
              skill->GetVictim ()->SetBuffid (0);
              skill->GetVictim ()->SetAmount (3606);
              skill->GetVictim ()->SetValue (2);
              skill->GetVictim ()->SetCycle (1);
              skill->GetVictim ()->SetRatio (zrand (100) < skill->GetPlayer ()->GetSkilllevel (3759) * 2 ? 3449 : 218);
              skill->GetVictim ()->SetAmount (218);
              skill->GetVictim ()->SetValue (218);
              skill->GetVictim ()->SetClearcooldown (1);
              skill->GetVictim ()->SetProbability (1.0 * (120));
              skill->GetVictim ()->SetTime (50);
              skill->GetVictim ()->
                SetRatio ((skill->GetPlayer ()->GetCyclemembercnt () ==
                           6 ? 3 : (skill->GetPlayer ()->GetCyclemembercnt () <=
                                    3 ? 1 : 2)) * 100 + 5 * skill->GetLevel () + skill->GetPlayer ()->GetCyclemembercnt () * 5 +
                          skill->GetPlayer ()->GetCyclemembercnt () * skill->GetT1 () + 10 * int (skill->GetCharging () / 4800));
              skill->GetVictim ()->SetBuffid (1);
              skill->GetVictim ()->SetValue (3645);
              skill->GetVictim ()->SetDelaycast (1);
              skill->GetVictim ()->SetTime (900);
              skill->GetVictim ()->SetRatio (0);
              skill->GetVictim ()->SetAmount (0);
              skill->GetVictim ()->SetValue (0);
              skill->GetVictim ()->SetEvilaura (1);
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
