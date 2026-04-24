#ifndef __CPPGEN_GNET_SKILL3725
#define __CPPGEN_GNET_SKILL3725
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3725:public Skill
    {
      public:
        enum
        { SKILL_ID = 3725 };
          Skill3725 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3725Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1200;
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
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3725Stub ():SkillStub (3725)
        {
            occupation = 128;
            name = L"·üôË»ê";
            nativename = "·üôË»ê";
            icon = "·üôË»ê.dds";
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
            action[0] = "Ì«ê»_×£¸£";
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
            action[11] = "Ì«ê»_×£¸£";
            action[12] = "0";
            action[13] = "0";
            action[14] = "0";
            action[15] = "Ì«ê»_Æï³Ë_×£¸£";
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
            action[26] = "Ì«ê»_Æï³Ë_×£¸£";
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
        virtual ~ Skill3725Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (500);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 120000 - 10000 * 3;
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
            return _snwprintf (buffer, length, format,
                               500, 4 * 6, 3 * 6, 8 * 6, 8 * 6, 120, 3 * 6, 2 * 6, 10 * 6, 8 * 6, 8, 20 * 6, 1 + 0.8 * 6, 2 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (3000 * 6 + 3000 * 3);
            skill->GetVictim ()->SetRatio (0.08 * 6);
            skill->GetVictim ()->SetValue (0.08 * 6);
            skill->GetVictim ()->SetSwift (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (3000 * 6 + 3000 * 3);
            skill->GetVictim ()->SetRatio (0.04 * 6);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetInccrithurt (1);
            skill->GetVictim ()->SetProbability (1.0 * (0));
            skill->GetVictim ()->SetTime (3000 * 6 + 3000 * 3);
            skill->GetVictim ()->SetRatio (4099);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount (6);
            skill->GetVictim ()->SetValue (3729);
            skill->GetVictim ()->SetActiveonfilteradd (1);
            skill->GetVictim ()->SetProbability (1.0 * (1));
            skill->GetVictim ()->SetTime (3000 * 6 + 3000 * 3);
            skill->GetVictim ()->SetRatio (4098);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (6);
            skill->GetVictim ()->SetValue (3726);
            skill->GetVictim ()->SetActiveonfilteradd (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (1));
            skill->GetVictim ()->SetTime (3000 * 6 + 3000 * 3);
            skill->GetVictim ()->SetRatio (4097);
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetAmount (6);
            skill->GetVictim ()->SetValue (3728);
            skill->GetVictim ()->SetActiveonfilteradd (1);
            skill->GetVictim ()->SetProbability (1.0 * (0));
            skill->GetVictim ()->SetTime (3000 * 6 + 3000 * 3);
            skill->GetVictim ()->SetRatio (4350);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (6);
            skill->GetVictim ()->SetValue (3727);
            skill->GetVictim ()->SetActiveonfilteradd (1);
            return true;
        }
#endif
    };
}
#endif
