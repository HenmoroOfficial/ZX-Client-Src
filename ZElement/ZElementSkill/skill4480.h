#ifndef __CPPGEN_GNET_SKILL4480
#define __CPPGEN_GNET_SKILL4480
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4480:public Skill
    {
      public:
        enum
        { SKILL_ID = 4480 };
          Skill4480 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4480Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1525;
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
                skill->SetRatio (1 + 0.02 * skill->GetLevel () + 0.01 * INT (skill->GetPlayer ()->GetLevel () / (11 - skill->GetLevel ())));
                skill->SetPlus (45 * skill->GetLevel ());
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
                skill->SetRatio (1 + 0.02 * skill->GetLevel () + 0.01 * INT (skill->GetPlayer ()->GetLevel () / (11 - skill->GetLevel ())));
                skill->SetPlus (45 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill4480Stub ():SkillStub (4480)
        {
            occupation = 130;
            name = L"³Ğ¡¤·ãÁÖÉ·Ñª";
            nativename = "³Ğ¡¤·ãÁÖÉ·Ñª";
            icon = "·ãÁÖÉ·Ñª.dds";
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
            action[0] = "ÊŞÉñ_·¨±¦_Ñª·ãÇ¹";
            action[1] = "ÇàÔÆÃÅ_·¨±¦_ÆßĞÇ½£";
            action[2] = "ÇàÔÆÃÅ_·¨±¦_ÆßĞÇ½£";
            action[3] = "ÇàÔÆÃÅ_·¨±¦_ÆßĞÇ½£";
            action[4] = "ÇàÔÆÃÅ_·¨±¦_ÆßĞÇ½£";
            action[5] = "ÇàÔÆÃÅ_·¨±¦_ÆßĞÇ½£";
            action[6] = "ÇàÔÆÃÅ_·¨±¦_ÆßĞÇ½£";
            action[7] = "ÊŞÉñ_·¨±¦_Ñª·ãÇ¹";
            action[8] = "ºüÑı_·¨±¦_·µ»êÏã";
            action[9] = "ĞùÔ¯_·¨±¦_ÄõÁú¹Ä";
            action[10] = "»³¹â_·¨±¦_ÆÆ¾ü";
            action[11] = "Ì«ê»_·¨±¦_´ºÇïËø";
            action[12] = "Ìì»ª_·¨±¦_ÑÇ·ç";
            action[13] = "01";
            action[14] = "01";
            action[15] = "ÊŞÉñ_Æï³Ë_·¨±¦_Ñª·ãÇ¹";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_·¨±¦_ÆßĞÇ½£";
            action[17] = "ÇàÔÆÃÅ_Æï³Ë_·¨±¦_ÆßĞÇ½£";
            action[18] = "ÇàÔÆÃÅ_Æï³Ë_·¨±¦_ÆßĞÇ½£";
            action[19] = "ÇàÔÆÃÅ_Æï³Ë_·¨±¦_ÆßĞÇ½£";
            action[20] = "ÇàÔÆÃÅ_Æï³Ë_·¨±¦_ÆßĞÇ½£";
            action[21] = "ÇàÔÆÃÅ_Æï³Ë_·¨±¦_ÆßĞÇ½£";
            action[22] = "ÊŞÉñ_Æï³Ë_·¨±¦_Ñª·ãÇ¹";
            action[23] = "ºüÑı_Æï³Ë_·¨±¦_·µ»êÏã";
            action[24] = "ĞùÔ¯_Æï³Ë_·¨±¦_ÄõÁú¹Ä";
            action[25] = "»³¹â_Æï³Ë_·¨±¦_ÆÆ¾ü";
            action[26] = "Ì«ê»_Æï³Ë_·¨±¦_´ºÇïËø";
            action[27] = "Ìì»ª_Æï³Ë_·¨±¦_ÑÇ·ç";
            action[28] = "01";
            action[29] = "01";
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
            statestub.push_back (new State3 ());
#endif
        }
        virtual ~ Skill4480Stub ()
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
            return (float) (185 - 15 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel ());
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
                               185 - 15 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel (),
                               6.3 - 0.3 * skill->GetLevel (),
                               2 * skill->GetLevel (),
                               45 * skill->GetLevel (),
                               11 - skill->GetLevel (),
                               6 + 2 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel (),
                               7 + 2 * skill->GetLevel (),
                               skill->GetLevel () + INT (skill->GetLevel () / 4) + 2 * INT (skill->GetLevel () / 5),
                               2 + 2 * skill->GetLevel (),
                               6 + 12 * skill->GetLevel (), skill->GetLevel () + INT (skill->GetLevel () / 4) + 2 * INT (skill->GetLevel () / 5));

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
            skill->GetVictim ()->SetTime (2000 + 2000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetAmount (skill->GetLevel () + norm (skill->GetLevel () / 3.9) + 2 * norm (skill->GetLevel () / 4.9));
            skill->GetVictim ()->SetValue (6 + 12 * skill->GetLevel ());
            skill->GetVictim ()->SetCursed (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (7000 + 2000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetAmount (skill->GetLevel () + norm (skill->GetLevel () / 3.9) + 2 * norm (skill->GetLevel () / 4.9));
            skill->GetVictim ()->SetValue (6 + 2 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel ());
            skill->GetVictim ()->SetFrenzied (1);
            return true;
        }
#endif
    };
}
#endif
