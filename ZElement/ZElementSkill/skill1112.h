#ifndef __CPPGEN_GNET_SKILL1112
#define __CPPGEN_GNET_SKILL1112
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1112:public Skill
    {
      public:
        enum
        { SKILL_ID = 1112 };
          Skill1112 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1112Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1600;
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.05 * skill->GetT3 ());
                skill->SetPlus (0.02 * (skill->GetPlayer ()->GetHp () + skill->GetPlayer ()->GetMp ()) +
                                (5.5 * skill->GetLevel () * skill->GetLevel () + 14 * skill->GetLevel () + 621));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1112Stub ():SkillStub (1112)
        {
            occupation = 26;
            name = L"ÆÆÈĞĞ°»ê";
            nativename = "ÆÆÈĞĞ°»ê";
            icon = "ÆÆÈĞĞ°»ê.dds";
            maxlevel = 20;
            maxlearn = 10;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 2;
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
            talent[0] = 1163;
            talent[1] = 1167;
            talent[2] = 1172;
            talent_size = 3;
            action[0] = "ÆÆÈĞĞ°»ê";
            action[1] = "ÆÆÈĞĞ°»ê";
            action[2] = "ÆÆÈĞĞ°»ê";
            action[3] = "ÆÆÈĞĞ°»ê";
            action[4] = "ÆÆÈĞĞ°»ê";
            action[5] = "ÆÆÈĞĞ°»ê";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÆÆÈĞĞ°»ê";
            action[16] = "ÆÆÈĞĞ°»ê";
            action[17] = "ÆÆÈĞĞ°»ê";
            action[18] = "ÆÆÈĞĞ°»ê";
            action[19] = "ÆÆÈĞĞ°»ê";
            action[20] = "ÆÆÈĞĞ°»ê";
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
        virtual ~ Skill1112Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1109 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 4 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1.2 * skill->GetLevel () * skill->GetLevel () - 5.3 * skill->GetLevel () + 97);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 5000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 65, 67, 69, 71, 73, 75, 77, 79, 82, 86 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (7);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (1.2 * skill->GetLevel () * skill->GetLevel () - 5.3 * skill->GetLevel () + 97),
                               INT (5.5 * skill->GetLevel () * skill->GetLevel () + 14 * skill->GetLevel () + 621),
                               3 * skill->GetLevel (), 2 + 0.5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (3 * skill->GetLevel () + skill->GetLevel () * skill->GetT1 ()));
            skill->GetVictim ()->SetTime (2000 + 100 + 500 * skill->GetLevel () + 2000 * skill->GetT0 ());
            skill->GetVictim ()->SetBarehanded (1);
            return true;
        }
#endif
    };
}
#endif
