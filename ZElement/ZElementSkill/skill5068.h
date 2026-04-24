#ifndef __CPPGEN_GNET_SKILL5068
#define __CPPGEN_GNET_SKILL5068
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill5068:public Skill
    {
      public:
        enum
        { SKILL_ID = 5068 };
          Skill5068 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill5068Stub:public SkillStub
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill5068Stub ():SkillStub (5068)
        {
            occupation = 128;
            name = L"蛊王姿态";
            nativename = "蛊王姿态";
            icon = "蛊王形态.dds";
            maxlevel = 9;
            maxlearn = 9;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 1;
            targettype = 0;
            autoattack = 0;
            allowform = 5;
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
            action[0] = "变身";
            action[1] = "变身";
            action[2] = "变身";
            action[3] = "变身";
            action[4] = "变身";
            action[5] = "变身";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "0";
            action[14] = "0";
            action[15] = "变身";
            action[16] = "变身";
            action[17] = "变身";
            action[18] = "变身";
            action[19] = "变身";
            action[20] = "变身";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "0";
            action[29] = "0";
            action[30] = "技能攻击2";
            rangetype = 5;
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
#endif
        }
        virtual ~ Skill5068Stub ()
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
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 10;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 0;
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
            return _snwprintf (buffer, length, format, 0, 200 * 9 + 120 * 9 * 9, 35 * 9, 25 * 9, 25 + 5 * 9, 60, 20);

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
            skill->GetVictim ()->SetProbability (1.0 * (0));
            skill->GetVictim ()->SetRatio (-25 * 9);
            skill->GetVictim ()->SetBuffid (200 * 9 + 120 * 9 * 9 + 40 * 9 * 2 + 24 * 9 * 9 * 2);
            skill->GetVictim ()->SetAmount (25 + 5 * 9);
            skill->GetVictim ()->SetValue (35 * 9);
            skill->GetVictim ()->SetInsanityform (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (4000 * norm (2));
            skill->GetVictim ()->SetRatio (0.25 * 2);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (100);
            skill->GetVictim ()->SetRetort (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (60000);
            skill->GetVictim ()->SetRatio (0.2);
            skill->GetVictim ()->SetAttack2mp (1);
            return true;
        }
#endif
    };
}
#endif
