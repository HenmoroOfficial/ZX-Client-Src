#ifndef __CPPGEN_GNET_SKILL272
#define __CPPGEN_GNET_SKILL272
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill272:public Skill
    {
      public:
        enum
        { SKILL_ID = 272 };
          Skill272 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill272Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 2400;
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
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.02 * skill->GetLevel () + 0.01 * skill->GetPlayer ()->GetLevel () / 7);
                skill->SetPlus (15 * skill->GetLevel () + 24 - 0.32 * skill->GetLevel () * skill->GetLevel () +
                                skill->GetLevel () * skill->GetPlayer ()->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill272Stub ():SkillStub (272)
        {
            occupation = 130;
            name = L"众生轮回";
            nativename = "众生轮回";
            icon = "众生轮回.dds";
            maxlevel = 8;
            maxlearn = 8;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
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
            action[0] = "天音寺_轮回珠";
            action[1] = "天音寺_轮回珠";
            action[2] = "天音寺_轮回珠";
            action[3] = "天音寺_轮回珠";
            action[4] = "天音寺_轮回珠";
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
            action[15] = "天音寺_骑乘_众生轮回";
            action[16] = "天音寺_骑乘_众生轮回";
            action[17] = "天音寺_骑乘_众生轮回";
            action[18] = "天音寺_骑乘_众生轮回";
            action[19] = "天音寺_骑乘_众生轮回";
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
            action[30] = "技能攻击1";
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
        virtual ~ Skill272Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1.6 * skill->GetLevel () * skill->GetLevel () + 24 * skill->GetLevel () + 216);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 60000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (10);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (10);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (6);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (1.6 * skill->GetLevel () * skill->GetLevel () + 24 * skill->GetLevel () + 216),
                               2 * skill->GetLevel (),
                               15 * skill->GetLevel () + 24 - 0.32 * skill->GetLevel () * skill->GetLevel (),
                               skill->GetLevel (), 9 + skill->GetLevel (), 8 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetLevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0.09 + 0.01 * skill->GetLevel ());
            skill->GetVictim ()->SetSecondattack (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetValue (8 * skill->GetLevel ());
            skill->GetVictim ()->SetJuqi (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (20);
        }
#endif
    };
}
#endif
