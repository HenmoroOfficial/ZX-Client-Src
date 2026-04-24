#ifndef __CPPGEN_GNET_SKILL277
#define __CPPGEN_GNET_SKILL277
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill277:public Skill
    {
      public:
        enum
        { SKILL_ID = 277 };
          Skill277 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill277Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 ((skill->GetPlayer ()->GetMp () - skill->GetPlayer ()->GetMaxmp () * 0.04 * skill->GetLevel ()) >
                                              0 ? 1 : 0);
                skill->GetPlayer ()->SetMp ((skill->GetPlayer ()->GetMp () - skill->GetPlayer ()->GetMaxmp () * 0.04 * skill->GetLevel ()) >
                                            0 ? (skill->GetPlayer ()->GetMp () -
                                                 skill->GetPlayer ()->GetMaxmp () * 0.04 * skill->GetLevel ()) : skill->GetPlayer ()->GetMp ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill277Stub ():SkillStub (277)
        {
            occupation = 12;
            name = L"·ğ¹âÆÕÕÕ";
            nativename = "·ğ¹âÆÕÕÕ";
            icon = "·ğ¹âÆÕÕÕ.dds";
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
            talent[1] = 710;
            talent[2] = 715;
            talent[3] = 719;
            talent_size = 4;
            action[0] = "ÌìÒôËÂ_·ğ¹âÆÕÕÕ";
            action[1] = "ÌìÒôËÂ_·ğ¹âÆÕÕÕ";
            action[2] = "ÌìÒôËÂ_·ğ¹âÆÕÕÕ";
            action[3] = "ÌìÒôËÂ_·ğ¹âÆÕÕÕ";
            action[4] = "ÌìÒôËÂ_·ğ¹âÆÕÕÕ";
            action[5] = "ÌìÒôËÂ_·ğ¹âÆÕÕÕ";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_·ğ¹âÆÕÕÕ";
            action[16] = "ÌìÒôËÂ_Æï³Ë_·ğ¹âÆÕÕÕ";
            action[17] = "ÌìÒôËÂ_Æï³Ë_·ğ¹âÆÕÕÕ";
            action[18] = "ÌìÒôËÂ_Æï³Ë_·ğ¹âÆÕÕÕ";
            action[19] = "ÌìÒôËÂ_Æï³Ë_·ğ¹âÆÕÕÕ";
            action[20] = "ÌìÒôËÂ_Æï³Ë_·ğ¹âÆÕÕÕ";
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
        virtual ~ Skill277Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 282 };
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
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 60000 - 1000 * skill->GetT0 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[12] = { 76, 78, 80, 83, 86, 89, 92, 95, 98, 102, 105, 108 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (6 + skill->GetLevel ());
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (6 + skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (2 + skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 4 * skill->GetLevel (), 5 + skill->GetLevel (), 4 * skill->GetLevel (), skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (100 * skill->GetPlayer ()->GetVar1 ()));
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetValue ((1 + 0.2 * (1 + skill->GetT2 ()) * norm (zrand (121) / 100)) *
                                           ((0.04 * skill->GetLevel () + 0.06 * skill->GetT2 ()) * skill->GetPlayer ()->GetMaxmp ()) * (1 +
                                                                                                                                        0.15 *
                                                                                                                                        skill->
                                                                                                                                        GetT3 ()));
            skill->GetVictim ()->SetHeal (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (8000);
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetAmount (0.02 * skill->GetT1 () * skill->GetPlayer ()->GetMp ());
            skill->GetVictim ()->SetValue ((0.04 * skill->GetLevel () * skill->GetPlayer ()->GetMaxmp ()) * (1 + 0.15 * skill->GetT3 ()));
            skill->GetVictim ()->SetHpgen (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (1800000 * norm (skill->GetT3 ()));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetValue (100 * skill->GetLevel () * skill->GetT3 ());
            skill->GetVictim ()->SetTiangu (1);
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
