#ifndef __CPPGEN_GNET_SKILL3156
#define __CPPGEN_GNET_SKILL3156
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3156:public Skill
    {
      public:
        enum
        { SKILL_ID = 3156 };
          Skill3156 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3156Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 50;
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
                return 50;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
                skill->GetPlayer ()->SetVar1 (zrand (1));
                skill->GetPlayer ()->SetVar2 (zrand (1));
                skill->GetPlayer ()->SetVar3 (zrand (1));
                skill->GetPlayer ()->SetVar4 (zrand (1));
            }
        };
#endif
      Skill3156Stub ():SkillStub (3156)
        {
            occupation = 128;
            name = L"仙境取宝之传送";
            nativename = "仙境取宝之传送";
            icon = "";
            maxlevel = 4;
            maxlearn = 4;
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
            action[0] = "";
            action[1] = "";
            action[2] = "";
            action[3] = "";
            action[4] = "";
            action[5] = "";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "";
            action[16] = "";
            action[17] = "";
            action[18] = "";
            action[19] = "";
            action[20] = "";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "";
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
            skill_limit = 7;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill3156Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (5);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 100;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 0;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (4);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (3);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetLevel () == 1 ? 100 : 0));
            skill->GetVictim ()->SetTime (315000);
            skill->GetVictim ()->SetBuffid ((skill->GetLevel () == 1) && (skill->GetPlayer ()->GetVar1 () == 0) ? -133 : -148);
            skill->GetVictim ()->SetAmount ((skill->GetLevel () == 1) && (skill->GetPlayer ()->GetVar1 () == 0) ? 92 : 94);
            skill->GetVictim ()->SetValue ((skill->GetLevel () == 1) && (skill->GetPlayer ()->GetVar1 () == 0) ? 142 : 28);
            skill->GetVictim ()->SetMagicdoor (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetLevel () == 2 ? 100 : 0));
            skill->GetVictim ()->SetTime (315000);
            skill->GetVictim ()->SetBuffid ((skill->GetLevel () == 2) && (skill->GetPlayer ()->GetVar2 () == 0) ? 11 : 61);
            skill->GetVictim ()->SetAmount ((skill->GetLevel () == 2) && (skill->GetPlayer ()->GetVar2 () == 0) ? 134 : 144);
            skill->GetVictim ()->SetValue ((skill->GetLevel () == 2) && (skill->GetPlayer ()->GetVar2 () == 0) ? -17 : -64);
            skill->GetVictim ()->SetMagicdoor (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetLevel () == 3 ? 100 : 0));
            skill->GetVictim ()->SetTime (315000);
            skill->GetVictim ()->SetBuffid ((skill->GetLevel () == 3) && (skill->GetPlayer ()->GetVar3 () == 0) ? 92 : 67);
            skill->GetVictim ()->SetAmount ((skill->GetLevel () == 3) && (skill->GetPlayer ()->GetVar3 () == 0) ? 249 : 257);
            skill->GetVictim ()->SetValue ((skill->GetLevel () == 3) && (skill->GetPlayer ()->GetVar3 () == 0) ? -107 : -52);
            skill->GetVictim ()->SetMagicdoor (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetLevel () == 4 ? 100 : 0));
            skill->GetVictim ()->SetTime (315000);
            skill->GetVictim ()->SetBuffid ((skill->GetLevel () == 4) && (skill->GetPlayer ()->GetVar4 () == 0) ? 128 : 183);
            skill->GetVictim ()->SetAmount ((skill->GetLevel () == 4) && (skill->GetPlayer ()->GetVar4 () == 0) ? 253 : 259);
            skill->GetVictim ()->SetValue ((skill->GetLevel () == 4) && (skill->GetPlayer ()->GetVar4 () == 0) ? -52 : -52);
            skill->GetVictim ()->SetMagicdoor (1);
            return true;
        }
#endif
    };
}
#endif
