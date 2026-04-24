#ifndef __CPPGEN_GNET_SKILL1746
#define __CPPGEN_GNET_SKILL1746
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1746:public Skill
    {
      public:
        enum
        { SKILL_ID = 1746 };
          Skill1746 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1746Stub:public SkillStub
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1746Stub ():SkillStub (1746)
        {
            occupation = 151;
            name = L"²âÊÔÕÙ»½50¸ö¹Ö";
            nativename = "²âÊÔÕÙ»½50¸ö¹Ö";
            icon = "ÐýÁú»ÃÉ±II.dds";
            maxlevel = 50;
            maxlearn = 50;
            type = 2;
            eventflag = 1;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            clearmask = 0;
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
            action[0] = "";
            action[1] = "";
            action[2] = "";
            action[3] = "";
            action[4] = "";
            action[5] = "";
            action[6] = "";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "0";
            action[14] = "0";
            action[15] = "";
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
            action[27] = "0";
            action[28] = "0";
            action[29] = "0";
            action[30] = "0";
            rangetype = 5;
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
        virtual ~ Skill1746Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (20);
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
        float GetPraydistance (Skill * skill) const
        {
            return (float) (10);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
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
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (3600 * 1000);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount (50);
            skill->GetVictim ()->SetValue (30007);
            skill->GetVictim ()->SetSummon (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (3600 * 1000);
            skill->GetVictim ()->SetRatio (5);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (29999);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetCreateobject (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (3600 * 1000);
            skill->GetVictim ()->SetRatio (6);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (29999);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetCreateobject (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (3600 * 1000);
            skill->GetVictim ()->SetRatio (7);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (29999);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetCreateobject (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (3600 * 1000);
            skill->GetVictim ()->SetRatio (8);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (29999);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetCreateobject (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (3600 * 1000);
            skill->GetVictim ()->SetRatio (9);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (29999);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetCreateobject (1);
            return true;
        }
#endif
    };
}
#endif
