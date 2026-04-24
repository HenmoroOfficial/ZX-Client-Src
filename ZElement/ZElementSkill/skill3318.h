#ifndef __CPPGEN_GNET_SKILL3318
#define __CPPGEN_GNET_SKILL3318
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3318:public Skill
    {
      public:
        enum
        { SKILL_ID = 3318 };
          Skill3318 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3318Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 667;
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
                return 333;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrit ((skill->GetCharging () > 250 ? skill->GetCharging () * 0.0005 * (0.01 * 6) : 0));
                skill->SetRatio (2.02 + 0.02 * 6 + 0.02 * 3 + (skill->GetCharging () > 250 ? skill->GetCharging () * 0.0005 * (0.5 + 0.1 * 6) : 0));
                skill->SetPlus (834);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3318Stub ():SkillStub (3318)
        {
            occupation = 128;
            name = L"ÂÖ»ØÉÁ";
            nativename = "ÂÖ»ØÉÁ";
            icon = "ÂÖ»ØÉÁ.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 3;
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
            action[0] = "ºüÑı_ÂÖ»ØÉÁ";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "ºüÑı_ÂÖ»ØÉÁ";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ºüÑı_Æï³Ë_ÂÖ»ØÉÁ";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "ºüÑı_Æï³Ë_ÂÖ»ØÉÁ";
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
#endif
        }
        virtual ~ Skill3318Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (40);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (866);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 40000 - 5000 * 3;
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
            return (float) (20);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 102 + 2 * 6, 834, 20 + 10 * 6, 50 + 10 * 6);

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (20 + 10 * 6 + 4 * 3));
            skill->GetVictim ()->SetTime (6000 + 2000 * 3 + 1200 * 3);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount ((skill->GetPlayer ()->GetReborncount () + 1) * skill->GetPlayer ()->GetLevel () * 50);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetMaxhp () - skill->GetPlayer ()->GetHp ());
            skill->GetVictim ()->SetSubhp (1);
            skill->GetVictim ()->SetProbability (1.0 * (20 + 10 * 6 + 4 * 3));
            skill->GetVictim ()->SetTime (6000 + 2000 * 3 + 1200 * 3);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount ((skill->GetPlayer ()->GetReborncount () + 1) * skill->GetPlayer ()->GetLevel () * 50);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetMaxmp () - skill->GetPlayer ()->GetMp ());
            skill->GetVictim ()->SetSubmp (1);
            return true;
        }
#endif
    };
}
#endif
