#ifndef __CPPGEN_GNET_SKILL1102
#define __CPPGEN_GNET_SKILL1102
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1102:public Skill
    {
      public:
        enum
        { SKILL_ID = 1102 };
          Skill1102 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1102Stub:public SkillStub
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1102Stub ():SkillStub (1102)
        {
            occupation = 25;
            name = L"护法鬼语";
            nativename = "护法鬼语";
            icon = "护法鬼语.dds";
            maxlevel = 20;
            maxlearn = 9;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 7;
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
            skill_class = 4;
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
            talent[0] = 1176;
            talent_size = 1;
            action[0] = "鬼道_护法鬼语";
            action[1] = "鬼道_护法鬼语";
            action[2] = "鬼道_护法鬼语";
            action[3] = "鬼道_护法鬼语";
            action[4] = "鬼道_护法鬼语";
            action[5] = "鬼道_护法鬼语";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "鬼道_骑乘_护法鬼语";
            action[16] = "鬼道_骑乘_护法鬼语";
            action[17] = "鬼道_骑乘_护法鬼语";
            action[18] = "鬼道_骑乘_护法鬼语";
            action[19] = "鬼道_骑乘_护法鬼语";
            action[20] = "鬼道_骑乘_护法鬼语";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 5;
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
        virtual ~ Skill1102Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1098 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 90000 - 4500 * skill->GetT0 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 40, 43, 47, 52, 58, 65, 73, 82, 92 };
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
            return _snwprintf (buffer, length, format, 0, 6 + 2 * skill->GetLevel (), 12, 2 * skill->GetLevel (), 90);

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
            skill->GetVictim ()->SetTime (6000 + 2000 * skill->GetLevel () + 100 + 2000 * skill->GetT0 ());
            skill->GetVictim ()->SetImmunesleep (1);
            skill->GetVictim ()->SetTime (6000 + 2000 * skill->GetLevel () + 100 + 2000 * skill->GetT0 ());
            skill->GetVictim ()->SetImmunedizzy (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (24000);
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0.02 * skill->GetLevel () * skill->GetPlayer ()->GetMp ());
            skill->GetVictim ()->SetHpgen (1);
            skill->GetVictim ()->SetProbability (1.0 * (30 * skill->GetT0 ()));
            skill->GetVictim ()->SetClearslow (1);
            return true;
        }
#endif
    };
}
#endif
