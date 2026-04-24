#ifndef __CPPGEN_GNET_SKILL3326
#define __CPPGEN_GNET_SKILL3326
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3326:public Skill
    {
      public:
        enum
        { SKILL_ID = 3326 };
          Skill3326 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3326Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1200;
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3326Stub ():SkillStub (3326)
        {
            occupation = 128;
            name = L"琼台雅韵";
            nativename = "琼台雅韵";
            icon = "琼台雅韵.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 2;
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
            action[0] = "天华_妆台秋思";
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
            action[12] = "天华_妆台秋思";
            action[13] = "";
            action[14] = "";
            action[15] = "天华_骑乘_妆台秋思";
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
            action[27] = "天华_骑乘_妆台秋思";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 2;
            doenchant = true;
            dobless = true;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = 12;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill3326Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (2000);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 60000 - 8000 * 3;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (15);
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
            return _snwprintf (buffer, length, format, 2000, 5000 + 1000 * 5, 30 + 5 * 5, 14 + 6 * 5);

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
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetValue (5000 + 1000 * 5);
            skill->GetVictim ()->SetHeal (1);
            skill->GetVictim ()->SetProbability (1.0 * (30 + 5 * 5 + 5 * 3));
            skill->GetVictim ()->SetRatio (4099);
            skill->GetVictim ()->SetAmount (15);
            skill->GetVictim ()->SetValue (14);
            skill->GetVictim ()->SetDispel (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetRatio (48);
            skill->GetVictim ()->SetAmount (49);
            skill->GetVictim ()->SetValue (50);
            skill->GetVictim ()->SetDispel (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetBuffid (99);
            skill->GetVictim ()->SetAmount (2);
            skill->GetVictim ()->SetValue (3);
            skill->GetVictim ()->SetExorcism (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (14 + 6 * 5 + 3));
            skill->GetVictim ()->SetTime (5000 + 2000 * norm (3));
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetSkillmirror (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (6);
        }
#endif
    };
}
#endif
