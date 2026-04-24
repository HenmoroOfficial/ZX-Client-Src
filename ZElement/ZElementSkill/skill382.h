#ifndef __CPPGEN_GNET_SKILL382
#define __CPPGEN_GNET_SKILL382
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill382:public Skill
    {
      public:
        enum
        { SKILL_ID = 382 };
          Skill382 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill382Stub:public SkillStub
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
      Skill382Stub ():SkillStub (382)
        {
            occupation = 11;
            name = L"ËÄ´ó½Ô¿Õ£º»ð";
            nativename = "ËÄ´ó½Ô¿Õ£º»ð";
            icon = "ËÄ´ó½Ô¿Õ£º»ð.dds";
            maxlevel = 20;
            maxlearn = 8;
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
            talent[0] = 679;
            talent[1] = 680;
            talent[2] = 691;
            talent_size = 3;
            action[0] = "ÌìÒôËÂ_ËÄ´ó½Ô¿Õ»ð";
            action[1] = "ÌìÒôËÂ_ËÄ´ó½Ô¿Õ»ð";
            action[2] = "ÌìÒôËÂ_ËÄ´ó½Ô¿Õ»ð";
            action[3] = "ÌìÒôËÂ_ËÄ´ó½Ô¿Õ»ð";
            action[4] = "ÌìÒôËÂ_ËÄ´ó½Ô¿Õ»ð";
            action[5] = "ÌìÒôËÂ_ËÄ´ó½Ô¿Õ»ð";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_ËÄ´ó½Ô¿Õ»ð";
            action[16] = "ÌìÒôËÂ_Æï³Ë_ËÄ´ó½Ô¿Õ»ð";
            action[17] = "ÌìÒôËÂ_Æï³Ë_ËÄ´ó½Ô¿Õ»ð";
            action[18] = "ÌìÒôËÂ_Æï³Ë_ËÄ´ó½Ô¿Õ»ð";
            action[19] = "ÌìÒôËÂ_Æï³Ë_ËÄ´ó½Ô¿Õ»ð";
            action[20] = "ÌìÒôËÂ_Æï³Ë_ËÄ´ó½Ô¿Õ»ð";
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
            rangetype = 5;
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
        virtual ~ Skill382Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 396 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (3 * skill->GetLevel () * skill->GetLevel () + 2.21 * skill->GetLevel () + 228);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 4000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[8] = { 55, 58, 61, 64, 67, 70, 73, 77 };
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
                               INT (3 * skill->GetLevel () * skill->GetLevel () + 2.21 * skill->GetLevel () + 228), 30, 8 * skill->GetLevel () + 8);

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
            skill->GetVictim ()->SetTime (1800000 + 900000 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue ((8 * skill->GetLevel () + 8) * (1 + 0.4 * skill->GetT1 ()));
            skill->GetVictim ()->SetAddantiweak (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetValue ((3 * skill->GetLevel () * skill->GetLevel () + 2.21 * skill->GetLevel () + 228) * 0.3 * skill->GetT0 ());
            skill->GetVictim ()->SetHeal (1);
            skill->GetVictim ()->SetProbability (1.0 * (8 * skill->GetT2 ()));
            skill->GetVictim ()->SetTime (15050 * skill->GetT2 ());
            skill->GetVictim ()->SetRatio (0.04 * skill->GetT2 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetIncattack (1);
            skill->GetVictim ()->SetProbability (1.0 * (100 * norm (skill->GetT1 ())));
            skill->GetVictim ()->SetTime (1800000);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetValue (0.25 * skill->GetT1 () * skill->GetPlayer ()->GetLevel () + 1 +
                                           (skill->GetPlayer ()->GetSkilllevel (382) + skill->GetPlayer ()->GetSkilllevel (383) +
                                            skill->GetPlayer ()->GetSkilllevel (384) +
                                            skill->GetPlayer ()->GetSkilllevel (385)) * skill->GetT1 () * 4);
            skill->GetVictim ()->SetAddattack3 (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (skill->GetPlayer ()->GetHasbuff (4511) ==
                                          1 ? 1 : (1800000 +
                                                   900000 * skill->GetT0 ()) * norm (skill->GetT1 ()) *
                                          norm (skill->GetPlayer ()->GetSkilllevel (383)));
            skill->GetVictim ()->SetBuffid (skill->GetPlayer ()->GetHasbuff (4511) == 1 ? 0 : 1);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetHasbuff (4511) ==
                                           1 ? 0.00001 : (8 * skill->GetPlayer ()->GetSkilllevel (383) + 8) * (1 + 0.4 * skill->GetT1 ()));
            skill->GetVictim ()->SetAddantisilent (1);
            skill->GetVictim ()->SetTime (skill->GetPlayer ()->GetHasbuff (4521) ==
                                          1 ? 1 : (1800000 +
                                                   900000 * skill->GetT0 ()) * norm (skill->GetT1 ()) *
                                          norm (skill->GetPlayer ()->GetSkilllevel (384)));
            skill->GetVictim ()->SetBuffid (skill->GetPlayer ()->GetHasbuff (4521) == 1 ? 0 : 1);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetHasbuff (4521) ==
                                           1 ? 0.00001 : (8 * skill->GetPlayer ()->GetSkilllevel (384) + 8) * (1 + 0.4 * skill->GetT1 ()));
            skill->GetVictim ()->SetAddantiwrap (1);
            skill->GetVictim ()->SetTime (skill->GetPlayer ()->GetHasbuff (4481) ==
                                          1 ? 1 : (1800000 +
                                                   900000 * skill->GetT0 ()) * norm (skill->GetT1 ()) *
                                          norm (skill->GetPlayer ()->GetSkilllevel (385)));
            skill->GetVictim ()->SetBuffid (skill->GetPlayer ()->GetHasbuff (4481) == 1 ? 0 : 1);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetHasbuff (4481) ==
                                           1 ? 0.00001 : (8 * skill->GetPlayer ()->GetSkilllevel (385) + 8) * (1 + 0.4 * skill->GetT1 ()));
            skill->GetVictim ()->SetAddantidizzy (1);
            return true;
        }
#endif
    };
}
#endif
