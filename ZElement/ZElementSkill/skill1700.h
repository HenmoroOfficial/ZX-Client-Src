#ifndef __CPPGEN_GNET_SKILL1700
#define __CPPGEN_GNET_SKILL1700
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1700:public Skill
    {
      public:
        enum
        { SKILL_ID = 1700 };
          Skill1700 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1700Stub:public SkillStub
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
                skill->SetRatio (1.10);
                skill->SetCrithurt (1.5);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1700Stub ():SkillStub (1700)
        {
            occupation = 146;
            name = L"ÂÖ»Ø»Ã¾³£º°µ";
            nativename = "ÂÖ»Ø»Ã¾³£º°µ";
            icon = "ÂÖ»Ø»Ã¾³.dds";
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
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 3;
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
            action[0] = "ÌìÒôËÂ_ÂÖ»Ø»Ã¾³";
            action[1] = "ÌìÒôËÂ_ÂÖ»Ø»Ã¾³";
            action[2] = "ÌìÒôËÂ_ÂÖ»Ø»Ã¾³";
            action[3] = "ÌìÒôËÂ_ÂÖ»Ø»Ã¾³";
            action[4] = "ÌìÒôËÂ_ÂÖ»Ø»Ã¾³";
            action[5] = "ÌìÒôËÂ_ÂÖ»Ø»Ã¾³";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_ÂÖ»Ø»Ã¾³";
            action[16] = "ÌìÒôËÂ_Æï³Ë_ÂÖ»Ø»Ã¾³";
            action[17] = "ÌìÒôËÂ_Æï³Ë_ÂÖ»Ø»Ã¾³";
            action[18] = "ÌìÒôËÂ_Æï³Ë_ÂÖ»Ø»Ã¾³";
            action[19] = "ÌìÒôËÂ_Æï³Ë_ÂÖ»Ø»Ã¾³";
            action[20] = "ÌìÒôËÂ_Æï³Ë_ÂÖ»Ø»Ã¾³";
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
        virtual ~ Skill1700Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 10);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (990);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 360000;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 7);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 2);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 990);

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
            skill->GetVictim ()->SetTime (8000 + norm (skill->GetPlayer ()->GetLevel () / 105) * zrand (9) * 1000 + 100);
            skill->GetVictim ()->SetRatio (0.8);
            skill->GetVictim ()->SetValue (0.8);
            skill->GetVictim ()->SetRandcurse (1);
            skill->GetVictim ()->SetProbability (1.0 * (50));
            skill->GetVictim ()->SetTime (8000 + 100);
            skill->GetVictim ()->SetDiet (1);
            return true;
        }
#endif
    };
}
#endif
