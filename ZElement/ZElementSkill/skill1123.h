#ifndef __CPPGEN_GNET_SKILL1123
#define __CPPGEN_GNET_SKILL1123
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1123:public Skill
    {
      public:
        enum
        { SKILL_ID = 1123 };
          Skill1123 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1123Stub:public SkillStub
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
      Skill1123Stub ():SkillStub (1123)
        {
            occupation = 28;
            name = L"Óü»ðÁ¶Ö¾";
            nativename = "Óü»ðÁ¶Ö¾";
            icon = "Óü»ðÁ¶Ö¾.dds";
            maxlevel = 20;
            maxlearn = 9;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 4;
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
            talent[0] = 1184;
            talent[1] = 1187;
            talent_size = 2;
            action[0] = "Óü»ðÁ¶Ö¾";
            action[1] = "Óü»ðÁ¶Ö¾";
            action[2] = "Óü»ðÁ¶Ö¾";
            action[3] = "Óü»ðÁ¶Ö¾";
            action[4] = "Óü»ðÁ¶Ö¾";
            action[5] = "Óü»ðÁ¶Ö¾";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "Óü»ðÁ¶Ö¾";
            action[16] = "Óü»ðÁ¶Ö¾";
            action[17] = "Óü»ðÁ¶Ö¾";
            action[18] = "Óü»ðÁ¶Ö¾";
            action[19] = "Óü»ðÁ¶Ö¾";
            action[20] = "Óü»ðÁ¶Ö¾";
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
        virtual ~ Skill1123Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1122 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
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
            return 180000 - 18000 * skill->GetT0 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 106, 109, 112, 115, 118, 121, 124, 125, 125 };
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
            return _snwprintf (buffer, length, format, 0, 12 + 3 * skill->GetLevel (), 3 * skill->GetLevel (), 15 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (100 - 15 * skill->GetT1 ()));
            skill->GetVictim ()->SetTime (12000 + 3000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (100 * skill->GetLevel () * (1 - 0.1 * skill->GetT1 ()));
            skill->GetVictim ()->SetCycsubdefence (1);
            skill->GetVictim ()->SetProbability (1.0 * (100 - 15 * skill->GetT1 ()));
            skill->GetVictim ()->SetTime (12000 + 3000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0.15 * skill->GetPlayer ()->GetMaxhp () * skill->GetLevel () * (1 - 0.1 * skill->GetT1 ()));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (12000 + 3000 * skill->GetLevel () + 3000 * skill->GetT0 () + 100);
            skill->GetVictim ()->SetRatio (0.03 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (4);
            skill->GetVictim ()->SetIncattack (1);
            return true;
        }
#endif
    };
}
#endif
