#ifndef __CPPGEN_GNET_SKILL4971
#define __CPPGEN_GNET_SKILL4971
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4971:public Skill
    {
      public:
        enum
        { SKILL_ID = 4971 };
          Skill4971 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4971Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1225;
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
                return 300;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes1 ());
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetMaxatk ());
                skill->GetPlayer ()->SetVar3 (1);
                skill->SetRatio (1 + 0.02 * skill->GetLevel () + 0.01 * INT (skill->GetPlayer ()->GetLevel () / (10 - skill->GetLevel ())));
                skill->SetPlus (50 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State3:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 475;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes1 ());
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetMaxatk ());
                skill->GetPlayer ()->SetVar3 (0);
                skill->SetRatio (1 + 0.02 * skill->GetLevel () + 0.01 * INT (skill->GetPlayer ()->GetLevel () / (10 - skill->GetLevel ())));
                skill->SetPlus (50 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill4971Stub ():SkillStub (4971)
        {
            occupation = 130;
            name = L"³Ğ¡¤Ê¥ÊŞ¿ñÀŞ";
            nativename = "³Ğ¡¤Ê¥ÊŞ¿ñÀŞ";
            icon = "Ê¥ÊŞÀŞ.dds";
            maxlevel = 5;
            maxlearn = 5;
            type = 1;
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
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
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
            action[0] = "Ìì»ª_·¨±¦_ÁğÁ§Õµ";
            action[1] = "01";
            action[2] = "01";
            action[3] = "01";
            action[4] = "01";
            action[5] = "01";
            action[6] = "01";
            action[7] = "01";
            action[8] = "01";
            action[9] = "01";
            action[10] = "01";
            action[11] = "01";
            action[12] = "Ìì»ª_·¨±¦_ÁğÁ§Õµ";
            action[13] = "01";
            action[14] = "Ó¢ÕĞ_·¨±¦_ÂÓÓ°";
            action[15] = "Ìì»ª_Æï³Ë_·¨±¦_ÁğÁ§Õµ";
            action[16] = "01";
            action[17] = "01";
            action[18] = "01";
            action[19] = "01";
            action[20] = "01";
            action[21] = "01";
            action[22] = "01";
            action[23] = "01";
            action[24] = "01";
            action[25] = "01";
            action[26] = "01";
            action[27] = "Ìì»ª_Æï³Ë_·¨±¦_ÁğÁ§Õµ";
            action[28] = "01";
            action[29] = "Ó¢ÕĞ_Æï³Ë_·¨±¦_ÂÓÓ°";
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
            statestub.push_back (new State3 ());
#endif
        }
        virtual ~ Skill4971Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetHpcost (Skill * skill) const
        {
            return (int) (295 - 25 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel ());
        }
        int GetInkcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05 + 10);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6300 - 300 * skill->GetLevel ();
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (skill->GetLevel () == 5 ? 8 : (1 + skill->GetLevel ()));
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
                               295 - 25 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel (),
                               6.3 - 0.3 * skill->GetLevel (),
                               2 * skill->GetLevel (),
                               50 * skill->GetLevel (),
                               10 - skill->GetLevel (),
                               2 + 0.5 * skill->GetLevel (), 30 * skill->GetLevel (), 5 + 3 * skill->GetLevel (), 10 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 ()));
            skill->GetVictim ()->SetTime (1000 * (2 + 0.5 * skill->GetLevel ()));
            skill->GetVictim ()->SetDizzy (1);
            skill->GetVictim ()->SetTime (5000 + skill->GetLevel () * 3000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (30 * skill->GetLevel ());
            skill->GetVictim ()->SetSubantidizzy (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar3 () * (INT (zrand (100)) < 10 * skill->GetLevel ()? 100 : 0)));
            skill->GetVictim ()->
                SetValue ((skill->GetPlayer ()->GetVar2 () *
                           (1 + 0.03 * skill->GetLevel () + 0.01 * INT (skill->GetPlayer ()->GetLevel () / (10 - skill->GetLevel ()))) +
                           50 * skill->GetLevel ()) >
                          skill->GetPlayer ()->GetDef ()? (skill->GetPlayer ()->GetVar2 () *
                                                           (1 + 0.03 * skill->GetLevel () +
                                                            0.01 * INT (skill->GetPlayer ()->GetLevel () / (10 - skill->GetLevel ()))) +
                                                           50 * skill->GetLevel () - skill->GetPlayer ()->GetDef ()) : 0);
            skill->GetVictim ()->SetDirecthurt (1);
            return true;
        }
#endif
    };
}
#endif
