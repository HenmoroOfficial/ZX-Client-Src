#ifndef __CPPGEN_GNET_SKILL1494
#define __CPPGEN_GNET_SKILL1494
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1494:public Skill
    {
      public:
        enum
        { SKILL_ID = 1494 };
          Skill1494 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1494Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 0;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1.2);
                skill->SetPlus (2 * skill->GetPlayer ()->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1494Stub ():SkillStub (1494)
        {
            occupation = 128;
            name = L"亮银一闪：欢跃";
            nativename = "亮银一闪：欢跃";
            icon = "亮银之证.dds";
            maxlevel = 1;
            maxlearn = 10;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 1;
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
            action[0] = "0";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "0";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
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
#endif
        }
        virtual ~ Skill1494Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (400);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 10;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 720000;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (15);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (12);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 400, 10000, 20, 2, 6, 20, 20, 12);

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
            skill->GetVictim ()->SetProbability (1.0 * (999));
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetRatio (0.5);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetWeak (1);
            skill->GetVictim ()->SetTime (20000);
            skill->GetVictim ()->SetRatio (0.2);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecskilldamage (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetValue (norm ((skill->GetPlayer ()->GetMaxhp () * 0.5 + 1) / 10000) * 10000 +
                                           norm (10000 / (0.5 * skill->GetPlayer ()->GetMaxhp () + 1)) * 0.5 * skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetHeal (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (15);
        }
#endif
    };
}
#endif
