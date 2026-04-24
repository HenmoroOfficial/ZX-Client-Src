#ifndef __CPPGEN_GNET_SKILL2487
#define __CPPGEN_GNET_SKILL2487
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2487:public Skill
    {
      public:
        enum
        { SKILL_ID = 2487 };
          Skill2487 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2487Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1200;
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
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 ((skill->GetPlayer ()->GetMp () - skill->GetPlayer ()->GetMaxmp () * 0.05 * skill->GetLevel ()) >
                                              0 ? 1 : 0);
                skill->GetPlayer ()->SetMp ((skill->GetPlayer ()->GetMp () - skill->GetPlayer ()->GetMaxmp () * 0.05 * skill->GetLevel ()) >
                                            0 ? (skill->GetPlayer ()->GetMp () -
                                                 skill->GetPlayer ()->GetMaxmp () * 0.05 * skill->GetLevel ()) : skill->GetPlayer ()->GetMp ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2487Stub ():SkillStub (2487)
        {
            occupation = 53;
            name = L"ÌıËªÑÅÔÏ";
            nativename = "ÌıËªÑÅÔÏ";
            icon = "ÌıËªÑÅÔÏ.dds";
            maxlevel = 15;
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
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 8;
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
            talent[0] = 2595;
            talent[1] = 2718;
            talent_size = 2;
            action[0] = "Ìì»ª_Ëªºè";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "Ìì»ª_Ëªºè";
            action[13] = "";
            action[14] = "";
            action[15] = "Ìì»ª_Æï³Ë_Ëªºè";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "Ìì»ª_Æï³Ë_Ëªºè";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 2;
            doenchant = true;
            dobless = true;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = 12;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill2487Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 2486 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 60000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 85, 87, 89, 91, 93, 95, 97, 99, 101 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (10);
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
            return _snwprintf (buffer, length, format, 5 * skill->GetLevel (), 6 * skill->GetLevel (), 8 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (120 * skill->GetPlayer ()->GetVar1 ()));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetValue (0.06 * skill->GetLevel () * skill->GetPlayer ()->GetMaxmp ());
            skill->GetVictim ()->SetHeal (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (8000);
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0.08 * skill->GetLevel () * skill->GetPlayer ()->GetMaxmp ());
            skill->GetVictim ()->SetHpgen (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (6000 * norm (skill->GetT0 ()));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0.05 * skill->GetT0 () * skill->GetPlayer ()->GetMaxmp ());
            skill->GetVictim ()->SetHpgen (1);
            skill->GetVictim ()->SetProbability (1.0 * (10 + skill->GetT1 ()));
            skill->GetVictim ()->SetTime (3000 + 2000 * norm (skill->GetT1 ()));
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetSkillmirror (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (6);
        }
#endif
    };
}
#endif
