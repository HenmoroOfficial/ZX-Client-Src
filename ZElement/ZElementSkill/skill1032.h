#ifndef __CPPGEN_GNET_SKILL1032
#define __CPPGEN_GNET_SKILL1032
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1032:public Skill
    {
      public:
        enum
        { SKILL_ID = 1032 };
          Skill1032 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1032Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1500;
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
                return 500;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.1 * skill->GetT0 () + 0.06 * skill->GetLevel () +
                                 0.01 * INT (skill->GetPlayer ()->GetLevel () / (9 - skill->GetLevel ())));
                skill->SetPlus (215 * skill->GetLevel () + 0.03 * skill->GetT1 () * skill->GetPlayer ()->GetDef ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1032Stub ():SkillStub (1032)
        {
            occupation = 130;
            name = L"¼ÅÃðÌìÎè";
            nativename = "¼ÅÃðÌìÎè";
            icon = "¼ÅÃðÌìÎè.dds";
            maxlevel = 5;
            maxlearn = 5;
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
            skill_class = -2;
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
            preskillnum = 0;
            talent[0] = 670;
            talent[1] = 1582;
            talent_size = 2;
            action[0] = "ÌìÒôËÂ_·¨±¦_ÂÖ»ØÅÌ";
            action[1] = "ÌìÒôËÂ_·¨±¦_ÂÖ»ØÅÌ";
            action[2] = "ÌìÒôËÂ_·¨±¦_ÂÖ»ØÅÌ";
            action[3] = "ÌìÒôËÂ_·¨±¦_ÂÖ»ØÅÌ";
            action[4] = "ÌìÒôËÂ_·¨±¦_ÂÖ»ØÅÌ";
            action[5] = "ÌìÒôËÂ_·¨±¦_ÂÖ»ØÅÌ";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_·¨±¦_ÂÖ»ØÅÌ";
            action[16] = "ÌìÒôËÂ_Æï³Ë_·¨±¦_ÂÖ»ØÅÌ";
            action[17] = "ÌìÒôËÂ_Æï³Ë_·¨±¦_ÂÖ»ØÅÌ";
            action[18] = "ÌìÒôËÂ_Æï³Ë_·¨±¦_ÂÖ»ØÅÌ";
            action[19] = "ÌìÒôËÂ_Æï³Ë_·¨±¦_ÂÖ»ØÅÌ";
            action[20] = "ÌìÒôËÂ_Æï³Ë_·¨±¦_ÂÖ»ØÅÌ";
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
        virtual ~ Skill1032Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05 + 10);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (445 - 35 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6300 - 300 * skill->GetLevel ();
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05 + 5);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               445 - 35 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel (),
                               6 * skill->GetLevel (),
                               6.3 - 0.3 * skill->GetLevel (),
                               215 * skill->GetLevel (),
                               3 + 3 * skill->GetLevel (),
                               15 + 5 * skill->GetLevel (),
                               25 + 5 * skill->GetLevel (),
                               9 - skill->GetLevel (),
                               9 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel () * skill->GetLevel (),
                               4 + 4 * skill->GetLevel (),
                               125 + 5 * skill->GetLevel (),
                               3 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel (), 32 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (4000 * skill->GetLevel () + 4000 + 100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (9 * skill->GetLevel () + 5 * skill->GetT0 () +
                                           skill->GetLevel () * skill->GetLevel () * skill->GetLevel ());
            skill->GetVictim ()->SetSubanti (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (100);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount ((3 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel ()) * skill->GetPlayer ()->GetLevel () *
                                            norm ((125 + 5 * skill->GetLevel ()) / skill->GetPlayer ()->GetLevel ()));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (15 + 5 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (3000 + 3000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetRatio (0.25 + 0.05 * skill->GetT0 () + 0.05 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetSlow (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetValue (32 * skill->GetLevel () * (1 + skill->GetT0 ()));
            skill->GetVictim ()->SetJuqi (1);
            return true;
        }
#endif
    };
}
#endif
