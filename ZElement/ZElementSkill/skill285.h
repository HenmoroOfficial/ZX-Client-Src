#ifndef __CPPGEN_GNET_SKILL285
#define __CPPGEN_GNET_SKILL285
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill285:public Skill
    {
      public:
        enum
        { SKILL_ID = 285 };
          Skill285 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill285Stub:public SkillStub
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
      Skill285Stub ():SkillStub (285)
        {
            occupation = 12;
            name = L"金刚护体";
            nativename = "金刚护体";
            icon = "金刚护体.dds";
            maxlevel = 12;
            maxlearn = 5;
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
            talent[0] = 695;
            talent[1] = 704;
            talent_size = 2;
            action[0] = "天音寺_金刚护体";
            action[1] = "天音寺_金刚护体";
            action[2] = "天音寺_金刚护体";
            action[3] = "天音寺_金刚护体";
            action[4] = "天音寺_金刚护体";
            action[5] = "天音寺_金刚护体";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "天音寺_骑乘_金刚护体";
            action[16] = "天音寺_骑乘_金刚护体";
            action[17] = "天音寺_骑乘_金刚护体";
            action[18] = "天音寺_骑乘_金刚护体";
            action[19] = "天音寺_骑乘_金刚护体";
            action[20] = "天音寺_骑乘_金刚护体";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "技能攻击1";
            rangetype = 0;
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
        virtual ~ Skill285Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 277 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 4 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (9 + skill->GetLevel ());
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (125 * skill->GetLevel () + 378 - 2.6 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 10000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[5] = { 85, 90, 95, 99, 105 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (8 + skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (6 + skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (125 * skill->GetLevel () + 378 - 2.6 * skill->GetLevel () * skill->GetLevel ()),
                               7 + skill->GetLevel () + INT (skill->GetLevel () / 4) + 2 * INT (skill->GetLevel () / 5), 6.0 + skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (1800000 + 900000 * skill->GetT1 ());
            skill->GetVictim ()->SetRatio (0.01 * (7 + skill->GetLevel () + INT (skill->GetLevel () / 3.98) + 2 * INT (skill->GetLevel () / 4.98)) +
                                           0.01 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetIncdefence (1);
            return true;
        }
#endif
    };
}
#endif
