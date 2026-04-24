#ifndef __CPPGEN_GNET_SKILL1104
#define __CPPGEN_GNET_SKILL1104
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1104:public Skill
    {
      public:
        enum
        { SKILL_ID = 1104 };
          Skill1104 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1104Stub:public SkillStub
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
      Skill1104Stub ():SkillStub (1104)
        {
            occupation = 26;
            name = L"Óù¿ÕÊõ";
            nativename = "Óù¿ÕÊõ";
            icon = "Óù¿ÕÊõ.dds";
            maxlevel = 4;
            maxlearn = 3;
            type = 4;
            eventflag = 0;
            spcost = 1;
            timetype = 1;
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
            talent[0] = 1177;
            talent_size = 1;
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
#endif
        }
        virtual ~ Skill1104Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1103 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (240 - 40 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 30000 - 5000 * skill->GetLevel () - 2000 * skill->GetT0 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[3] = { 46, 50, 76 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (5 + 2.5 * skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 240 - 40 * skill->GetLevel (), 5, 30 - 5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (5000);
            skill->GetVictim ()->SetValue (15);
            skill->GetVictim ()->SetDodge (1);
            skill->GetVictim ()->SetRatio (1093 + 39 * norm (zrand (100) / (95 - 15 * skill->GetT0 ())));
            skill->GetVictim ()->SetAmount (1093 + 12 * norm (zrand (100) / (95 - 15 * skill->GetT0 ())));
            skill->GetVictim ()->SetValue (1093 + 16 * norm (zrand (100) / (95 - 15 * skill->GetT0 ())));
            skill->GetVictim ()->SetClearcooldown (1);
            skill->GetVictim ()->SetRatio (1093 + 21 * norm (zrand (100) / (95 - 15 * skill->GetT0 ())));
            skill->GetVictim ()->SetAmount (1093 + norm (zrand (100) / (95 - 15 * skill->GetT0 ())));
            skill->GetVictim ()->SetValue (1093 + 25 * norm (zrand (100) / (95 - 15 * skill->GetT0 ())));
            skill->GetVictim ()->SetClearcooldown (1);
            skill->GetVictim ()->SetRatio (1093 + 28 * norm (zrand (100) / (95 - 15 * skill->GetT0 ())));
            skill->GetVictim ()->SetAmount (1093 + 30 * norm (zrand (100) / (95 - 15 * skill->GetT0 ())));
            skill->GetVictim ()->SetValue (1093 + 31 * norm (zrand (100) / (95 - 15 * skill->GetT0 ())));
            skill->GetVictim ()->SetClearcooldown (1);
            return true;
        }
#endif
    };
}
#endif
