#ifndef __CPPGEN_GNET_SKILL3112
#define __CPPGEN_GNET_SKILL3112
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3112:public Skill
    {
      public:
        enum
        { SKILL_ID = 3112 };
          Skill3112 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3112Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1400;
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
                return 4000;
            }
            void Calculate (Skill * skill) const
            {
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State3:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 100;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar2 (1);
                skill->GetPlayer ()->SetVar1 (skill->GetCharging ());
                skill->SetRatio (0);
                skill->SetPlus (0);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State4:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 500;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar2 (2);
                skill->SetRatio (1 + (skill->GetCharging () > 250 ? skill->GetCharging () * 0.0006 * 0.1 * skill->GetLevel () : 0));
                skill->SetSaint (skill->GetCharging () >
                                 250 ? skill->GetCharging () * 0.0005 * 0.2 * (skill->GetT0 () + skill->GetT1 ()) * skill->GetLevel () : 0);
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill3112Stub ():SkillStub (3112)
        {
            occupation = 169;
            name = L"六字真言";
            nativename = "六字真言";
            icon = "极乐一指.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 3;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            clearmask = 4;
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
            talent[0] = 3105;
            talent[1] = 3106;
            talent_size = 2;
            action[0] = "佛_极乐一指";
            action[1] = "佛_极乐一指";
            action[2] = "佛_极乐一指";
            action[3] = "佛_极乐一指";
            action[4] = "佛_极乐一指";
            action[5] = "佛_极乐一指";
            action[6] = "佛_极乐一指";
            action[7] = "佛_极乐一指";
            action[8] = "佛_极乐一指";
            action[9] = "佛_极乐一指";
            action[10] = "佛_极乐一指";
            action[11] = "佛_极乐一指";
            action[12] = "佛_极乐一指";
            action[13] = "佛_极乐一指";
            action[14] = "佛_极乐一指";
            action[15] = "骑乘_佛_极乐一指";
            action[16] = "骑乘_佛_极乐一指";
            action[17] = "骑乘_佛_极乐一指";
            action[18] = "骑乘_佛_极乐一指";
            action[19] = "骑乘_佛_极乐一指";
            action[20] = "骑乘_佛_极乐一指";
            action[21] = "骑乘_佛_极乐一指";
            action[22] = "骑乘_佛_极乐一指";
            action[23] = "骑乘_佛_极乐一指";
            action[24] = "骑乘_佛_极乐一指";
            action[25] = "骑乘_佛_极乐一指";
            action[26] = "骑乘_佛_极乐一指";
            action[27] = "骑乘_佛_极乐一指";
            action[28] = "骑乘_佛_极乐一指";
            action[29] = "骑乘_佛_极乐一指";
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
            statestub.push_back (new State3 ());
            statestub.push_back (new State4 ());
#endif
        }
        virtual ~ Skill3112Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (190 + 50 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 400, 500, 500, 700, 1500, 3100, 3900, 6500, 10300, 16900, 21800, 43300, 49900, 81300, 84500, 197600, 340200, 435500, 998600,
 1897100 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 2, 4, 6, 8, 10, 13, 16, 19, 23, 27, 31, 36, 41, 46, 51, 57, 63, 69, 75, 81 };
            return array[level - 1];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (50);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 6000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 180000 - 4000 * skill->GetLevel ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 150 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (45);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (21);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               190 + 50 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel (),
                               180 - 4 * skill->GetLevel (), 16, 6 * skill->GetLevel (), 1 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () > 0 && skill->GetPlayer ()->GetVar1 () < 4100 ? 120 : 0));
            skill->GetVictim ()->SetAmount (skill->GetPlayer ()->GetVar1 () > 3800
                                            && skill->GetPlayer ()->GetVar2 () ==
                                            1 ? INT (skill->GetPlayer ()->GetVar1 () * 0.001) + 2 : INT (skill->GetPlayer ()->GetVar1 () * 0.001) +
                                            1);
            skill->GetVictim ()->SetClearbuff (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar2 () == 2 ? 120 : 0));
            skill->GetVictim ()->SetTime (6000 + norm (skill->GetLevel () / 19.9) * 6000);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (skill->GetPlayer ()->GetMaxhp () * 0.6);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetMaxhp () - skill->GetPlayer ()->GetHp ());
            skill->GetVictim ()->SetSubhp (1);
            return true;
        }
#endif
    };
}
#endif
