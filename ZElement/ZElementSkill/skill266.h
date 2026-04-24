#ifndef __CPPGEN_GNET_SKILL266
#define __CPPGEN_GNET_SKILL266
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill266:public Skill
    {
      public:
        enum
        { SKILL_ID = 266 };
          Skill266 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill266Stub:public SkillStub
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
      Skill266Stub ():SkillStub (266)
        {
            occupation = 10;
            name = L"´ó±¯Öä";
            nativename = "´ó±¯Öä";
            icon = "´ó±¯Öä.dds";
            maxlevel = 20;
            maxlearn = 12;
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
            talent[0] = 703;
            talent[1] = 704;
            talent[2] = 707;
            talent[3] = 719;
            talent_size = 4;
            action[0] = "ÌìÒôËÂ_´ó±¯Öä";
            action[1] = "ÌìÒôËÂ_´ó±¯Öä";
            action[2] = "ÌìÒôËÂ_´ó±¯Öä";
            action[3] = "ÌìÒôËÂ_´ó±¯Öä";
            action[4] = "ÌìÒôËÂ_´ó±¯Öä";
            action[5] = "ÌìÒôËÂ_´ó±¯Öä";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_´ó±¯Öä";
            action[16] = "ÌìÒôËÂ_Æï³Ë_´ó±¯Öä";
            action[17] = "ÌìÒôËÂ_Æï³Ë_´ó±¯Öä";
            action[18] = "ÌìÒôËÂ_Æï³Ë_´ó±¯Öä";
            action[19] = "ÌìÒôËÂ_Æï³Ë_´ó±¯Öä";
            action[20] = "ÌìÒôËÂ_Æï³Ë_´ó±¯Öä";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "¼¼ÄÜ¹¥»÷1";
            rangetype = 0;
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
        virtual ~ Skill266Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 267 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (15.2 + 0.8 * skill->GetLevel ());
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1.9 * skill->GetLevel () * skill->GetLevel () + 9.2 * skill->GetLevel () + 32);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 1000 - 1000 * skill->GetT0 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[12] = { 16, 18, 19, 22, 25, 29, 33, 37, 41, 43, 45, 48 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (13.2 + 0.8 * skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (11.2 + 0.8 * skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (1.9 * skill->GetLevel () * skill->GetLevel () + 9.2 * skill->GetLevel () + 32),
                               INT (2.9 * (1.9 * skill->GetLevel () * skill->GetLevel () + 9.2 * skill->GetLevel () + 32)),
                               11.2 + 0.8 * skill->GetLevel (), 45 + 15 * skill->GetLevel ());

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
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetValue ((INT (2.9 * (1.9 * skill->GetLevel () * skill->GetLevel () + 9.2 * skill->GetLevel () + 32))) *
                                           (INT (zrand (100) / (100 - 9 * skill->GetT1 ())) + 1) * (1 + 0.15 * skill->GetT3 ()));
            skill->GetVictim ()->SetHeal (1);
            skill->GetVictim ()->SetTime (90000 * (1 + skill->GetT2 ()));
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetValue ((0.45 + 0.15 * skill->GetLevel ()) * skill->GetPlayer ()->GetLevel () * (1 + 0.6 * skill->GetT2 ()));
            skill->GetVictim ()->SetAddhp (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (1800000 * norm (skill->GetT3 ()));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (100 * skill->GetLevel () * skill->GetT3 ());
            skill->GetVictim ()->SetTiangu (1);
            return true;
        }
#endif
    };
}
#endif
