#ifndef __CPPGEN_GNET_SKILL5013
#define __CPPGEN_GNET_SKILL5013
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill5013:public Skill
    {
      public:
        enum
        { SKILL_ID = 5013 };
          Skill5013 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill5013Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetHasmultbuff (14));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill5013Stub ():SkillStub (5013)
        {
            occupation = 145;
            name = L"甘霖·改<水曜>";
            nativename = "甘霖·改<水曜>";
            icon = "枯木逢春改.dds";
            maxlevel = 4;
            maxlearn = 10;
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
            skill_class = -2;
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
            preskillnum = 0;
            action[0] = "0";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "0";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
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
            rangetype = 7;
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
        virtual ~ Skill5013Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetHpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetInkcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 4);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (25 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 60000;
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
                               25 * skill->GetLevel (),
                               5 + 5 * skill->GetLevel (),
                               10 + 10 * skill->GetLevel (), 2 * skill->GetLevel (), 2 + 2 * skill->GetLevel (), 60 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetLevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetHasmultbuff (15) == 1 ? 100 : 0));
            skill->GetVictim ()->SetRatio (15);
            skill->GetVictim ()->SetAmount (15);
            skill->GetVictim ()->SetValue (15);
            skill->GetVictim ()->SetDispel (1);
            skill->GetVictim ()->SetTime (skill->GetPlayer ()->GetHasmultbuff (15) == 1 ? 8000 : 0);
            skill->GetVictim ()->SetRatio (0.02 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetIncmp (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetClearwrap (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () == 1 ? 100 : 0));
            skill->GetVictim ()->SetRatio (14);
            skill->GetVictim ()->SetAmount (14);
            skill->GetVictim ()->SetValue (14);
            skill->GetVictim ()->SetDispel (1);
            skill->GetVictim ()->SetTime (skill->GetPlayer ()->GetVar1 () == 1 ? 25000 : 0);
            skill->GetVictim ()->SetRatio (0.25 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetIncmp (1);
            return true;
        }
#endif
    };
}
#endif
