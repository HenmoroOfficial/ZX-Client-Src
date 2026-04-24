#ifndef __CPPGEN_GNET_SKILL1109
#define __CPPGEN_GNET_SKILL1109
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1109:public Skill
    {
      public:
        enum
        { SKILL_ID = 1109 };
          Skill1109 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1109Stub:public SkillStub
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
      Skill1109Stub ():SkillStub (1109)
        {
            occupation = 26;
            name = L"Ô¹Äîî¿÷ã";
            nativename = "Ô¹Äîî¿÷ã";
            icon = "Ô¹Äîî¿÷ã.dds";
            maxlevel = 12;
            maxlearn = 8;
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
            talent[0] = 1161;
            talent[1] = 1165;
            talent_size = 2;
            action[0] = "Ô¹Äîî¿÷ã";
            action[1] = "Ô¹Äîî¿÷ã";
            action[2] = "Ô¹Äîî¿÷ã";
            action[3] = "Ô¹Äîî¿÷ã";
            action[4] = "Ô¹Äîî¿÷ã";
            action[5] = "Ô¹Äîî¿÷ã";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "0";
            action[14] = "0";
            action[15] = "Ô¹Äîî¿÷ã";
            action[16] = "Ô¹Äîî¿÷ã";
            action[17] = "Ô¹Äîî¿÷ã";
            action[18] = "Ô¹Äîî¿÷ã";
            action[19] = "Ô¹Äîî¿÷ã";
            action[20] = "Ô¹Äîî¿÷ã";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "0";
            action[29] = "0";
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
        virtual ~ Skill1109Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1105 };
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
            return 180000 - 18000 * skill->GetT1 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[8] = { 55, 58, 61, 64, 68, 72, 76, 81 };
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
            return _snwprintf (buffer, length, format, 0, 12 + 3 * skill->GetLevel (), 5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (12000 + 3000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetPowerless (1);
            skill->GetVictim ()->SetTime (12000 + 3000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetRatio (0.05 * skill->GetLevel () + 0.05 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetIncskilldamage (1);
            skill->GetVictim ()->SetTime (4000 + 100);
            skill->GetVictim ()->SetRatio (0.05);
            skill->GetVictim ()->SetInccritrate (1);
            skill->GetVictim ()->SetTime (12000 + 3000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetImmunesilent (1);
            return true;
        }
#endif
    };
}
#endif
