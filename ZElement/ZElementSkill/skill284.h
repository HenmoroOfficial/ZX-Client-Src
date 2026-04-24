#ifndef __CPPGEN_GNET_SKILL284
#define __CPPGEN_GNET_SKILL284
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill284:public Skill
    {
      public:
        enum
        { SKILL_ID = 284 };
          Skill284 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill284Stub:public SkillStub
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
                skill->SetRatio (1 + 0.05 * skill->GetT1 ());
                skill->SetPlus ((skill->GetPlayer ()->GetHp () * 0.3 + skill->GetPlayer ()->GetMp ()) * (0.12 + 0.03 * skill->GetLevel ()));
                skill->GetPlayer ()->SetHp (0.7 * skill->GetPlayer ()->GetHp ());
                skill->GetPlayer ()->SetMp (0.05 * skill->GetT0 () * skill->GetPlayer ()->GetMp ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill284Stub ():SkillStub (284)
        {
            occupation = 12;
            name = L"天地同寿";
            nativename = "天地同寿";
            icon = "天地同寿.dds";
            maxlevel = 20;
            maxlearn = 9;
            type = 1;
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
            skill_class = 3;
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
            preskillnum = 1;
            talent[0] = 678;
            talent[1] = 681;
            talent_size = 2;
            action[0] = "天音寺_舍生取义";
            action[1] = "天音寺_舍生取义";
            action[2] = "天音寺_舍生取义";
            action[3] = "天音寺_舍生取义";
            action[4] = "天音寺_舍生取义";
            action[5] = "天音寺_舍生取义";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "天音寺_骑乘_舍生取义";
            action[16] = "天音寺_骑乘_舍生取义";
            action[17] = "天音寺_骑乘_舍生取义";
            action[18] = "天音寺_骑乘_舍生取义";
            action[19] = "天音寺_骑乘_舍生取义";
            action[20] = "天音寺_骑乘_舍生取义";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "技能攻击1";
            rangetype = 2;
            doenchant = false;
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
        virtual ~ Skill284Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 384 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 6 };
            return array[index];
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
            return (10500 - 500 * skill->GetLevel ()) * (100 - 15 * skill->GetT0 ()) / 100;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 100, 102, 104, 106, 108, 110, 112, 114, 116 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (5.8 + 0.2 * skill->GetLevel ());
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (5.8 + 0.2 * skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (2.8 + 0.2 * skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               30, 4.8 + 0.2 * skill->GetLevel (), 12 + 3 * skill->GetLevel (), 10.5 - 0.5 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        int GetCoverage (Skill * skill) const
        {
            return (int) (30);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
