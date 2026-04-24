#ifndef __CPPGEN_GNET_SKILL1114
#define __CPPGEN_GNET_SKILL1114
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1114:public Skill
    {
      public:
        enum
        { SKILL_ID = 1114 };
          Skill1114 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1114Stub:public SkillStub
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
      Skill1114Stub ():SkillStub (1114)
        {
            occupation = 27;
            name = L"¹éÁéÃØÓ¡";
            nativename = "¹éÁéÃØÓ¡";
            icon = "¹éÁéÃØÓ¡.dds";
            maxlevel = 20;
            maxlearn = 9;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 4;
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
            skill_class = 4;
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
            talent[0] = 1180;
            talent[1] = 1184;
            talent_size = 2;
            action[0] = "¹éÁéÃØÓ¡";
            action[1] = "¹éÁéÃØÓ¡";
            action[2] = "¹éÁéÃØÓ¡";
            action[3] = "¹éÁéÃØÓ¡";
            action[4] = "¹éÁéÃØÓ¡";
            action[5] = "¹éÁéÃØÓ¡";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹éÁéÃØÓ¡";
            action[16] = "¹éÁéÃØÓ¡";
            action[17] = "¹éÁéÃØÓ¡";
            action[18] = "¹éÁéÃØÓ¡";
            action[19] = "¹éÁéÃØÓ¡";
            action[20] = "¹éÁéÃØÓ¡";
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
        virtual ~ Skill1114Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1113 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 180000 - 18000 * skill->GetT1 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 75, 79, 83, 87, 91, 94, 97, 100, 103 };
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
            return _snwprintf (buffer, length, format, 0, 27 + 3 * skill->GetLevel (), 33, skill->GetLevel (), 2 * skill->GetLevel (), 85);

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
            skill->GetVictim ()->SetProbability (1.0 * (33 + 5 * skill->GetT0 ()));
            skill->GetVictim ()->SetTime (27000 + 3000 * skill->GetLevel () + 3000 * skill->GetT1 () + 100);
            skill->GetVictim ()->SetRatio (0.01 * skill->GetLevel () + 0.01 * skill->GetT0 ());
            skill->GetVictim ()->SetAttack2mp (1);
            skill->GetVictim ()->SetTime (27000 + 3000 * skill->GetLevel () + 3000 * skill->GetT1 () + 100);
            skill->GetVictim ()->SetRatio (0.85);
            skill->GetVictim ()->SetAmount (2 * skill->GetLevel () * skill->GetPlayer ()->GetMaxmp () + 1);
            skill->GetVictim ()->SetMagicshield (1);
            return true;
        }
#endif
    };
}
#endif
