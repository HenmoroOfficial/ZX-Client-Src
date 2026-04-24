#ifndef __CPPGEN_GNET_SKILL548
#define __CPPGEN_GNET_SKILL548
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill548:public Skill
    {
      public:
        enum
        { SKILL_ID = 548 };
          Skill548 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill548Stub:public SkillStub
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
      Skill548Stub ():SkillStub (548)
        {
            occupation = 22;
            name = L"¾»ÍÁÕæÑÔ";
            nativename = "¾»ÍÁÕæÑÔ";
            icon = "¾»ÍÁÕæÑÔ.dds";
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
            talent[1] = 718;
            talent[2] = 719;
            talent_size = 3;
            action[0] = "ÌìÒôËÂ_°ãÈô²¨ÂŞÃÛ";
            action[1] = "ÌìÒôËÂ_°ãÈô²¨ÂŞÃÛ";
            action[2] = "ÌìÒôËÂ_°ãÈô²¨ÂŞÃÛ";
            action[3] = "ÌìÒôËÂ_°ãÈô²¨ÂŞÃÛ";
            action[4] = "ÌìÒôËÂ_°ãÈô²¨ÂŞÃÛ";
            action[5] = "ÌìÒôËÂ_°ãÈô²¨ÂŞÃÛ";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_°ãÈô²¨ÂŞÃÛ";
            action[16] = "ÌìÒôËÂ_Æï³Ë_°ãÈô²¨ÂŞÃÛ";
            action[17] = "ÌìÒôËÂ_Æï³Ë_°ãÈô²¨ÂŞÃÛ";
            action[18] = "ÌìÒôËÂ_Æï³Ë_°ãÈô²¨ÂŞÃÛ";
            action[19] = "ÌìÒôËÂ_Æï³Ë_°ãÈô²¨ÂŞÃÛ";
            action[20] = "ÌìÒôËÂ_Æï³Ë_°ãÈô²¨ÂŞÃÛ";
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
        virtual ~ Skill548Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 546 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (22);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (230 * skill->GetLevel () + 556 - 1.9 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 30000 - 1000 * skill->GetT0 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[12] = { 106, 109, 112, 115, 118, 121, 124, 125, 125, 125, 125, 125 };
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
                               INT (230 * skill->GetLevel () + 556 - 1.9 * skill->GetLevel () * skill->GetLevel ()),
                               16,
                               2, INT (92 * skill->GetLevel () + 222.4 - 0.76 * skill->GetLevel () * skill->GetLevel ()), 15 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (16000);
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (4);
            skill->GetVictim ()->SetAmount (0.15 * skill->GetLevel () *
                                            (92 * skill->GetLevel () + 222.4 - 0.76 * skill->GetLevel () * skill->GetLevel ()) * (1 +
                                                                                                                                  0.15 *
                                                                                                                                  skill->GetT2 ()) *
                                            (1 + 0.4 * skill->GetT1 ()));
            skill->GetVictim ()->SetValue (8 * INT (0.4 * (230 * skill->GetLevel () + 556 - 1.9 * skill->GetLevel () * skill->GetLevel ())) *
                                           (1 + 0.15 * skill->GetT2 ()));
            skill->GetVictim ()->SetHpgen (1);
            skill->GetVictim ()->SetProbability (1.0 * (100 * INT (skill->GetPlayer ()->GetSkilllevel (277) / 12) * norm (skill->GetT2 ())));
            skill->GetVictim ()->SetTime (1800000 * norm (skill->GetT2 ()));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetValue (INT (skill->GetPlayer ()->GetSkilllevel (277) / 12) * 100 * skill->GetPlayer ()->GetSkilllevel (277) *
                                           skill->GetT2 ());
            skill->GetVictim ()->SetTiangu (1);
            skill->GetVictim ()->SetProbability (1.0 * (100 * INT (skill->GetPlayer ()->GetSkilllevel (790) / 4) * norm (skill->GetT2 ())));
            skill->GetVictim ()->SetTime (1800000 * norm (skill->GetT2 ()));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (5);
            skill->GetVictim ()->SetValue (INT (skill->GetPlayer ()->GetSkilllevel (790) / 4) * 100 * skill->GetPlayer ()->GetSkilllevel (790) *
                                           skill->GetT2 ());
            skill->GetVictim ()->SetTiangu (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (100 *
                                                  ((skill->GetPlayer ()->GetCultivation () ==
                                                    1 ? norm (skill->GetPlayer ()->GetSkilllevel (3028)) : 0) +
                                                   (skill->GetPlayer ()->GetCultivation () ==
                                                    2 ? norm (skill->GetPlayer ()->GetSkilllevel (3076)) : 0) +
                                                   (skill->GetPlayer ()->GetCultivation () ==
                                                    4 ? norm (skill->GetPlayer ()->GetSkilllevel (3124)) : 0)) * norm (skill->GetT2 ())));
            skill->GetVictim ()->SetTime (1800000 * norm (skill->GetT2 ()));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (5);
            skill->GetVictim ()->SetValue (100 *
                                           ((skill->GetPlayer ()->GetCultivation () ==
                                             1 ? skill->GetPlayer ()->GetSkilllevel (3028) : 0) + (skill->GetPlayer ()->GetCultivation () ==
                                                                                                   2 ? skill->GetPlayer ()->
                                                                                                   GetSkilllevel (3076) : 0) +
                                            (skill->GetPlayer ()->GetCultivation () ==
                                             4 ? skill->GetPlayer ()->GetSkilllevel (3124) : 0)) * skill->GetT2 ());
            skill->GetVictim ()->SetTiangu (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100 * norm (skill->GetT2 ())));
            skill->GetVictim ()->SetTime (1800000 * norm (skill->GetT2 ()));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (4);
            skill->GetVictim ()->SetValue (100 * skill->GetLevel () * skill->GetT2 () * int (skill->GetLevel () / 12));
              skill->GetVictim ()->SetTiangu (1);
              skill->GetVictim ()->SetProbability (1.0 * (100 * INT (skill->GetPlayer ()->GetSkilllevel (273) / 12) * norm (skill->GetT2 ())));
              skill->GetVictim ()->SetTime (1800000 * norm (skill->GetT2 ()));
              skill->GetVictim ()->SetRatio (0);
              skill->GetVictim ()->SetBuffid (2);
              skill->GetVictim ()->SetValue (INT (skill->GetPlayer ()->GetSkilllevel (273) / 12) * 100 * skill->GetPlayer ()->GetSkilllevel (273) *
                                             skill->GetT2 ());
              skill->GetVictim ()->SetTiangu (1);
              skill->GetVictim ()->SetProbability (1.0 * (100 * INT (skill->GetPlayer ()->GetSkilllevel (266) / 12) * norm (skill->GetT2 ())));
              skill->GetVictim ()->SetTime (1800000 * norm (skill->GetT2 ()));
              skill->GetVictim ()->SetRatio (0);
              skill->GetVictim ()->SetBuffid (1);
              skill->GetVictim ()->SetValue (INT (skill->GetPlayer ()->GetSkilllevel (266) / 12) * 100 * skill->GetPlayer ()->GetSkilllevel (266) *
                                             skill->GetT2 ());
              skill->GetVictim ()->SetTiangu (1);
              return true;
        }
#endif
    };
}
#endif
