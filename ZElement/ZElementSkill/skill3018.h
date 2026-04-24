#ifndef __CPPGEN_GNET_SKILL3018
#define __CPPGEN_GNET_SKILL3018
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3018:public Skill
    {
      public:
        enum
        { SKILL_ID = 3018 };
          Skill3018 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3018Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes3 ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3018Stub ():SkillStub (3018)
        {
            occupation = 167;
            name = L"天尊法身<玄>";
            nativename = "天尊法身<玄>";
            icon = "仙·天尊法身.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 2;
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
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "青云门_骑乘_天尊法身";
            action[16] = "青云门_骑乘_天尊法身";
            action[17] = "青云门_骑乘_天尊法身";
            action[18] = "青云门_骑乘_天尊法身";
            action[19] = "青云门_骑乘_天尊法身";
            action[20] = "青云门_骑乘_天尊法身";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
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
        virtual ~ Skill3018Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (180 + 26 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 784 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 5 };
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
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
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
                               180 + 26 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel (),
                               120, 20, skill->GetLevel (), skill->GetLevel () + 1, 4 + skill->GetLevel (), 1 + skill->GetLevel ());

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
            skill->GetVictim ()->SetValue ((0.01 * skill->GetPlayer ()->GetMaxhp () * skill->GetLevel ()) * (1 + 0.3 * skill->GetT0 ()));
            skill->GetVictim ()->SetFashen (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (4000 + 1000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetAmount (skill->GetLevel ());
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetVar1 ());
            skill->GetVictim ()->SetFrenzied (1);
            return true;
        }
#endif
    };
}
#endif
