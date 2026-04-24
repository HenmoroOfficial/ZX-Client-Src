#ifndef __CPPGEN_GNET_SKILL5079
#define __CPPGEN_GNET_SKILL5079
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill5079:public Skill
    {
      public:
        enum
        { SKILL_ID = 5079 };
          Skill5079 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill5079Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1000;
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
                return 500;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrithurt ((skill->GetCharging () > 250 ? skill->GetCharging () * 0.003 * (0.01 * 9) : 0));
                skill->SetCrit (0.05 + (skill->GetCharging () > 250 ? skill->GetCharging () * 0.0005 * (0.01 * 9) : 0));
                skill->GetPlayer ()->SetVar1 (1);
                skill->SetRatio (1 + 0.01 * 9 + 0.01 * 2 + (skill->GetCharging () > 250 ? skill->GetCharging () * 0.0005 * (0.5 + 0.05 * 9) : 0));
                skill->SetPlus (472 + 19 * 9);
                skill->GetPlayer ()->SetPerform (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State3:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 500;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrithurt ((skill->GetCharging () > 250 ? skill->GetCharging () * 0.003 * (0.01 * 9) : 0));
                skill->SetCrit (0.05 + (skill->GetCharging () > 250 ? skill->GetCharging () * 0.0005 * (0.01 * 9) : 0));
                skill->GetPlayer ()->SetVar1 (2);
                skill->SetRatio (1 + 0.01 * 9 + 0.01 * 2 + (skill->GetCharging () > 250 ? skill->GetCharging () * 0.0005 * (0.5 + 0.05 * 9) : 0));
                skill->SetPlus (472 + 19 * 9);
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill5079Stub ():SkillStub (5079)
        {
            occupation = 128;
            name = L"Ë«·É»ğ";
            nativename = "Ë«·É»ğ";
            icon = "Ë«·É»ğ.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
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
            skill_class = -2;
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
            action[0] = "ºüÑı_Ë«·É»ğ";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "ºüÑı_Ë«·É»ğ";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "0";
            action[14] = "0";
            action[15] = "ºüÑı_Æï³Ë_Ë«·É»ğ";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "ºüÑı_Æï³Ë_Ë«·É»ğ";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "0";
            action[29] = "0";
            action[30] = "¼¼ÄÜ¹¥»÷1";
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
            statestub.push_back (new State3 ());
#endif
        }
        virtual ~ Skill5079Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetHpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetInkcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (40);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (687);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (30);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (25);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 9, 472 + 19 * 9, 6, 1416 + 57 * 9, 50 + 5 * 9);

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () == 1 ? 120 : 0));
            skill->GetVictim ()->SetTime (6000 + 2000 * 2);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount ((472 + 19 * 9) * (3 + 2));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () == 2 ? 120 : 0));
            skill->GetVictim ()->SetTime (6000 + 2000 * 2);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount ((472 + 19 * 9) * (3 + 2));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetMpleak (1);
            return true;
        }
#endif
    };
}
#endif
