#ifndef __CPPGEN_GNET_SKILL3703
#define __CPPGEN_GNET_SKILL3703
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3703:public Skill
    {
      public:
        enum
        { SKILL_ID = 3703 };
          Skill3703 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3703Stub:public SkillStub
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
                skill->SetPlus (320 + skill->GetLevel () * skill->GetLevel () * 6);
                skill->SetCrithurt (0.02 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3703Stub ():SkillStub (3703)
        {
            occupation = 180;
            name = L"Áú»¢¶·";
            nativename = "Áú»¢¶·";
            icon = "Áú»¢¶·.dds";
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
            talent[0] = 3513;
            talent[1] = 3506;
            talent[2] = 3511;
            talent[3] = 3588;
            talent_size = 4;
            action[0] = "Ì«ê»_ËéÐÇ»÷";
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
            action[11] = "Ì«ê»_ËéÐÇ»÷";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "Ì«ê»_Æï³Ë_ËéÐÇ»÷";
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
            action[26] = "Ì«ê»_Æï³Ë_ËéÐÇ»÷";
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
#endif
        }
        virtual ~ Skill3703Stub ()
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
            return (float) (324 + 15 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 15000 - 7000 * norm (skill->GetT3 () / 19.99);
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
                               324 + 15 * skill->GetLevel (),
                               80 + 5 * skill->GetLevel (), 2 * skill->GetLevel (), 3 + 2 * skill->GetLevel (), 3 + skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (30 * skill->GetT0 ()));
            skill->GetVictim ()->SetAmount (skill->GetT0 ());
            skill->GetVictim ()->SetClearbuff (1);
            skill->GetVictim ()->SetProbability (1.0 * (0));
            skill->GetVictim ()->SetTime (skill->GetLevel () * 1000);
            skill->GetVictim ()->SetRatio (4350);
            skill->GetVictim ()->SetBuffid (5);
            skill->GetVictim ()->SetAmount (skill->GetLevel ());
            skill->GetVictim ()->SetValue (3603);
            skill->GetVictim ()->SetActiveonfilteradd (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetVar1 () ==
                                                  0 ? 0 : int (skill->GetPlayer ()->GetVar1 () * 0.001 * norm (skill->GetT1 ())) +
                                                  4 * skill->GetT1 ()));
            skill->GetVictim ()->SetTime (5000 * skill->GetT1 ());
            skill->GetVictim ()->SetRatio (0.15 * skill->GetT1 ());
            skill->GetVictim ()->SetValue (0.15 * skill->GetT1 ());
            skill->GetVictim ()->SetParalysis (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetRatio (norm (zrand (100) / (80 - 5 * skill->GetT2 ())) == 0 ? 0 : 3466);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetClearcooldown (1);
            return true;
        }
#endif
    };
}
#endif
