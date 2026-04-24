#ifndef __CPPGEN_GNET_SKILL241
#define __CPPGEN_GNET_SKILL241
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill241:public Skill
    {
      public:
        enum
        { SKILL_ID = 241 };
          Skill241 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill241Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1600;
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill241Stub ():SkillStub (241)
        {
            occupation = 9;
            name = L"天仙护体";
            nativename = "天仙护体";
            icon = "天仙护体.dds";
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
            skill_class = 2;
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
            talent[0] = 592;
            talent[1] = 594;
            talent[2] = 596;
            talent[3] = 1590;
            talent_size = 4;
            action[0] = "青云门_天仙护体";
            action[1] = "青云门_天仙护体";
            action[2] = "青云门_天仙护体";
            action[3] = "青云门_天仙护体";
            action[4] = "青云门_天仙护体";
            action[5] = "青云门_天仙护体";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "青云门_骑乘_天仙护体";
            action[16] = "青云门_骑乘_天仙护体";
            action[17] = "青云门_骑乘_天仙护体";
            action[18] = "青云门_骑乘_天仙护体";
            action[19] = "青云门_骑乘_天仙护体";
            action[20] = "青云门_骑乘_天仙护体";
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
            rangetype = 5;
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
#endif
        }
        virtual ~ Skill241Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 313 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 4 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (117 - 1 * skill->GetLevel () * skill->GetLevel () + 51 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 120000 - 10000 * skill->GetT2 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[5] = { 85, 90, 95, 101, 105 };
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
                               INT (117 - 1 * skill->GetLevel () * skill->GetLevel () + 51 * skill->GetLevel ()),
                               10 + 10 * skill->GetLevel (), 200 * skill->GetLevel (), 6 + 6 * skill->GetLevel (), 5 + 5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (6000 + 6000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue ((10 + 10 * skill->GetLevel ()) * (1 + 4 * skill->GetT1 ()) *
                                           (1 + INT (zrand (100) / (100 - 9 * skill->GetT1 ()))) * (1 + 0.1 * skill->GetT3 ()));
            skill->GetVictim ()->SetAdddefence (1);
            skill->GetVictim ()->SetTime (300000 + 300000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetValue (200 * skill->GetLevel () * (1 + 0.1 * skill->GetT3 ()));
            skill->GetVictim ()->SetAddmp (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (4000 * norm (skill->GetT0 ()));
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue ((117 - 1 * skill->GetLevel () * skill->GetLevel () + 51 * skill->GetLevel ()) * 0.15 * skill->GetT0 ());
            skill->GetVictim ()->SetMpgen (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (12000 * skill->GetT1 ());
            skill->GetVictim ()->SetRatio (0.05 * skill->GetT1 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetIncanti (1);
            return true;
        }
#endif
    };
}
#endif
