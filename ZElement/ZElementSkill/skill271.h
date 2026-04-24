#ifndef __CPPGEN_GNET_SKILL271
#define __CPPGEN_GNET_SKILL271
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill271:public Skill
    {
      public:
        enum
        { SKILL_ID = 271 };
          Skill271 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill271Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes5 ());
                skill->SetRatio (1 + 0.07 * skill->GetT3 ());
                skill->SetPlus (201 + 15 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill271Stub ():SkillStub (271)
        {
            occupation = 10;
            name = L"诵经";
            nativename = "诵经";
            icon = "诵经.dds";
            maxlevel = 20;
            maxlearn = 9;
            type = 1;
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
            skill_class = 3;
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
            talent[0] = 671;
            talent[1] = 690;
            talent[2] = 698;
            talent[3] = 702;
            talent_size = 4;
            action[0] = "天音寺_观音经";
            action[1] = "天音寺_观音经";
            action[2] = "天音寺_观音经";
            action[3] = "天音寺_观音经";
            action[4] = "天音寺_观音经";
            action[5] = "天音寺_观音经";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "天音寺_骑乘_观音经";
            action[16] = "天音寺_骑乘_观音经";
            action[17] = "天音寺_骑乘_观音经";
            action[18] = "天音寺_骑乘_观音经";
            action[19] = "天音寺_骑乘_观音经";
            action[20] = "天音寺_骑乘_观音经";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "技能攻击1";
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
        virtual ~ Skill271Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 276 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 5 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (9);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (20);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 9000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 30, 32, 34, 36, 38, 40, 42, 44, 46 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (8);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (5);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               20,
                               201 + 15 * skill->GetLevel (),
                               INT (37 + 3.7 * skill->GetLevel ()), 1.9 + 0.6 * skill->GetLevel (), 144 + 48 * skill->GetLevel (), 5);

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
                                                 (37 + 3.7 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetT1 () +
                                                  skill->GetPlayer ()->GetVar1 () * skill->GetT1 () * 0.2));
            skill->GetVictim ()->SetTime (1900 + 600 * skill->GetLevel () + 100 + 400 * skill->GetLevel () * skill->GetT0 ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetSleep (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (8000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (144 + 48 * skill->GetLevel ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (6 * skill->GetT2 ()));
            skill->GetVictim ()->SetTime (4050 * skill->GetT2 ());
            skill->GetVictim ()->SetRatio (0.03 * skill->GetT2 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecdefence (1);
            return true;
        }
#endif
    };
}
#endif
