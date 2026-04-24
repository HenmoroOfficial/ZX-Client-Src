#ifndef __CPPGEN_GNET_SKILL2446
#define __CPPGEN_GNET_SKILL2446
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2446:public Skill
    {
      public:
        enum
        { SKILL_ID = 2446 };
          Skill2446 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2446Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 0;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2446Stub ():SkillStub (2446)
        {
            occupation = 48;
            name = L"Ó°¶İ";
            nativename = "Ó°¶İ";
            icon = "Ó°¶İ.dds";
            maxlevel = 10;
            maxlearn = 5;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 1;
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
            skill_class = 7;
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
            talent[0] = 2548;
            talent[1] = 2550;
            talent[2] = 2551;
            talent[3] = 2447;
            talent[4] = 2565;
            talent_size = 5;
            action[0] = "0";
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
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "0";
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
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 5;
            doenchant = true;
            dobless = true;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 1792;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
#endif
        }
        virtual ~ Skill2446Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 2445 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 4 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (625);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 0;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 95000 - 5000 * skill->GetLevel () - 5000 * skill->GetT1 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[5] = { 125, 125, 125, 125, 125 };
            return array[level - 1];
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
            return _snwprintf (buffer, length, format, 1 - 0.2 * skill->GetLevel (), 95 - 5 * skill->GetLevel (), skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetRatio (3);
            skill->GetVictim ()->SetUnsummonclone (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (20000);
            skill->GetVictim ()->SetRatio (0.2 * skill->GetLevel () - 1);
            skill->GetVictim ()->SetAmount (skill->GetLevel () + skill->GetT2 ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetShadowhide (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (95000 - 5000 * skill->GetLevel () - 5000 * skill->GetT1 () - 5000 * skill->GetT0 ());
            skill->GetVictim ()->SetRatio (2428);
            skill->GetVictim ()->SetAmount (2437);
            skill->GetVictim ()->SetValue (2447);
            skill->GetVictim ()->SetSetcooldown (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetT3 () > 0 ? 120 : 0));
            skill->GetVictim ()->SetAmount (skill->GetT3 ());
            skill->GetVictim ()->SetCleardebuff (1);
            skill->GetVictim ()->SetTime (skill->GetT4 () > 0 ? 6000 : 100);
            skill->GetVictim ()->SetValue (skill->GetT4 () > 0 ? skill->GetT4 () : 0);
            skill->GetVictim ()->SetBlessed (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (skill->GetT3 () > 0 ? 10000 : 100);
            skill->GetVictim ()->SetRatio (0.15 * skill->GetT3 ());
            skill->GetVictim ()->SetNingjin (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetRatio (4141);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetDispel (1);
            return true;
        }
#endif
    };
}
#endif
