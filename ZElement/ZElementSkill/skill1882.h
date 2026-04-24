#ifndef __CPPGEN_GNET_SKILL1882
#define __CPPGEN_GNET_SKILL1882
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1882:public Skill
    {
      public:
        enum
        { SKILL_ID = 1882 };
          Skill1882 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1882Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
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
                return 725;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (1);
                skill->SetRatio (0);
                skill->SetPlus (0);
                skill->GetPlayer ()->SetPerform (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State3:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (2);
                skill->SetRatio (1.02 + 0.02 * skill->GetLevel ());
                skill->SetPlus ((skill->GetT1 () > 0 ? 133 : 0) + 10 * skill->GetT1 () + (skill->GetT2 () > 0 ? 324 : 0) + 21 * skill->GetT2 ());
                skill->GetPlayer ()->SetPerform (0);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State4:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 875;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (3);
                skill->SetRatio (1.02 + 0.02 * skill->GetLevel ());
                skill->SetPlus ((skill->GetT3 () > 0 ? 1043 : 0) + 33 * skill->GetT3 () + (skill->GetT4 () > 0 ? 1543 : 0) + 32 * skill->GetT4 ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill1882Stub ():SkillStub (1882)
        {
            occupation = 37;
            name = L"鏖战";
            nativename = "鏖战";
            icon = "銮战.dds";
            maxlevel = 6;
            maxlearn = 5;
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
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 5;
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
            talent[0] = 2118;
            talent[1] = 1850;
            talent[2] = 1859;
            talent[3] = 1868;
            talent[4] = 1877;
            talent_size = 5;
            action[0] = "兽神_鏖战";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "兽神_鏖战";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "兽神_骑乘_鏖战";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "兽神_骑乘_鏖战";
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
            weapon_limit = 7;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
            statestub.push_back (new State3 ());
            statestub.push_back (new State4 ());
#endif
        }
        virtual ~ Skill1882Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1879 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (877 + 16 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 12000 - 1000 * skill->GetT0 () - 1000 * int (skill->GetT1 () / 8.99) - 1000 * int (skill->GetT2 () / 8.99) -
                1000 * int (skill->GetT3 () / 8.99) - 1000 * int (skill->GetT4 () / 8.99);
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[5] = { 136, 137, 138, 139, 140 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (3);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 877 + 16 * skill->GetLevel (), 2 + 2 * skill->GetLevel (), 4, 8);

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () == 2 ? (skill->GetT1 () > 0 ? 600 : 0) : 0));
            skill->GetVictim ()->SetTime (zrand (100) < 15 + 5 * skill->GetT1 ()? 4000 : 400);
            skill->GetVictim ()->SetSilent (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () == 2 ? (skill->GetT2 () > 0 ? 120 : 0) : 0));
            skill->GetVictim ()->SetTime (8000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (60 + 10 * skill->GetT2 () + 2 * skill->GetT2 () * skill->GetT2 ());
            skill->GetVictim ()->SetSubdefence (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () == 3 ? (skill->GetT3 () > 0 ? 120 : 0) : 0));
            skill->GetVictim ()->SetTime (8000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetVar1 () == 3 ? (skill->GetT3 () > 0 ? (35 + 5 * skill->GetT3 ()) : 0) : 0);
            skill->GetVictim ()->SetSubanti (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetVar1 () == 3 ? (skill->GetT4 () > 0 ? (35 + 5 * skill->GetT4 ()) : 0) : 0));
            skill->GetVictim ()->SetTime (8000);
            skill->GetVictim ()->SetBarehanded (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (3000);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetVar1 () == 1 ? (zrand (100) < skill->GetT0 () * 34 ? 1 : 0) : 0);
            skill->GetVictim ()->SetBlessed (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetT0 () > 0 ? (skill->GetPlayer ()->GetVar1 () == 1 ? 120 : 0) : 0));
            skill->GetVictim ()->SetTime (2000 + 2000 * skill->GetT0 ());
            skill->GetVictim ()->SetRatio (0.1 * skill->GetLevel ());
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (100);
            skill->GetVictim ()->SetRetort (1);
            return true;
        }
#endif
    };
}
#endif
