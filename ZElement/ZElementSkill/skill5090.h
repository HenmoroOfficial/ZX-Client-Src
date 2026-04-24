#ifndef __CPPGEN_GNET_SKILL5090
#define __CPPGEN_GNET_SKILL5090
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill5090:public Skill
    {
      public:
        enum
        { SKILL_ID = 5090 };
          Skill5090 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill5090Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 100;
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
                return 2900;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill5090Stub ():SkillStub (5090)
        {
            occupation = 128;
            name = L"Ììô¥£º½ðÉß¿ñÎè";
            nativename = "Ììô¥£º½ðÉß¿ñÎè";
            icon = "Ììô¥£º½ðÉß¿ñÎè.dds";
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
            action[0] = "Ìì»ª_ÁìÓò_ËÕÎäÄÁÑò";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "Ìì»ª_ÁìÓò_ËÕÎäÄÁÑò";
            action[13] = "0";
            action[14] = "0";
            action[15] = "Ìì»ª_ÁìÓò_ËÕÎäÄÁÑò";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "Ìì»ª_ÁìÓò_ËÕÎäÄÁÑò";
            action[28] = "0";
            action[29] = "0";
            action[30] = "¼¼ÄÜ¹¥»÷1";
            rangetype = 14;
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
        virtual ~ Skill5090Stub ()
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
            return (float) (20);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (300 * (1 - 0.04 * 2));
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 5000;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (20);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 5 * 9, INT (9 / 3) + 3);

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
            skill->GetVictim ()->SetClearwrap (1);
            skill->GetVictim ()->SetTime (3000);
            skill->GetVictim ()->SetImmunewrap (1);
            skill->GetVictim ()->SetProbability (1.0 * (120 * norm (2)));
            skill->GetVictim ()->SetTime (3000);
            skill->GetVictim ()->SetRatio (0.05 * 2);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetIncattack (1);
            skill->GetVictim ()->SetTime (3000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (5 * 9);
            skill->GetVictim ()->SetAddanti (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (0));
            skill->GetVictim ()->SetTime (3600000);
            skill->GetVictim ()->SetRatio (28);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (1);
            skill->GetVictim ()->SetAbsulotearea (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (INT (9 / 3) + 3);
        }
#endif
    };
}
#endif
