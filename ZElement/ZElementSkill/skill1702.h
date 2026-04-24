#ifndef __CPPGEN_GNET_SKILL1702
#define __CPPGEN_GNET_SKILL1702
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1702:public Skill
    {
      public:
        enum
        { SKILL_ID = 1702 };
          Skill1702 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1702Stub:public SkillStub
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1702Stub ():SkillStub (1702)
        {
            occupation = 146;
            name = L"几多愁：暗";
            nativename = "几多愁：暗";
            icon = "几多愁.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 3;
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
            skill_class = 1;
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
            action[0] = "合欢派_几多愁";
            action[1] = "合欢派_几多愁";
            action[2] = "合欢派_几多愁";
            action[3] = "合欢派_几多愁";
            action[4] = "合欢派_几多愁";
            action[5] = "合欢派_几多愁";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "合欢派_骑乘_几多愁";
            action[16] = "合欢派_骑乘_几多愁";
            action[17] = "合欢派_骑乘_几多愁";
            action[18] = "合欢派_骑乘_几多愁";
            action[19] = "合欢派_骑乘_几多愁";
            action[20] = "合欢派_骑乘_几多愁";
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
        virtual ~ Skill1702Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (950);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 360000;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (25);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 950);

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
            skill->GetVictim ()->SetProbability (1.0 * (100 * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetTime (12000);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount (12);
            skill->GetVictim ()->SetValue (1000000);
            skill->GetVictim ()->SetMpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (100 * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetDrainmagic (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (16000 + 100);
            skill->GetVictim ()->SetRatio (0.251);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetDecskillaccu (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (50));
            skill->GetVictim ()->SetTime (60000 + 100);
            skill->GetVictim ()->SetRatio (0.05);
            skill->GetVictim ()->SetAttack2hp (1);
            skill->GetVictim ()->SetProbability (1.0 * (50));
            skill->GetVictim ()->SetTime (60000 + 100);
            skill->GetVictim ()->SetRatio (0.05);
            skill->GetVictim ()->SetAttack2mp (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (25);
        }
#endif
    };
}
#endif
