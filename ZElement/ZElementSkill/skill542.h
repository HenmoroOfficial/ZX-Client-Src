#ifndef __CPPGEN_GNET_SKILL542
#define __CPPGEN_GNET_SKILL542
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill542:public Skill
    {
      public:
        enum
        { SKILL_ID = 542 };
          Skill542 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill542Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetBuffcnt ());
                skill->SetCrithurt (0.2 * skill->GetT2 ());
                skill->SetRatio (1 + 0.15 * skill->GetLevel () + 0.04 * skill->GetLevel () * skill->GetT2 ());
                skill->SetPlus (0.05 * skill->GetPlayer ()->GetMaxmp () *
                                INT (skill->GetPlayer ()->GetMp () / ((0.95 - 0.1 * skill->GetT3 ()) * skill->GetPlayer ()->GetMaxmp ())));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill542Stub ():SkillStub (542)
        {
            occupation = 19;
            name = L"真元华闪";
            nativename = "真元华闪";
            icon = "真元华闪.dds";
            maxlevel = 18;
            maxlearn = 7;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
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
            talent[1] = 596;
            talent[2] = 598;
            talent[3] = 612;
            talent_size = 4;
            action[0] = "青云门_真元华闪";
            action[1] = "青云门_真元华闪";
            action[2] = "青云门_真元华闪";
            action[3] = "青云门_真元华闪";
            action[4] = "青云门_真元华闪";
            action[5] = "青云门_真元华闪";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "青云门_骑乘_真元华闪";
            action[16] = "青云门_骑乘_真元华闪";
            action[17] = "青云门_骑乘_真元华闪";
            action[18] = "青云门_骑乘_真元华闪";
            action[19] = "青云门_骑乘_真元华闪";
            action[20] = "青云门_骑乘_真元华闪";
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
            rangetype = 0;
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
        virtual ~ Skill542Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 539 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (950 + 15 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 90000 - 10000 * skill->GetT1 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[7] = { 115, 118, 121, 124, 125, 125, 125 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (7);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (2);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 950 + 15 * skill->GetLevel (), 15 * skill->GetLevel ());

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
            skill->GetVictim ()->
                SetRatio ((0.05 * skill->GetLevel () + (2 * skill->GetPlayer ()->GetHp () + 100) / (skill->GetPlayer ()->GetMaxhp () + 100)) * (1 +
                                                                                                                                                0.04 *
                                                                                                                                                skill->
                                                                                                                                                GetT2
                                                                                                                                                () *
                                                                                                                                                skill->
                                                                                                                                                GetPlayer
                                                                                                                                                ()->
                                                                                                                                                GetVar1
                                                                                                                                                () +
                                                                                                                                                0.04 *
                                                                                                                                                skill->
                                                                                                                                                GetT2
                                                                                                                                                () *
                                                                                                                                                skill->
                                                                                                                                                GetPlayer
                                                                                                                                                ()->
                                                                                                                                                GetDebuffcnt
                                                                                                                                                ()));
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetSecondattack (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (4000 * norm (skill->GetT0 ()));
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0.15 * skill->GetT0 () * (950 + 15 * skill->GetLevel ()));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetMpgen (1);
            skill->GetVictim ()->
                SetBuffid (INT (skill->GetPlayer ()->GetMp () / ((0.95 - 0.1 * skill->GetT3 ()) * skill->GetPlayer ()->GetMaxmp ())));
            skill->GetVictim ()->SetUniqprompt (1);
            return true;
        }
#endif
    };
}
#endif
