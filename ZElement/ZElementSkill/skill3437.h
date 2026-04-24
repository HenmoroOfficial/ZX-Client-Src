#ifndef __CPPGEN_GNET_SKILL3437
#define __CPPGEN_GNET_SKILL3437
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3437:public Skill
    {
      public:
        enum
        { SKILL_ID = 3437 };
          Skill3437 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3437Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3437Stub ():SkillStub (3437)
        {
            occupation = 66;
            name = L"ÌìËøÇôÔ¹";
            nativename = "ÌìËøÇôÔ¹";
            icon = "ÌìËøÇôÔ¹.dds";
            maxlevel = 20;
            maxlearn = 8;
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
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 9;
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
            talent[0] = 3400;
            talent[1] = 3402;
            talent[2] = 3396;
            talent[3] = 3747;
            talent_size = 4;
            action[0] = "·ÙÏã¹È_»ðÑæÇôÀÎ";
            action[1] = "·ÙÏã¹È_»ðÑæÇôÀÎ";
            action[2] = "·ÙÏã¹È_»ðÑæÇôÀÎ";
            action[3] = "·ÙÏã¹È_»ðÑæÇôÀÎ";
            action[4] = "·ÙÏã¹È_»ðÑæÇôÀÎ";
            action[5] = "·ÙÏã¹È_»ðÑæÇôÀÎ";
            action[6] = "·ÙÏã¹È_»ðÑæÇôÀÎ";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "·ÙÏã¹È_Æï³Ë_»ðÑæÇôÀÎ";
            action[16] = "·ÙÏã¹È_Æï³Ë_»ðÑæÇôÀÎ";
            action[17] = "·ÙÏã¹È_Æï³Ë_»ðÑæÇôÀÎ";
            action[18] = "·ÙÏã¹È_Æï³Ë_»ðÑæÇôÀÎ";
            action[19] = "·ÙÏã¹È_Æï³Ë_»ðÑæÇôÀÎ";
            action[20] = "·ÙÏã¹È_Æï³Ë_»ðÑæÇôÀÎ";
            action[21] = "·ÙÏã¹È_Æï³Ë_»ðÑæÇôÀÎ";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "";
            rangetype = 0;
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
        virtual ~ Skill3437Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3436 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (600 + 15 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 80000 * (1 - 0.1 * skill->GetT2 () - 0.03 * skill->GetT3 ());
        }
        int GetEnmity (Skill * skill) const
        {
            return 0;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[8] = { 85, 87, 89, 91, 93, 95, 97, 99 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (18);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (16);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, (600 + 15 * skill->GetLevel ()), 8, (6 + 2 * skill->GetLevel ()));

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetHasbuff (4197) == 1 ? 120 : 0));
            skill->GetVictim ()->SetRatio (4197);
            skill->GetVictim ()->SetAmount (4197);
            skill->GetVictim ()->SetValue (4197);
            skill->GetVictim ()->SetDispel (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetT0 () > 0 ? 300 : 0));
            skill->GetVictim ()->SetTime (1000 * (6 + 2 * skill->GetLevel () + 2 * skill->GetT1 ()));
            skill->GetVictim ()->SetRatio (0.15 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetSlow (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (1000 * (6 + 2 * skill->GetLevel () + 2 * skill->GetT1 ()));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (44678);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetCreateobject (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (80000 * (1 - 0.1 * skill->GetT2 () - 0.03 * skill->GetT3 ()));
            skill->GetVictim ()->SetRatio (3749);
            skill->GetVictim ()->SetAmount (3749);
            skill->GetVictim ()->SetValue (3749);
            skill->GetVictim ()->SetSetcooldown (1);
            return true;
        }
#endif
    };
}
#endif
