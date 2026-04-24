#ifndef __CPPGEN_GNET_SKILL273
#define __CPPGEN_GNET_SKILL273
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill273:public Skill
    {
      public:
        enum
        { SKILL_ID = 273 };
          Skill273 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill273Stub:public SkillStub
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
      Skill273Stub ():SkillStub (273)
        {
            occupation = 11;
            name = L"ºêÔ¸";
            nativename = "ºêÔ¸";
            icon = "ºêÔ¸.dds";
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
            talent[1] = 711;
            talent[2] = 719;
            talent_size = 3;
            action[0] = "ÌìÒôËÂ_ºêÔ¸";
            action[1] = "ÌìÒôËÂ_ºêÔ¸";
            action[2] = "ÌìÒôËÂ_ºêÔ¸";
            action[3] = "ÌìÒôËÂ_ºêÔ¸";
            action[4] = "ÌìÒôËÂ_ºêÔ¸";
            action[5] = "ÌìÒôËÂ_ºêÔ¸";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_ºêÔ¸";
            action[16] = "ÌìÒôËÂ_Æï³Ë_ºêÔ¸";
            action[17] = "ÌìÒôËÂ_Æï³Ë_ºêÔ¸";
            action[18] = "ÌìÒôËÂ_Æï³Ë_ºêÔ¸";
            action[19] = "ÌìÒôËÂ_Æï³Ë_ºêÔ¸";
            action[20] = "ÌìÒôËÂ_Æï³Ë_ºêÔ¸";
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
        virtual ~ Skill273Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 274 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (17);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (3.7 * skill->GetLevel () * skill->GetLevel () + 20 * skill->GetLevel () + 247);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 2000 - 1000 * skill->GetT0 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[12] = { 46, 48, 50, 52, 56, 59, 62, 65, 68, 72, 75, 78 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (13);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (3.7 * skill->GetLevel () * skill->GetLevel () + 20 * skill->GetLevel () + 247),
                               INT (7.4 * skill->GetLevel () * skill->GetLevel () + 40 * skill->GetLevel () + 494),
                               INT (1.29 * skill->GetLevel () * skill->GetLevel () + 6.9 * skill->GetLevel () + 172.8), 45 + 15 * skill->GetLevel ());

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
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetValue (INT (7.4 * skill->GetLevel () * skill->GetLevel () + 40 * skill->GetLevel () + 494) *
                                           (1 + 0.15 * skill->GetT2 ()));
            skill->GetVictim ()->SetHeal (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (INT (3 * (1.29 * skill->GetLevel () * skill->GetLevel () + 6.9 * skill->GetLevel () + 172.8)) *
                                           (1 + 0.15 * skill->GetT2 ()));
            skill->GetVictim ()->SetHpgen (1);
            skill->GetVictim ()->SetTime (90000 * (1 + skill->GetT1 ()));
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue ((0.45 + 0.15 * skill->GetLevel ()) * skill->GetPlayer ()->GetLevel () * (1 + 0.6 * skill->GetT1 ()));
            skill->GetVictim ()->SetAddmp (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (1800000 * norm (skill->GetT2 ()));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetValue (100 * skill->GetLevel () * skill->GetT2 ());
            skill->GetVictim ()->SetTiangu (1);
            return true;
        }
#endif
    };
}
#endif
