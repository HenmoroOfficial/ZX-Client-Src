#ifndef __CPPGEN_GNET_SKILL1798
#define __CPPGEN_GNET_SKILL1798
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1798:public Skill
    {
      public:
        enum
        { SKILL_ID = 1798 };
          Skill1798 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1798Stub:public SkillStub
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
      Skill1798Stub ():SkillStub (1798)
        {
            occupation = 129;
            name = L"ÌØ¼¼£ºÁğÁ§ÕÈ";
            nativename = "ÌØ¼¼£ºÁğÁ§ÕÈ";
            icon = "ÁğÁ§ÕÈ.dds";
            maxlevel = 1;
            maxlearn = 1;
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
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
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
            action[0] = "ÌìÒôËÂ_Í¬¹é¼«ÀÖ";
            action[1] = "ÌìÒôËÂ_Í¬¹é¼«ÀÖ";
            action[2] = "ÌìÒôËÂ_Í¬¹é¼«ÀÖ";
            action[3] = "ÌìÒôËÂ_Í¬¹é¼«ÀÖ";
            action[4] = "ÌìÒôËÂ_Í¬¹é¼«ÀÖ";
            action[5] = "ÌìÒôËÂ_Í¬¹é¼«ÀÖ";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_Í¬¹é¼«ÀÖ";
            action[16] = "ÌìÒôËÂ_Æï³Ë_Í¬¹é¼«ÀÖ";
            action[17] = "ÌìÒôËÂ_Æï³Ë_Í¬¹é¼«ÀÖ";
            action[18] = "ÌìÒôËÂ_Æï³Ë_Í¬¹é¼«ÀÖ";
            action[19] = "ÌìÒôËÂ_Æï³Ë_Í¬¹é¼«ÀÖ";
            action[20] = "ÌìÒôËÂ_Æï³Ë_Í¬¹é¼«ÀÖ";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "ÆÕÍ¨¹¥»÷1";
            rangetype = 3;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 10;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill1798Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (30);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 10000;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (25);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (26);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (22);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 1200);

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 1);

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (100);
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetMaxhp () * 0.3 + 1200);
            skill->GetVictim ()->SetBleeding (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (40);
        }
#endif
    };
}
#endif
