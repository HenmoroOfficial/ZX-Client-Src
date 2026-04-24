#ifndef __CPPGEN_GNET_SKILL677
#define __CPPGEN_GNET_SKILL677
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill677:public Skill
    {
      public:
        enum
        { SKILL_ID = 677 };
          Skill677 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill677Stub:public SkillStub
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
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill677Stub ():SkillStub (677)
        {
            occupation = 10;
            name = L"相续不空";
            nativename = "相续不空";
            icon = "相续不空.dds";
            maxlevel = 5;
            maxlearn = 3;
            type = 2;
            eventflag = 1;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            talent_type = 1;
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
            action[0] = "天音寺_相续不空";
            action[1] = "天音寺_相续不空";
            action[2] = "天音寺_相续不空";
            action[3] = "天音寺_相续不空";
            action[4] = "天音寺_相续不空";
            action[5] = "0";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "天音寺_骑乘_相续不空";
            action[16] = "天音寺_骑乘_相续不空";
            action[17] = "天音寺_骑乘_相续不空";
            action[18] = "天音寺_骑乘_相续不空";
            action[19] = "天音寺_骑乘_相续不空";
            action[20] = "0";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 5;
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
#endif
        }
        virtual ~ Skill677Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 676 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (120 + 5 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 90000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[3] = { 45, 45, 45 };
            return array[level - 1];
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
            return _snwprintf (buffer, length, format,
                               INT (120 + 5 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel ()),
                               35 + 15 * skill->GetLevel (),
                               4 * skill->GetLevel (), 8 + 2 * skill->GetLevel (), 4 * skill->GetLevel (), 5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (15000 + 5000 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio (0.35 + 0.15 * skill->GetLevel ());
            skill->GetVictim ()->SetAmount (4);
            skill->GetVictim ()->SetValue (8 + 2 * skill->GetLevel ());
            skill->GetVictim ()->SetFocusanti (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (5000 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->
                SetValue ((skill->GetPlayer ()->GetMaxhp () + skill->GetPlayer ()->GetMaxmp () - skill->GetPlayer ()->GetHp () -
                           skill->GetPlayer ()->GetMp ()) * (0.05 * skill->GetLevel () + 0.002 * skill->GetPlayer ()->GetLevel ()) + 1);
            skill->GetVictim ()->SetAdddefence (1);
            return true;
        }
#endif
    };
}
#endif
