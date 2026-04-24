#ifndef __CPPGEN_GNET_SKILL528
#define __CPPGEN_GNET_SKILL528
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill528:public Skill
    {
      public:
        enum
        { SKILL_ID = 528 };
          Skill528 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill528Stub:public SkillStub
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
                skill->SetCrit (0.02 * skill->GetT1 ());
                skill->SetCrithurt (0.08 * skill->GetLevel ());
                skill->SetRatio (1.22 + 0.03 * skill->GetLevel () + 0.05 * skill->GetT2 ());
                skill->SetPlus (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                (0.1 * skill->GetLevel () * skill->GetLevel () + 18 * skill->GetLevel () + 921) + INT (0.05 * skill->GetLevel () +
                                                                                                                       0.55) * (3 *
                                                                                                                                skill->GetLevel () *
                                                                                                                                skill->GetLevel () -
                                                                                                                                0.064 *
                                                                                                                                skill->GetLevel () +
                                                                                                                                974));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill528Stub ():SkillStub (528)
        {
            occupation = 16;
            name = L"±ÌÓ°";
            nativename = "±ÌÓ°";
            icon = "±ÌÓ°.dds";
            maxlevel = 20;
            maxlearn = 9;
            type = 1;
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
            talent[0] = 723;
            talent[1] = 731;
            talent[2] = 732;
            talent_size = 3;
            action[0] = "ºÏ»¶ÅÉ_±ÌÓ°";
            action[1] = "ºÏ»¶ÅÉ_±ÌÓ°";
            action[2] = "ºÏ»¶ÅÉ_±ÌÓ°";
            action[3] = "ºÏ»¶ÅÉ_±ÌÓ°";
            action[4] = "ºÏ»¶ÅÉ_±ÌÓ°";
            action[5] = "ºÏ»¶ÅÉ_±ÌÓ°";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_±ÌÓ°";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_±ÌÓ°";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_±ÌÓ°";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_±ÌÓ°";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_±ÌÓ°";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_±ÌÓ°";
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
        virtual ~ Skill528Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 0 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 24 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1.3 * skill->GetLevel () * skill->GetLevel () - 3.5 * skill->GetLevel () + 501);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 0;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 105, 107, 109, 111, 113, 115, 117, 119, 122 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (7);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (1.3 * skill->GetLevel () * skill->GetLevel () - 3.5 * skill->GetLevel () + 501),
                               22 + 3 * skill->GetLevel (),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (0.1 * skill->GetLevel () * skill->GetLevel () + 18 * skill->GetLevel () + 921) + INT (0.05 * skill->GetLevel () +
                                                                                                                           0.55) * (3 *
                                                                                                                                    skill->
                                                                                                                                    GetLevel () *
                                                                                                                                    skill->
                                                                                                                                    GetLevel () -
                                                                                                                                    0.064 *
                                                                                                                                    skill->
                                                                                                                                    GetLevel () +
                                                                                                                                    974)),
                               8 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (15 * skill->GetT0 () * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetTime (4000 * norm (skill->GetT0 ()));
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0.15 * skill->GetT0 () *
                                            (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                             (0.1 * skill->GetLevel () * skill->GetLevel () + 18 * skill->GetLevel () + 921) +
                                             INT (0.05 * skill->GetLevel () + 0.55) * (3 * skill->GetLevel () * skill->GetLevel () -
                                                                                       0.064 * skill->GetLevel () + 974)));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            return true;
        }
#endif
    };
}
#endif
