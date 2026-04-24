#ifndef __CPPGEN_GNET_SKILL1880
#define __CPPGEN_GNET_SKILL1880
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1880:public Skill
    {
      public:
        enum
        { SKILL_ID = 1880 };
          Skill1880 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1880Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 350;
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (1);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetLevel ());
                skill->SetRatio (1.02 + 0.02 * (skill->GetLevel () + skill->GetT0 ()) + 0.01 * skill->GetT1 ());
                skill->SetPlus (skill->GetT1 () > 0 ? (133 + 10 * skill->GetT1 ()) : 0);
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
                return 225;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (2);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetLevel ());
                skill->SetRatio (1.02 + 0.02 * (skill->GetLevel () + skill->GetT0 ()) + 0.01 * skill->GetT2 ());
                skill->SetPlus (skill->GetT2 () > 0 ? (324 + 21 * skill->GetT2 ()) : 0);
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
                return 500;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (3);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetLevel ());
                skill->SetRatio (1.02 + 0.02 * (skill->GetLevel () + skill->GetT0 ()) + 0.01 * skill->GetT3 ());
                skill->SetPlus (skill->GetT3 () > 0 ? (1043 + 33 * skill->GetT3 ()) : 0);
                skill->GetPlayer ()->SetPerform (0);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State5:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 525;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (4);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetLevel ());
                skill->SetRatio (1.02 + 0.02 * (skill->GetLevel () + skill->GetT0 ()) + 0.01 * skill->GetT4 ());
                skill->SetPlus (skill->GetT4 () > 0 ? (1543 + 32 * skill->GetT4 ()) : 0);
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill1880Stub ():SkillStub (1880)
        {
            occupation = 37;
            name = L"°ËëÅ";
            nativename = "°ËëÅ";
            icon = "°ËëÅ.dds";
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
            talent[0] = 2084;
            talent[1] = 1847;
            talent[2] = 1856;
            talent[3] = 1865;
            talent[4] = 1874;
            talent_size = 5;
            action[0] = "ÊÞÉñ_°ËëÅ";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "ÊÞÉñ_°ËëÅ";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ÊÞÉñ_Æï³Ë_°ËëÅ";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "ÊÞÉñ_Æï³Ë_°ËëÅ";
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
            statestub.push_back (new State5 ());
#endif
        }
        virtual ~ Skill1880Stub ()
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
            return (float) (15);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (458 + 8 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 10000 - 400 - 1000 * skill->GetT0 () - 1000 * int (skill->GetT1 () / 8.99) - 1000 * int (skill->GetT2 () / 8.99) -
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
            return (float) (7);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (3);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 458 + 8 * skill->GetLevel (), 2 + 2 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () == 1 ? (skill->GetT1 () > 0 ? 600 : 0) : 0));
            skill->GetVictim ()->SetTime (4000);
            skill->GetVictim ()->SetRatio (0.15 + 0.05 * skill->GetT1 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetSlow (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetVar1 () ==
                                                  2 ? (skill->GetT2 () >
                                                       0 ? (skill->GetPlayer ()->GetVar2 () +
                                                            skill->GetPlayer ()->GetVar2 () * skill->GetT2 () / 9) : 0) : 0));
            skill->GetVictim ()->SetTime (4000);
            skill->GetVictim ()->SetRatio (0.2);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetWeak (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetVar1 () ==
                                                  3 ? (skill->GetT3 () >
                                                       0 ? (skill->GetPlayer ()->GetVar2 () +
                                                            skill->GetPlayer ()->GetVar2 () * skill->GetT3 () / 9) : 0) : 0));
            skill->GetVictim ()->SetTime (4000);
            skill->GetVictim ()->SetWrap (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetVar1 () ==
                                                  4 ? (skill->GetT4 () >
                                                       0 ? (skill->GetPlayer ()->GetVar2 () +
                                                            skill->GetPlayer ()->GetVar2 () * skill->GetT4 () / 9) : 0) : 0));
            skill->GetVictim ()->SetTime (4000);
            skill->GetVictim ()->SetDizzy (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () > 1 ? 10 * skill->GetT0 () : 0));
            skill->GetVictim ()->SetAmount (1);
            skill->GetVictim ()->SetCleardebuff (1);
            return true;
        }
#endif
    };
}
#endif
