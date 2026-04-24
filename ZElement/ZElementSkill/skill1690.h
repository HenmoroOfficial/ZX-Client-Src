#ifndef __CPPGEN_GNET_SKILL1690
#define __CPPGEN_GNET_SKILL1690
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1690:public Skill
    {
      public:
        enum
        { SKILL_ID = 1690 };
          Skill1690 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1690Stub:public SkillStub
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
      Skill1690Stub ():SkillStub (1690)
        {
            occupation = 146;
            name = L"°Ë»ÄÕæ»ð£º°µ";
            nativename = "°Ë»ÄÕæ»ð£º°µ";
            icon = "°Ë»ÄÕæ»ð¾÷.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 3;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
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
            skill_class = 2;
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
            action[0] = "ÇàÔÆÃÅ_°Ë»ÄÕæ»ð¾÷";
            action[1] = "ÇàÔÆÃÅ_°Ë»ÄÕæ»ð¾÷";
            action[2] = "ÇàÔÆÃÅ_°Ë»ÄÕæ»ð¾÷";
            action[3] = "ÇàÔÆÃÅ_°Ë»ÄÕæ»ð¾÷";
            action[4] = "ÇàÔÆÃÅ_°Ë»ÄÕæ»ð¾÷";
            action[5] = "ÇàÔÆÃÅ_°Ë»ÄÕæ»ð¾÷";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_°Ë»ÄÕæ»ð¾÷";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_°Ë»ÄÕæ»ð¾÷";
            action[17] = "ÇàÔÆÃÅ_Æï³Ë_°Ë»ÄÕæ»ð¾÷";
            action[18] = "ÇàÔÆÃÅ_Æï³Ë_°Ë»ÄÕæ»ð¾÷";
            action[19] = "ÇàÔÆÃÅ_Æï³Ë_°Ë»ÄÕæ»ð¾÷";
            action[20] = "ÇàÔÆÃÅ_Æï³Ë_°Ë»ÄÕæ»ð¾÷";
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
        virtual ~ Skill1690Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 2);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1500);
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
            return (float) (12);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 1500);

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
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (30000 + 30000 * norm (skill->GetPlayer ()->GetMaxmp () / (4 * skill->GetPlayer ()->GetMp () + 1)) + 100);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetValue (4 * skill->GetPlayer ()->GetLevel () *
                                           (1 + norm (skill->GetPlayer ()->GetMaxhp () / (2 * skill->GetPlayer ()->GetHp () + 1))));
            skill->GetVictim ()->SetSubdefence (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (16000 + 100);
            skill->GetVictim ()->SetRatio (0.251);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetDecskillaccu (1);
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
