#ifndef __CPPGEN_GNET_SKILL4947
#define __CPPGEN_GNET_SKILL4947
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4947:public Skill
    {
      public:
        enum
        { SKILL_ID = 4947 };
          Skill4947 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4947Stub:public SkillStub
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
                skill->SetRatio (1 + 0.03 * skill->GetLevel () + 0.01 * INT (skill->GetPlayer ()->GetLevel () / (11 - skill->GetLevel ())));
                skill->SetPlus (60 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4947Stub ():SkillStub (4947)
        {
            occupation = 130;
            name = L"³Ð¡¤ÐÇÔÉÉß±ä";
            nativename = "³Ð¡¤ÐÇÔÉÉß±ä";
            icon = "ÐÇÔÉÉß±ä.dds";
            maxlevel = 5;
            maxlearn = 5;
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
            action[0] = "»³¹â_·¨±¦_ÆÆ¾ü";
            action[1] = "»³¹â_·¨±¦_ÆÆ¾ü";
            action[2] = "»³¹â_·¨±¦_ÆÆ¾ü";
            action[3] = "»³¹â_·¨±¦_ÆÆ¾ü";
            action[4] = "»³¹â_·¨±¦_ÆÆ¾ü";
            action[5] = "»³¹â_·¨±¦_ÆÆ¾ü";
            action[6] = "»³¹â_·¨±¦_ÆÆ¾ü";
            action[7] = "»³¹â_·¨±¦_ÆÆ¾ü";
            action[8] = "»³¹â_·¨±¦_ÆÆ¾ü";
            action[9] = "»³¹â_·¨±¦_ÆÆ¾ü";
            action[10] = "»³¹â_·¨±¦_ÆÆ¾ü";
            action[11] = "»³¹â_·¨±¦_ÆÆ¾ü";
            action[12] = "»³¹â_·¨±¦_ÆÆ¾ü";
            action[13] = "ÁéÙí_·¨±¦_ÂÞ_Óñ";
            action[14] = "0";
            action[15] = "»³¹â_Æï³Ë_·¨±¦_ÆÆ¾ü";
            action[16] = "»³¹â_Æï³Ë_·¨±¦_ÆÆ¾ü";
            action[17] = "»³¹â_Æï³Ë_·¨±¦_ÆÆ¾ü";
            action[18] = "»³¹â_Æï³Ë_·¨±¦_ÆÆ¾ü";
            action[19] = "»³¹â_Æï³Ë_·¨±¦_ÆÆ¾ü";
            action[20] = "»³¹â_Æï³Ë_·¨±¦_ÆÆ¾ü";
            action[21] = "»³¹â_Æï³Ë_·¨±¦_ÆÆ¾ü";
            action[22] = "»³¹â_Æï³Ë_·¨±¦_ÆÆ¾ü";
            action[23] = "»³¹â_Æï³Ë_·¨±¦_ÆÆ¾ü";
            action[24] = "»³¹â_Æï³Ë_·¨±¦_ÆÆ¾ü";
            action[25] = "»³¹â_Æï³Ë_·¨±¦_ÆÆ¾ü";
            action[26] = "»³¹â_Æï³Ë_·¨±¦_ÆÆ¾ü";
            action[27] = "»³¹â_Æï³Ë_·¨±¦_ÆÆ¾ü";
            action[28] = "ÁéÙí_Æï³Ë_·¨±¦_ÂÞ_Óñ";
            action[29] = "0";
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
        virtual ~ Skill4947Stub ()
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
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05 + 10);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (195 - 15 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6300 - 300 * skill->GetLevel ();
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05 + 5);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               195 - 15 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel (),
                               6.3 - 0.3 * skill->GetLevel (),
                               3 * skill->GetLevel (),
                               60 * skill->GetLevel (),
                               11 - skill->GetLevel (), 10, 345 * skill->GetLevel (), 5 * skill->GetLevel (), 12, 2 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (10000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (skill->GetPlayer ()->GetType () == 0 ? 345 * 2 * skill->GetLevel () : 345 * skill->GetLevel ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (5 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (12000);
            skill->GetVictim ()->SetRatio (0.02 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetDecdodge (1);
            return true;
        }
#endif
    };
}
#endif
