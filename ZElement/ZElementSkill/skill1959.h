#ifndef __CPPGEN_GNET_SKILL1959
#define __CPPGEN_GNET_SKILL1959
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1959:public Skill
    {
      public:
        enum
        { SKILL_ID = 1959 };
          Skill1959 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1959Stub:public SkillStub
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
      Skill1959Stub ():SkillStub (1959)
        {
            occupation = 128;
            name = L"³þººÎä¶·B";
            nativename = "³þººÎä¶·B";
            icon = "";
            maxlevel = 10;
            maxlearn = 10;
            type = 5;
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
            action[30] = "¼¼ÄÜ¹¥»÷2";
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
        virtual ~ Skill1959Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
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
            return 1000;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (4 + 4 * skill->GetLevel ());
        }
        float GetAngle (Skill * skill) const
        {
            return (float) (1 - 0.0111111 * (180));
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
            return _snwprintf (buffer, length, format);
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
            skill->GetVictim ()->SetTime (30000 + 30000 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio (0.1 * skill->GetLevel () + 0.1);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetInchp (1);
            skill->GetVictim ()->SetTime (30000 + 30000 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio (0.1 * skill->GetLevel () + 0.1);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetIncmp (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (30000 + 30000 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio (0.1 * skill->GetLevel () + 0.1);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetInchp (1);
            skill->GetVictim ()->SetTime (30000 + 30000 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio (0.1 * skill->GetLevel () + 0.1);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetIncmp (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (4 + 4 * skill->GetLevel ());
        }
#endif
    };
}
#endif
