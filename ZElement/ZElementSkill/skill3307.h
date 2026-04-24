#ifndef __CPPGEN_GNET_SKILL3307
#define __CPPGEN_GNET_SKILL3307
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3307:public Skill
    {
      public:
        enum
        { SKILL_ID = 3307 };
          Skill3307 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3307Stub:public SkillStub
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
      Skill3307Stub ():SkillStub (3307)
        {
            occupation = 128;
            name = L"万象生佛";
            nativename = "万象生佛";
            icon = "万象生佛.dds";
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
            action[0] = "天音寺_万象生佛";
            action[1] = "天音寺_万象生佛";
            action[2] = "天音寺_万象生佛";
            action[3] = "天音寺_万象生佛";
            action[4] = "天音寺_万象生佛";
            action[5] = "天音寺_万象生佛";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "天音寺_骑乘_万象生佛";
            action[16] = "天音寺_骑乘_万象生佛";
            action[17] = "天音寺_骑乘_万象生佛";
            action[18] = "天音寺_骑乘_万象生佛";
            action[19] = "天音寺_骑乘_万象生佛";
            action[20] = "天音寺_骑乘_万象生佛";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
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
        virtual ~ Skill3307Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (4500);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 60000 - 5000 * 3;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (30);
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
            return _snwprintf (buffer, length, format, 1000, 30, 15 + 10 * 4, 6000 + 3000 * 4, 20 + 5 * 4, 60);

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
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetValue (norm ((skill->GetPlayer ()->GetMaxhp () * (0.15 + 0.1 * 4) * (1 + 0.15 * 3) + 1) / (6000 + 3000 * 4)) *
                                           (6000 + 3000 * 4) +
                                           norm ((6000 + 3000 * 4) / ((0.15 + 0.1 * 4) * (1 + 0.15 * 3) * skill->GetPlayer ()->GetMaxhp () +
                                                                      1)) * (0.15 + 0.1 * 4) * (1 + 0.15 * 3) * skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetHeal (1);
            skill->GetVictim ()->SetProbability (1.0 * (20 + 5 * 4 + 5 * 3));
            skill->GetVictim ()->SetCleardizzy (1);
            skill->GetVictim ()->SetProbability (1.0 * (20 + 5 * 4 + 5 * 3));
            skill->GetVictim ()->SetClearsilent (1);
            skill->GetVictim ()->SetProbability (1.0 * (20 + 5 * 4 + 5 * 3));
            skill->GetVictim ()->SetClearwrap (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (900000 * norm (3));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (5);
            skill->GetVictim ()->SetValue (100 * 4 * 3);
            skill->GetVictim ()->SetTiangu (1);
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
