#ifndef __CPPGEN_GNET_SKILL305
#define __CPPGEN_GNET_SKILL305
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill305:public Skill
    {
      public:
        enum
        { SKILL_ID = 305 };
          Skill305 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill305Stub:public SkillStub
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
      Skill305Stub ():SkillStub (305)
        {
            occupation = 4;
            name = L"热血";
            nativename = "热血";
            icon = "热血.dds";
            maxlevel = 20;
            maxlearn = 8;
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
            skill_class = 1;
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
            talent[0] = 740;
            talent_size = 1;
            action[0] = "合欢派_热情";
            action[1] = "合欢派_热情";
            action[2] = "合欢派_热情";
            action[3] = "合欢派_热情";
            action[4] = "合欢派_热情";
            action[5] = "合欢派_热情";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "合欢派_骑乘_热情";
            action[16] = "合欢派_骑乘_热情";
            action[17] = "合欢派_骑乘_热情";
            action[18] = "合欢派_骑乘_热情";
            action[19] = "合欢派_骑乘_热情";
            action[20] = "合欢派_骑乘_热情";
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
        virtual ~ Skill305Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 291 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0.59 * skill->GetLevel () * skill->GetLevel () + 8 * skill->GetLevel () + 60);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 4000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[8] = { 35, 39, 43, 47, 49, 51, 53, 56 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (4.5 + 0.5 * skill->GetLevel ());
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
                               INT (0.59 * skill->GetLevel () * skill->GetLevel () + 8 * skill->GetLevel () + 60),
                               20 * skill->GetLevel (), 5 * skill->GetLevel (), 30);

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
            skill->GetVictim ()->SetTime (1800000 + 900000 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (4);
            skill->GetVictim ()->
                SetValue ((0.2 * skill->GetLevel () * skill->GetPlayer ()->GetLevel () +
                           0.05 * skill->GetLevel () * skill->GetPlayer ()->GetMaxmp ()) * (1 + 0.35 * skill->GetT0 ()));
            skill->GetVictim ()->SetAddhp (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (12);
        }
#endif
    };
}
#endif
