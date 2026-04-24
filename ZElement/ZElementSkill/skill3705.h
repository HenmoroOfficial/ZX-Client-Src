#ifndef __CPPGEN_GNET_SKILL3705
#define __CPPGEN_GNET_SKILL3705
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3705:public Skill
    {
      public:
        enum
        { SKILL_ID = 3705 };
          Skill3705 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3705Stub:public SkillStub
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
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (0);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetPlus (295 + 5 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel () * 6);
                skill->SetRatio (1.02 + 0.03 * skill->GetLevel ());
                skill->SetCrit (0.02 + 0.01 * skill->GetLevel ());
                skill->SetCrithurt (0.03 * skill->GetLevel () + 0.05 * skill->GetT1 ());
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
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (0);
                skill->GetPlayer ()->SetVar2 (0);
                skill->SetPlus (295 + 5 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel () * 6);
                skill->SetRatio (1.02 + 0.03 * skill->GetLevel ());
                skill->SetCrit (0.02 + 0.01 * skill->GetLevel ());
                skill->SetCrithurt (0.03 * skill->GetLevel () + 0.05 * skill->GetT1 ());
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
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (1);
                skill->GetPlayer ()->SetVar2 (0);
                skill->SetPlus (295 + 5 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel () * 6);
                skill->SetRatio (1.02 + 0.03 * skill->GetLevel ());
                skill->SetCrit (0.02 + 0.01 * skill->GetLevel ());
                skill->SetCrithurt (0.03 * skill->GetLevel () + 0.05 * skill->GetT1 ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill3705Stub ():SkillStub (3705)
        {
            occupation = 180;
            name = L"ÉúËÀ¼À";
            nativename = "ÉúËÀ¼À";
            icon = "ÉúËÀ¼À.dds";
            maxlevel = 12;
            maxlearn = 0;
            type = 1;
            eventflag = 0;
            spcost = 0;
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
            skill_class = 10;
            guide = false;
            summon_id = 0;
            trigger = true;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 0;
            talent[0] = 3511;
            talent[1] = 3514;
            talent[2] = 3506;
            talent[3] = 3805;
            talent[4] = 3588;
            talent_size = 5;
            action[0] = "Ì«ê»_±»´¥·¢3";
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
            action[11] = "Ì«ê»_±»´¥·¢3";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "Ì«ê»_Æï³Ë_±»´¥·¢3";
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
            action[26] = "Ì«ê»_Æï³Ë_±»´¥·¢3";
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
            weapon_limit = 11;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
            statestub.push_back (new State3 ());
            statestub.push_back (new State4 ());
#endif
        }
        virtual ~ Skill3705Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (375 + 15 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 15000 - 7000 * norm (skill->GetT4 () / 19.99);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               375 + 15 * skill->GetLevel (),
                               295 + 5 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel () * 6,
                               2 + 3 * skill->GetLevel (),
                               2 + skill->GetLevel (), 3 * skill->GetLevel (), skill->GetLevel (), skill->GetLevel (), 2 * skill->GetLevel ());

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
                                                 (skill->GetPlayer ()->GetVar2 () ==
                                                  0 ? 0 : int (skill->GetPlayer ()->GetVar2 () * 0.001 * norm (skill->GetT2 ())) +
                                                  4 * skill->GetT2 ()));
            skill->GetVictim ()->SetTime (5000 * skill->GetT2 ());
            skill->GetVictim ()->SetRatio (0.15 * skill->GetT2 ());
            skill->GetVictim ()->SetValue (0.15 * skill->GetT2 ());
            skill->GetVictim ()->SetParalysis (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar2 () == 0 ? 0 : skill->GetT3 () * 8));
            skill->GetVictim ()->SetTime (5000 * norm (skill->GetT3 ()));
            skill->GetVictim ()->SetRatio (0.2);
            skill->GetVictim ()->SetValue (0.2);
            skill->GetVictim ()->SetRandcurse (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetRatio (skill->GetPlayer ()->GetVar1 () * (norm (zrand (100) / (70 - 5 * skill->GetT0 ())) == 0 ? 0 : 3466));
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetClearcooldown (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (15000);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount (skill->GetLevel () * 10 + skill->GetT1 ());
            skill->GetVictim ()->SetValue (3616);
            skill->GetVictim ()->SetActivebecrit (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (1000 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio (0.01 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetBloodthirsty (1);
            return true;
        }
#endif
    };
}
#endif
