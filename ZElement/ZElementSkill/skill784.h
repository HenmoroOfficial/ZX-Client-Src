#ifndef __CPPGEN_GNET_SKILL784
#define __CPPGEN_GNET_SKILL784
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill784:public Skill
    {
      public:
        enum
        { SKILL_ID = 784 };
          Skill784 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill784Stub:public SkillStub
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
      Skill784Stub ():SkillStub (784)
        {
            occupation = 20;
            name = L"天尊法身";
            nativename = "天尊法身";
            icon = "天尊法身.dds";
            maxlevel = 6;
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
            talent[0] = 578;
            talent[1] = 592;
            talent[2] = 594;
            talent[3] = 596;
            talent_size = 4;
            action[0] = "青云门_天尊法身";
            action[1] = "青云门_天尊法身";
            action[2] = "青云门_天尊法身";
            action[3] = "青云门_天尊法身";
            action[4] = "青云门_天尊法身";
            action[5] = "青云门_天尊法身";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "青云门_骑乘_天尊法身";
            action[16] = "青云门_骑乘_天尊法身";
            action[17] = "青云门_骑乘_天尊法身";
            action[18] = "青云门_骑乘_天尊法身";
            action[19] = "青云门_骑乘_天尊法身";
            action[20] = "青云门_骑乘_天尊法身";
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
        virtual ~ Skill784Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 783 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (560 + 30 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 120000 - 10000 * skill->GetT3 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[5] = { 135, 136, 136, 138, 139 };
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
                               560 + 30 * skill->GetLevel (),
                               20,
                               INT (500 * skill->GetLevel () + 75 * skill->GetLevel () * skill->GetLevel () * skill->GetLevel () -
                                    1500 * INT (skill->GetLevel () / 6.5)), skill->GetLevel () + 1);

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
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (20000 + 100);
            skill->GetVictim ()->SetRatio ((0.011 + 0.01 * skill->GetLevel () + 0.01 * skill->GetT2 ()) *
                                           (INT (zrand (99 + 5 * skill->GetT2 ()) / 100) + 1));
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->
                SetValue ((500 * skill->GetLevel () + 75 * skill->GetLevel () * skill->GetLevel () * skill->GetLevel () -
                           INT (skill->GetLevel () / 6.5) * 1500) * (1 + 0.3 * skill->GetT0 ()));
            skill->GetVictim ()->SetFashen (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (4000 * norm (skill->GetT1 ()));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0.02 * skill->GetT1 () * skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetHpgen (1);
            return true;
        }
#endif
    };
}
#endif
