#ifndef __CPPGEN_GNET_SKILL1888
#define __CPPGEN_GNET_SKILL1888
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1888:public Skill
    {
      public:
        enum
        { SKILL_ID = 1888 };
          Skill1888 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1888Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1888Stub ():SkillStub (1888)
        {
            occupation = 39;
            name = L"¶¾Ñ×Öä";
            nativename = "¶¾Ñ×Öä";
            icon = "¶¾Ñ×Öä.dds";
            maxlevel = 10;
            maxlearn = 5;
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
            skill_class = 6;
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
            talent[0] = 2136;
            talent[1] = 2137;
            talent[2] = 2141;
            talent_size = 3;
            action[0] = "ºüÑý_¶¾Ñ×Öä";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "ºüÑý_¶¾Ñ×Öä";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ºüÑý_Æï³Ë_¶¾Ñ×Öä";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "ºüÑý_Æï³Ë_¶¾Ñ×Öä";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
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
        virtual ~ Skill1888Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1887 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 4 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (22 + skill->GetT2 ());
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (175);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 12000 - 2000 * skill->GetT0 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[5] = { 35, 40, 45, 50, 55 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (17 + skill->GetT2 ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10 + skill->GetT2 ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetLevel (), 2 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 *
                                                 ((skill->GetPlayer ()->GetType () != 0 ? 100 : 0) * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetTime (10000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->
                SetAmount ((0.01 * skill->GetLevel () * skill->GetPlayer ()->GetHp () <
                            skill->GetPlayer ()->GetVar1 () * 0.5 ? 0.01 * skill->GetLevel () *
                            skill->GetPlayer ()->GetHp () : skill->GetPlayer ()->GetVar1 () * 0.5) * 5);
            skill->GetVictim ()->SetValue (zrand (100) < 10 * skill->GetT1 ()? 0.1 * skill->GetPlayer ()->GetMaxhp () : 0);
            skill->GetVictim ()->SetHpleak (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 ((skill->GetPlayer ()->GetType () == 0 ? 100 : 0) * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetTime (10000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->
                SetAmount ((0.02 * skill->GetLevel () * skill->GetPlayer ()->GetHp () <
                            skill->GetPlayer ()->GetVar1 ()? 0.02 * skill->GetLevel () *
                            skill->GetPlayer ()->GetHp () : skill->GetPlayer ()->GetVar1 ()) * 5);
            skill->GetVictim ()->SetValue (zrand (100) < 10 * skill->GetT1 ()? 0.1 * skill->GetPlayer ()->GetMaxhp () : 0);
            skill->GetVictim ()->SetHpleak (1);
            return true;
        }
#endif
    };
}
#endif
