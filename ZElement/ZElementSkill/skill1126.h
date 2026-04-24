#ifndef __CPPGEN_GNET_SKILL1126
#define __CPPGEN_GNET_SKILL1126
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1126:public Skill
    {
      public:
        enum
        { SKILL_ID = 1126 };
          Skill1126 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1126Stub:public SkillStub
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
                skill->SetRatio (0.48 + 0.02 * skill->GetLevel ());
                skill->SetPlus (0.04 * (skill->GetPlayer ()->GetHp () + skill->GetPlayer ()->GetMp ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1126Stub ():SkillStub (1126)
        {
            occupation = 28;
            name = L"°Ù¹Æà¢»ê";
            nativename = "°Ù¹Æà¢»ê";
            icon = "°Ù¹Æà¢»ê.dds";
            maxlevel = 20;
            maxlearn = 12;
            type = 1;
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
            talent[1] = 1185;
            talent[2] = 1186;
            talent[3] = 1189;
            talent[4] = 2062;
            talent_size = 5;
            action[0] = "°Ù¹Æà¢»ê";
            action[1] = "°Ù¹Æà¢»ê";
            action[2] = "°Ù¹Æà¢»ê";
            action[3] = "°Ù¹Æà¢»ê";
            action[4] = "°Ù¹Æà¢»ê";
            action[5] = "°Ù¹Æà¢»ê";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "°Ù¹Æà¢»ê";
            action[16] = "°Ù¹Æà¢»ê";
            action[17] = "°Ù¹Æà¢»ê";
            action[18] = "°Ù¹Æà¢»ê";
            action[19] = "°Ù¹Æà¢»ê";
            action[20] = "°Ù¹Æà¢»ê";
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
        virtual ~ Skill1126Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1123 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (24);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0.25 * (8.96 * skill->GetLevel () * skill->GetLevel () + 371.84 * skill->GetLevel () + 3320));
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
            static int array[12] = { 115, 117, 119, 121, 123, 125, 125, 125, 125, 125, 125, 125 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (22);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (20);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (0.25 * (8.96 * skill->GetLevel () * skill->GetLevel () + 371.84 * skill->GetLevel () + 3320)),
                               48 + 2 * skill->GetLevel (),
                               20,
                               48,
                               INT (8.96 * skill->GetLevel () * skill->GetLevel () + 371.84 * skill->GetLevel () + 8320), 2 * skill->GetLevel (), 4);

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
                                                 (100 * (1 - 0.002 * skill->GetPlayer ()->GetLevel () + 0.03 * skill->GetT4 ()) *
                                                  (1 + 0.1 * skill->GetT3 ())));
            skill->GetVictim ()->SetTime (48000 - 2000 * skill->GetT3 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount ((8.96 * skill->GetLevel () * skill->GetLevel () + 371.84 * skill->GetLevel () + 8320) *
                                            (1 + 0.1 * skill->GetT1 ()) +
                                            0.005 * skill->GetT1 () * skill->GetLevel () * skill->GetPlayer ()->GetHp ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (2 * skill->GetLevel () + 3 * skill->GetT0 ()));
            skill->GetVictim ()->SetTime (4000 + 1000 * skill->GetT2 () + 100);
            skill->GetVictim ()->SetDiet (1);
            skill->GetVictim ()->SetProbability (1.0 * (norm (zrand (100) / 67) * 600));
            skill->GetVictim ()->SetTime (2050 * skill->GetT2 ());
            skill->GetVictim ()->SetWrap (1);
            return true;
        }
#endif
    };
}
#endif
