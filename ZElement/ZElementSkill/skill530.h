#ifndef __CPPGEN_GNET_SKILL530
#define __CPPGEN_GNET_SKILL530
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill530:public Skill
    {
      public:
        enum
        { SKILL_ID = 530 };
          Skill530 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill530Stub:public SkillStub
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
      Skill530Stub ():SkillStub (530)
        {
            occupation = 16;
            name = L"»ÃÕæ";
            nativename = "»ÃÕæ";
            icon = "»ÃÕæ.dds";
            maxlevel = 4;
            maxlearn = 3;
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
            preskillnum = 1;
            talent[0] = 763;
            talent[1] = 764;
            talent[2] = 769;
            talent_size = 3;
            action[0] = "ºÏ»¶ÅÉ_»ÃÕæ";
            action[1] = "ºÏ»¶ÅÉ_»ÃÕæ";
            action[2] = "ºÏ»¶ÅÉ_»ÃÕæ";
            action[3] = "ºÏ»¶ÅÉ_»ÃÕæ";
            action[4] = "ºÏ»¶ÅÉ_»ÃÕæ";
            action[5] = "ºÏ»¶ÅÉ_»ÃÕæ";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_»ÃÕæ";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_»ÃÕæ";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_»ÃÕæ";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_»ÃÕæ";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_»ÃÕæ";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_»ÃÕæ";
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
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill530Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 528 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (250 + 10 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 150000 - 12000 * skill->GetT0 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[3] = { 106, 114, 122 };
            return array[level - 1];
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
                               250 + 10 * skill->GetLevel (), 10 + 5 * skill->GetLevel (), 15 * skill->GetLevel (), 33, skill->GetLevel ());

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
            skill->GetVictim ()->SetTime ((10000 + 5000 * skill->GetLevel ()) * (1 + 0.1 * skill->GetT1 ()) + 100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (15 * skill->GetLevel () * (1 + 0.15 * skill->GetT2 ()));
            skill->GetVictim ()->SetAdddodge (1);
            skill->GetVictim ()->SetProbability (1.0 * (33));
            skill->GetVictim ()->SetTime ((10000 + 5000 * skill->GetLevel ()) * (1 + 0.1 * skill->GetT1 ()) + 100);
            skill->GetVictim ()->SetRatio (0.01 * skill->GetLevel () * (1 + 0.2 * skill->GetT2 ()));
            skill->GetVictim ()->SetAttack2mp (1);
            return true;
        }
#endif
    };
}
#endif
