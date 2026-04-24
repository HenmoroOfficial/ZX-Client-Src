#ifndef __CPPGEN_GNET_SKILL2234
#define __CPPGEN_GNET_SKILL2234
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2234:public Skill
    {
      public:
        enum
        { SKILL_ID = 2234 };
          Skill2234 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2234Stub:public SkillStub
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
                return 2000;
            }
            void Calculate (Skill * skill) const
            {
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
                return 50;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrithurt ((skill->GetCharging () > 250 ? skill->GetCharging () * 0.0005 * (0.06 * skill->GetLevel ()) : 0));
                skill->GetPlayer ()->SetVar3 ((skill->GetCharging () > 250 ? skill->GetCharging () : 0));
                skill->SetCrit ((skill->GetCharging () > 250 ? skill->GetCharging () * 0.0005 : 0) * (0.01 * skill->GetLevel ()));
                skill->GetPlayer ()->SetVar1 (1);
                skill->SetRatio (1 + 0.02 * skill->GetLevel ());
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetHp () * 0.05 * skill->GetLevel ());
                skill->SetPlus (skill->GetPlayer ()->GetHp () * 0.05 * skill->GetLevel ());
                skill->GetPlayer ()->SetHp (skill->GetPlayer ()->GetHp () * 0.5);
                skill->GetPlayer ()->SetPerform (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State4:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 950;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrithurt ((skill->GetCharging () > 250 ? skill->GetCharging () * 0.0005 * (0.06 * skill->GetLevel ()) : 0));
                skill->GetPlayer ()->SetVar3 ((skill->GetCharging () > 250 ? skill->GetCharging () : 0));
                skill->SetCrit ((skill->GetCharging () > 250 ? skill->GetCharging () * 0.0005 : 0) * (0.01 * skill->GetLevel ()));
                skill->GetPlayer ()->SetVar1 (2);
                skill->SetRatio (1 + 0.02 * skill->GetLevel ());
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetMp () * 0.05 * skill->GetLevel ());
                skill->SetPlus (skill->GetPlayer ()->GetMp () * 0.05 * skill->GetLevel ());
                skill->GetPlayer ()->SetMp (skill->GetPlayer ()->GetMp () * 0.5);
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill2234Stub ():SkillStub (2234)
        {
            occupation = 149;
            name = L"Ë«·É»ğII";
            nativename = "Ë«·É»ğII";
            icon = "Ë«·É»ğII.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 3;
            targettype = 0;
            autoattack = 1;
            allowform = 0;
            credittype = 10;
            clearmask = 130;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 6;
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
            action[13] = "";
            action[14] = "";
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
            weapon_limit = 8;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
            statestub.push_back (new State3 ());
            statestub.push_back (new State4 ());
#endif
        }
        virtual ~ Skill2234Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1910 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 9 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (40);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 4000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 60000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 125, 125, 125, 125, 125, 127, 129, 131, 133, 135 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (30);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (25);
        }
        int GetCreditCost (int level) const
        {
            static int array[10] = { 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 38000, 45000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 2 * skill->GetLevel (), 10 * skill->GetLevel (), 10 * skill->GetLevel (), 10, 20);

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
            skill->GetVictim ()->SetTime (10000 + int (skill->GetPlayer ()->GetVar3 () * 0.0005 * 10) * 2000);
              skill->GetVictim ()->SetBuffid (1);
              skill->GetVictim ()->SetAmount (skill->GetPlayer ()->GetVar2 () * (5 + int (skill->GetPlayer ()->GetVar3 () * 0.0005 * 10)));
              skill->GetVictim ()->SetValue (0);
              skill->GetVictim ()->SetHpleak (1);
              skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () == 2 ? 120 : 0));
              skill->GetVictim ()->SetTime (10000 + int (skill->GetPlayer ()->GetVar3 () * 0.0005 * 10) * 2000);
              skill->GetVictim ()->SetBuffid (1);
              skill->GetVictim ()->SetAmount (skill->GetPlayer ()->GetVar2 () * (5 + int (skill->GetPlayer ()->GetVar3 () * 0.0005 * 10)));
              skill->GetVictim ()->SetValue (0);
              skill->GetVictim ()->SetMpleak (1);
              return true;
        }
#endif
    };
}
#endif
