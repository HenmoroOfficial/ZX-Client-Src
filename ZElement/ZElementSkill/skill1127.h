#ifndef __CPPGEN_GNET_SKILL1127
#define __CPPGEN_GNET_SKILL1127
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1127:public Skill
    {
      public:
        enum
        { SKILL_ID = 1127 };
          Skill1127 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1127Stub:public SkillStub
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1127Stub ():SkillStub (1127)
        {
            occupation = 28;
            name = L"È¼»ê½Ù»ğ";
            nativename = "È¼»ê½Ù»ğ";
            icon = "È¼»ê½Ù»ğ.dds";
            maxlevel = 20;
            maxlearn = 8;
            type = 3;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
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
            talent[0] = 1161;
            talent[1] = 1165;
            talent_size = 2;
            action[0] = "È¼»ê½Ù»ğ";
            action[1] = "È¼»ê½Ù»ğ";
            action[2] = "È¼»ê½Ù»ğ";
            action[3] = "È¼»ê½Ù»ğ";
            action[4] = "È¼»ê½Ù»ğ";
            action[5] = "È¼»ê½Ù»ğ";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "È¼»ê½Ù»ğ";
            action[16] = "È¼»ê½Ù»ğ";
            action[17] = "È¼»ê½Ù»ğ";
            action[18] = "È¼»ê½Ù»ğ";
            action[19] = "È¼»ê½Ù»ğ";
            action[20] = "È¼»ê½Ù»ğ";
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
            rangetype = 2;
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
        virtual ~ Skill1127Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1124 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (150 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 90000 - 9000 * skill->GetT1 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[8] = { 115, 117, 119, 121, 123, 125, 125, 125 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (5 + 2 * skill->GetLevel ());
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (4 + 2 * skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (3 + 2 * skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (150 * skill->GetLevel ()),
                               4 + 2 * skill->GetLevel (), 600 * skill->GetLevel () + 20 * skill->GetLevel () * skill->GetLevel ());

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
            skill->GetVictim ()->SetAmount ((600 * skill->GetLevel () + 20 * skill->GetLevel () * skill->GetLevel ()) * (1 + 0.2 * skill->GetT0 ()));
            skill->GetVictim ()->SetMpdisperse (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (25);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
