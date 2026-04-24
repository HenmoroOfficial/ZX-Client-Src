#ifndef __CPPGEN_GNET_SKILL526
#define __CPPGEN_GNET_SKILL526
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill526:public Skill
    {
      public:
        enum
        { SKILL_ID = 526 };
          Skill526 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill526Stub:public SkillStub
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
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes3 ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill526Stub ():SkillStub (526)
        {
            occupation = 13;
            name = L"无间寂";
            nativename = "无间寂";
            icon = "无间寂.dds";
            maxlevel = 20;
            maxlearn = 7;
            type = 3;
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
            skill_class = 0;
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
            talent[0] = 633;
            talent[1] = 648;
            talent[2] = 668;
            talent_size = 3;
            action[0] = "鬼王宗_无间寂";
            action[1] = "鬼王宗_无间寂";
            action[2] = "鬼王宗_无间寂";
            action[3] = "鬼王宗_无间寂";
            action[4] = "鬼王宗_无间寂";
            action[5] = "鬼王宗_无间寂";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "鬼王宗_骑乘_无间寂";
            action[16] = "鬼王宗_骑乘_无间寂";
            action[17] = "鬼王宗_骑乘_无间寂";
            action[18] = "鬼王宗_骑乘_无间寂";
            action[19] = "鬼王宗_骑乘_无间寂";
            action[20] = "鬼王宗_骑乘_无间寂";
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
            rangetype = 2;
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
        virtual ~ Skill526Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 524 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 4 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (632 + 16 * skill->GetLevel () + 1.7 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 120000 - 12000 * skill->GetT0 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[7] = { 125, 125, 125, 125, 125, 125, 125 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (9 + 3 * skill->GetLevel ());
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (9 + 3 * skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (6 + 3 * skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (632 + 16 * skill->GetLevel () + 1.7 * skill->GetLevel () * skill->GetLevel ()),
                               40, 9 + 3 * skill->GetLevel (), 3 + skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () > 600 ? skill->GetPlayer ()->GetVar1 () : 600));
            skill->GetVictim ()->SetTime ((3000 + 1000 * skill->GetLevel ()) * (1 + 0.3 * skill->GetT1 ()) + 100);
            skill->GetVictim ()->SetWrap (1);
            skill->GetVictim ()->SetProbability (1.0 * (100 * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetTime (14000 - 2000 * skill->GetT2 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount ((60 + 120 * skill->GetLevel ()) * skill->GetT2 () * skill->GetLevel ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (600));
            skill->GetVictim ()->SetTime (2050 * skill->GetT1 ());
            skill->GetVictim ()->SetSilent (1);
            skill->GetVictim ()->SetProbability (1.0 * (600));
            skill->GetVictim ()->SetTime ((3000 + 1000 * skill->GetLevel ()) * (1 + 0.3 * skill->GetT1 ()) + 100);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetSlow (1);
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
