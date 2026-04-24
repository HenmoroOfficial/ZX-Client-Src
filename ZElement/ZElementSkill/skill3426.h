#ifndef __CPPGEN_GNET_SKILL3426
#define __CPPGEN_GNET_SKILL3426
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3426:public Skill
    {
      public:
        enum
        { SKILL_ID = 3426 };
          Skill3426 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3426Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (1.05 + 0.02 * skill->GetT1 ());
                skill->SetPlus (88 + 3 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3426Stub ():SkillStub (3426)
        {
            occupation = 65;
            name = L"Ò»ÏßÌì";
            nativename = "Ò»ÏßÌì";
            icon = "Ò»ÏßÌì.dds";
            maxlevel = 20;
            maxlearn = 9;
            type = 1;
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
            talent[0] = 3397;
            talent[1] = 3381;
            talent[2] = 3751;
            talent_size = 3;
            action[0] = "·ÙÏã¹È_µØÁÑÅüÕ¶";
            action[1] = "·ÙÏã¹È_µØÁÑÅüÕ¶";
            action[2] = "·ÙÏã¹È_µØÁÑÅüÕ¶";
            action[3] = "·ÙÏã¹È_µØÁÑÅüÕ¶";
            action[4] = "·ÙÏã¹È_µØÁÑÅüÕ¶";
            action[5] = "·ÙÏã¹È_µØÁÑÅüÕ¶";
            action[6] = "·ÙÏã¹È_µØÁÑÅüÕ¶";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "·ÙÏã¹È_Æï³Ë_µØÁÑÅüÕ¶";
            action[16] = "·ÙÏã¹È_Æï³Ë_µØÁÑÅüÕ¶";
            action[17] = "·ÙÏã¹È_Æï³Ë_µØÁÑÅüÕ¶";
            action[18] = "·ÙÏã¹È_Æï³Ë_µØÁÑÅüÕ¶";
            action[19] = "·ÙÏã¹È_Æï³Ë_µØÁÑÅüÕ¶";
            action[20] = "·ÙÏã¹È_Æï³Ë_µØÁÑÅüÕ¶";
            action[21] = "·ÙÏã¹È_Æï³Ë_µØÁÑÅüÕ¶";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "";
            rangetype = 1;
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
        virtual ~ Skill3426Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3425 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (14);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (270 + 6 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 8000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 60, 62, 64, 66, 68, 70, 72, 74, 76 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (3);
        }
        float GetAttackdistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (11);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (9);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               270 + 6 * skill->GetLevel (),
                               88 + 3 * skill->GetLevel (), 10 + skill->GetLevel (), 2 + 0.5 * skill->GetLevel (), 5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (300));
            skill->GetVictim ()->SetTime ((2 + 0.5 * skill->GetLevel ()) * 1000);
            skill->GetVictim ()->SetRatio (0.05 * skill->GetLevel () + 0.1 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetSlow (1);
            skill->GetVictim ()->SetProbability (1.0 * (20 * skill->GetT1 () + skill->GetT2 ()));
            skill->GetVictim ()->SetTime (8100);
            skill->GetVictim ()->SetRatio (5);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (5);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetVar1 () * 0.15 * (1 + 0.1 * skill->GetT2 ()));
            skill->GetVictim ()->SetFiring (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (10 + skill->GetLevel ());
        }
#endif
    };
}
#endif
