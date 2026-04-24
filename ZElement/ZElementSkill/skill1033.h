#ifndef __CPPGEN_GNET_SKILL1033
#define __CPPGEN_GNET_SKILL1033
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1033:public Skill
    {
      public:
        enum
        { SKILL_ID = 1033 };
          Skill1033 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1033Stub:public SkillStub
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
                skill->SetCrit (0.01 * skill->GetT1 ());
                skill->SetRatio (1 + 0.06 * skill->GetLevel () + 0.08 * skill->GetT0 () +
                                 0.01 * INT (skill->GetPlayer ()->GetLevel () / (9 - skill->GetLevel ())) +
                                 0.16 * INT (skill->GetPlayer ()->GetMp () / 0.95 / skill->GetPlayer ()->GetMaxmp ()) * (1 + skill->GetT0 ()));
                skill->SetPlus (215 * skill->GetLevel () + 0.03 * skill->GetT2 () * skill->GetPlayer ()->GetDef ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1033Stub ():SkillStub (1033)
        {
            occupation = 130;
            name = L"³¾¹âÌ¾Ãî";
            nativename = "³¾¹âÌ¾Ãî";
            icon = "³¾¹âÌ¾Ãî.dds";
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
            talent[0] = 568;
            talent[1] = 599;
            talent[2] = 1582;
            talent_size = 3;
            action[0] = "ÇàÔÆÃÅ_·¨±¦_Ç¬À¤Ëø";
            action[1] = "ÇàÔÆÃÅ_·¨±¦_Ç¬À¤Ëø";
            action[2] = "ÇàÔÆÃÅ_·¨±¦_Ç¬À¤Ëø";
            action[3] = "ÇàÔÆÃÅ_·¨±¦_Ç¬À¤Ëø";
            action[4] = "ÇàÔÆÃÅ_·¨±¦_Ç¬À¤Ëø";
            action[5] = "ÇàÔÆÃÅ_·¨±¦_Ç¬À¤Ëø";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_·¨±¦_Ç¬À¤Ëø";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_·¨±¦_Ç¬À¤Ëø";
            action[17] = "ÇàÔÆÃÅ_Æï³Ë_·¨±¦_Ç¬À¤Ëø";
            action[18] = "ÇàÔÆÃÅ_Æï³Ë_·¨±¦_Ç¬À¤Ëø";
            action[19] = "ÇàÔÆÃÅ_Æï³Ë_·¨±¦_Ç¬À¤Ëø";
            action[20] = "ÇàÔÆÃÅ_Æï³Ë_·¨±¦_Ç¬À¤Ëø";
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
        virtual ~ Skill1033Stub ()
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
                               5 * skill->GetLevel (),
                               5 * skill->GetLevel (),
                               32 + skill->GetLevel () * skill->GetLevel (),
                               9 - skill->GetLevel (),
                               15 * skill->GetLevel (),
                               32 + skill->GetLevel () * skill->GetLevel (),
                               2 + 2 * skill->GetLevel (),
                               32 + skill->GetLevel () * skill->GetLevel (),
                               2 + 2 * skill->GetLevel (),
                               40 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel () * skill->GetLevel (), 15 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (32 + 3 * skill->GetT0 () + skill->GetLevel () * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (5000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetRatio (0.05 * skill->GetLevel () + 0.05 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecaccuracy (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (33 *
                                                  norm (0.15 * skill->GetLevel () * skill->GetPlayer ()->GetMaxhp () /
                                                        (skill->GetPlayer ()->GetHp () + 1.0))));
            skill->GetVictim ()->SetTime (2000 + 2000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetRatio (0.2 + 0.05 * skill->GetT0 () + 0.01 * skill->GetLevel () * skill->GetLevel ());
            skill->GetVictim ()->SetValue (0.2 + 0.05 * skill->GetT0 () + 0.01 * skill->GetLevel () * skill->GetLevel ());
            skill->GetVictim ()->SetRandcurse (1);
            skill->GetVictim ()->SetProbability (1.0 * (32 + 5 * skill->GetT0 () + skill->GetLevel () * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (2000 + 2000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->
                SetValue ((1 + norm (0.15 * skill->GetLevel () * skill->GetPlayer ()->GetMaxmp () / (skill->GetPlayer ()->GetMp () + 1.0))) *
                          (40 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel () * skill->GetLevel ()));
            skill->GetVictim ()->SetSubdefence (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetBuffid (INT (skill->GetPlayer ()->GetMp () / 0.95 / skill->GetPlayer ()->GetMaxmp ()));
            skill->GetVictim ()->SetUniqprompt (1);
            return true;
        }
#endif
    };
}
#endif
