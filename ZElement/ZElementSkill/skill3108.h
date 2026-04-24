#ifndef __CPPGEN_GNET_SKILL3108
#define __CPPGEN_GNET_SKILL3108
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3108:public Skill
    {
      public:
        enum
        { SKILL_ID = 3108 };
          Skill3108 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3108Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1600;
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3108Stub ():SkillStub (3108)
        {
            occupation = 169;
            name = L"无上正觉";
            nativename = "无上正觉";
            icon = "技能反射.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
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
            action[0] = "佛_技能反射";
            action[1] = "佛_技能反射";
            action[2] = "佛_技能反射";
            action[3] = "佛_技能反射";
            action[4] = "佛_技能反射";
            action[5] = "佛_技能反射";
            action[6] = "佛_技能反射";
            action[7] = "佛_技能反射";
            action[8] = "佛_技能反射";
            action[9] = "佛_技能反射";
            action[10] = "佛_技能反射";
            action[11] = "佛_技能反射";
            action[12] = "佛_技能反射";
            action[13] = "佛_技能反射";
            action[14] = "佛_技能反射";
            action[15] = "骑乘_佛_技能反射";
            action[16] = "骑乘_佛_技能反射";
            action[17] = "骑乘_佛_技能反射";
            action[18] = "骑乘_佛_技能反射";
            action[19] = "骑乘_佛_技能反射";
            action[20] = "骑乘_佛_技能反射";
            action[21] = "骑乘_佛_技能反射";
            action[22] = "骑乘_佛_技能反射";
            action[23] = "骑乘_佛_技能反射";
            action[24] = "骑乘_佛_技能反射";
            action[25] = "骑乘_佛_技能反射";
            action[26] = "骑乘_佛_技能反射";
            action[27] = "骑乘_佛_技能反射";
            action[28] = "骑乘_佛_技能反射";
            action[29] = "骑乘_佛_技能反射";
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
#endif
        }
        virtual ~ Skill3108Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (180 + 30 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 200, 500, 500, 600, 900, 2400, 3500, 5600, 8000, 9100, 21700, 36400, 51500, 77500, 79800, 179300, 290600, 437200, 872700, 1620700 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 1, 3, 5, 7, 9, 12, 15, 18, 22, 26, 30, 35, 40, 45, 50, 56, 62, 68, 74, 80 };
            return array[level - 1];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 180000 - 5000 * skill->GetLevel ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               180 + 30 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel (),
                               2 + 0.5 * skill->GetLevel (),
                               180 - 5 * skill->GetLevel (), 10, 5 * skill->GetLevel (), INT (1 + skill->GetLevel () / 4.9));

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
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (2050 + 500 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio (5 * skill->GetLevel ());
            skill->GetVictim ()->SetValue (1 + INT (skill->GetLevel () / 4.9));
            skill->GetVictim ()->SetSkillreflect (1);
            return true;
        }
#endif
    };
}
#endif
