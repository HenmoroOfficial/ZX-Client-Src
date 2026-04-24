#ifndef __CPPGEN_GNET_SKILL302
#define __CPPGEN_GNET_SKILL302
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill302:public Skill
    {
      public:
        enum
        { SKILL_ID = 302 };
          Skill302 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill302Stub:public SkillStub
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
      Skill302Stub ():SkillStub (302)
        {
            occupation = 5;
            name = L"相思";
            nativename = "相思";
            icon = "相思.dds";
            maxlevel = 20;
            maxlearn = 9;
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
            talent[0] = 762;
            talent[1] = 763;
            talent[2] = 764;
            talent_size = 3;
            action[0] = "合欢派_相思";
            action[1] = "合欢派_相思";
            action[2] = "合欢派_相思";
            action[3] = "合欢派_相思";
            action[4] = "合欢派_相思";
            action[5] = "合欢派_相思";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "合欢派_骑乘_相思";
            action[16] = "合欢派_骑乘_相思";
            action[17] = "合欢派_骑乘_相思";
            action[18] = "合欢派_骑乘_相思";
            action[19] = "合欢派_骑乘_相思";
            action[20] = "合欢派_骑乘_相思";
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
        virtual ~ Skill302Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 296 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 5 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (2.7 * skill->GetLevel () * skill->GetLevel () + 3.4 * skill->GetLevel () + 238);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 180000 - 14400 * skill->GetT1 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 55, 60, 65, 71, 75, 78, 80, 83, 86 };
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
                               INT (2.7 * skill->GetLevel () * skill->GetLevel () + 3.4 * skill->GetLevel () + 238),
                               INT (12 * skill->GetLevel () * skill->GetLevel () + 186 * skill->GetLevel () + 1800),
                               22 + 2 * skill->GetLevel (), INT (4.8 * skill->GetLevel () * skill->GetLevel () + 177 * skill->GetLevel () + 1130));

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
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetValue ((12 * skill->GetLevel () * skill->GetLevel () + 186 * skill->GetLevel () + 1800) *
                                           (1 + 0.15 * skill->GetT0 ()));
            skill->GetVictim ()->SetHeal (1);
            skill->GetVictim ()->SetProbability (1.0 * (25 - 3 * skill->GetT0 ()));
            skill->GetVictim ()->SetTime (22000 + 2000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount ((4.8 * skill->GetLevel () * skill->GetLevel () + 177 * skill->GetLevel () + 1130) *
                                            (1 - 0.1 * skill->GetT2 ()));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak6 (1);
            return true;
        }
#endif
    };
}
#endif
