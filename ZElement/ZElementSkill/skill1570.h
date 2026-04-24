#ifndef __CPPGEN_GNET_SKILL1570
#define __CPPGEN_GNET_SKILL1570
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1570:public Skill
    {
      public:
        enum
        { SKILL_ID = 1570 };
          Skill1570 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1570Stub:public SkillStub
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
                skill->SetRatio (0.58 + 0.07 * (skill->GetLevel () > 13 ? (skill->GetLevel () - 13) : skill->GetLevel ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1570Stub ():SkillStub (1570)
        {
            occupation = 149;
            name = L"¸¿×ãÉß¹ÆII";
            nativename = "¸¿×ãÉß¹ÆII";
            icon = "¸¿×ãÉß¹ÆII.dds";
            maxlevel = 30;
            maxlearn = 10;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 4;
            credittype = 10;
            clearmask = 130;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
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
            talent[0] = 1182;
            talent[1] = 1185;
            talent[2] = 1189;
            talent[3] = 2062;
            talent[4] = 1169;
            talent_size = 5;
            action[0] = "¸¿×ãÉß¹Æ";
            action[1] = "¸¿×ãÉß¹Æ";
            action[2] = "¸¿×ãÉß¹Æ";
            action[3] = "¸¿×ãÉß¹Æ";
            action[4] = "¸¿×ãÉß¹Æ";
            action[5] = "¸¿×ãÉß¹Æ";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "¸¿×ãÉß¹Æ";
            action[16] = "¸¿×ãÉß¹Æ";
            action[17] = "¸¿×ãÉß¹Æ";
            action[18] = "¸¿×ãÉß¹Æ";
            action[19] = "¸¿×ãÉß¹Æ";
            action[20] = "¸¿×ãÉß¹Æ";
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
            rangetype = 3;
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
        virtual ~ Skill1570Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1117 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 12 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (30);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (skill->GetLevel () >
                            13 ? 0.2 * (14.08 * (skill->GetLevel () - 13) * (skill->GetLevel () - 13) + 4.42 * (skill->GetLevel () - 13) +
                                        1888) : 0.2 * (14.08 * skill->GetLevel () * skill->GetLevel () + 4.42 * skill->GetLevel () + 1888));
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 7000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 125, 125, 125, 125, 125, 127, 129, 131, 133, 135 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (11);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (26);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (20);
        }
        int GetCreditCost (int level) const
        {
            static int array[10] = { 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 38000, 45000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               12,
                               INT (0.2 * (14.08 * skill->GetLevel () * skill->GetLevel () + 4.42 * skill->GetLevel () + 1888)),
                               58 + 7 * skill->GetLevel (),
                               20,
                               32, INT (6.27 * skill->GetLevel () * skill->GetLevel () + 128 * skill->GetLevel () + 1837), 5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (100 * (1 - 0.002 * skill->GetPlayer ()->GetLevel () + 0.03 * skill->GetT3 ()) *
                                                  (1 + 0.1 * skill->GetT2 ())));
            skill->GetVictim ()->SetTime (32000 - 2000 * skill->GetT2 ());
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetAmount (skill->GetLevel () >
                                            13 ? (1 +
                                                  0.4 * skill->GetT4 ()) * ((6.27 * (skill->GetLevel () - 13) * (skill->GetLevel () - 13) +
                                                                             128 * (skill->GetLevel () - 13) + 1837) * (1 + 0.1 * skill->GetT1 ()) +
                                                                            0.005 * (skill->GetLevel () -
                                                                                     13) * skill->GetT1 () * skill->GetPlayer ()->GetHp ()) : (6.27 *
                                                                                                                                               skill->
                                                                                                                                               GetLevel
                                                                                                                                               () *
                                                                                                                                               skill->
                                                                                                                                               GetLevel
                                                                                                                                               () +
                                                                                                                                               128 *
                                                                                                                                               skill->
                                                                                                                                               GetLevel
                                                                                                                                               () +
                                                                                                                                               1837) *
                                            (1 + 0.1 * skill->GetT1 ()) +
                                            0.005 * skill->GetLevel () * skill->GetT1 () * skill->GetPlayer ()->GetHp ());
            skill->GetVictim ()->SetValue (skill->GetLevel () >
                                           13 ? (6.27 * (skill->GetLevel () - 13) * (skill->GetLevel () - 13) + 128 * (skill->GetLevel () - 13) +
                                                 1837) * 0.1 * (1 +
                                                                0.4 * skill->GetT4 ()) * skill->GetT0 () : (6.27 * skill->GetLevel () *
                                                                                                            skill->GetLevel () +
                                                                                                            128 * skill->GetLevel () +
                                                                                                            1837) * 0.1 * skill->GetT0 ());
            skill->GetVictim ()->SetHpleak2 (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (32000);
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetValue (skill->GetLevel () >
                                           13 ? 5 * (skill->GetLevel () - 13) * (1 + skill->GetT0 ()) : 5 * skill->GetLevel () * (1 +
                                                                                                                                  skill->GetT0 ()));
            skill->GetVictim ()->SetSubanti (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (12);
        }
#endif
    };
}
#endif
