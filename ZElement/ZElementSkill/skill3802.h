#ifndef __CPPGEN_GNET_SKILL3802
#define __CPPGEN_GNET_SKILL3802
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3802:public Skill
    {
      public:
        enum
        { SKILL_ID = 3802 };
          Skill3802 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3802Stub:public SkillStub
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
      Skill3802Stub ():SkillStub (3802)
        {
            occupation = 146;
            name = L"É³îø£º·Ù";
            nativename = "É³îø£º·Ù";
            icon = "É³îø.dds";
            maxlevel = 1;
            maxlearn = 1;
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
            preskillnum = 0;
            action[0] = "¹íµÀ_É³îø";
            action[1] = "¹íµÀ_É³îø";
            action[2] = "¹íµÀ_É³îø";
            action[3] = "¹íµÀ_É³îø";
            action[4] = "¹íµÀ_É³îø";
            action[5] = "¹íµÀ_É³îø";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íµÀ_Æï³Ë_É³îø";
            action[16] = "¹íµÀ_Æï³Ë_É³îø";
            action[17] = "¹íµÀ_Æï³Ë_É³îø";
            action[18] = "¹íµÀ_Æï³Ë_É³îø";
            action[19] = "¹íµÀ_Æï³Ë_É³îø";
            action[20] = "¹íµÀ_Æï³Ë_É³îø";
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
        virtual ~ Skill3802Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (900);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 360000;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (25);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 900);

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
            skill->GetVictim ()->SetTime (30000 + 100);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetAmount (25);
            skill->GetVictim ()->SetValue (2 * skill->GetPlayer ()->GetLevel ());
            skill->GetVictim ()->SetBloodshield (1);
            skill->GetVictim ()->SetTime (12000 + 100);
            skill->GetVictim ()->SetImmunedizzy (1);
            skill->GetVictim ()->SetRatio (3801);
            skill->GetVictim ()->SetAmount (3803);
            skill->GetVictim ()->SetValue (3804);
            skill->GetVictim ()->SetClearcooldown (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (60000);
            skill->GetVictim ()->SetRatio (0.15);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetIncskillaccu (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (25);
        }
#endif
    };
}
#endif
