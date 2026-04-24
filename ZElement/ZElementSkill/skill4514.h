#ifndef __CPPGEN_GNET_SKILL4514
#define __CPPGEN_GNET_SKILL4514
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4514:public Skill
    {
      public:
        enum
        { SKILL_ID = 4514 };
          Skill4514 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4514Stub:public SkillStub
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
                skill->SetRatio (1 + 0.03 * skill->GetT0 () + 0.05 * skill->GetLevel () +
                                 0.01 * INT (skill->GetPlayer ()->GetLevel () / (9 - skill->GetLevel ())));
                skill->SetPlus (205 * skill->GetLevel () + 0.03 * skill->GetT1 () * skill->GetPlayer ()->GetDef ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4514Stub ():SkillStub (4514)
        {
            occupation = 130;
            name = L"³Ğ¡¤´ó·²Èô¼ò";
            nativename = "³Ğ¡¤´ó·²Èô¼ò";
            icon = "Í¨ÓÃ04.dds";
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
            talent[0] = 687;
            talent[1] = 1582;
            talent_size = 2;
            action[0] = "¹íµÀ_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[1] = "ÇàÔÆÃÅ_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[2] = "¹íÍõ×Ú_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[3] = "ºÏ»¶ÅÉ_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[4] = "ÌìÒôËÂ_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[5] = "¹íµÀ_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[6] = "·ÙÏã¹È_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[7] = "ÊŞÉñ_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[8] = "ºüÑı_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[9] = "ĞùÔ¯_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[10] = "»³¹â_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[11] = "Ì«ê»_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[12] = "Ìì»ª_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[13] = "ÁéÙí_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[14] = "Ó¢ÕĞ_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[15] = "¹íµÀ_Æï³Ë_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[17] = "¹íÍõ×Ú_Æï³Ë_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[19] = "ÌìÒôËÂ_Æï³Ë_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[20] = "¹íµÀ_Æï³Ë_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[21] = "·ÙÏã¹È_Æï³Ë_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[22] = "ÊŞÉñ_Æï³Ë_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[23] = "ºüÑı_Æï³Ë_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[24] = "ĞùÔ¯_Æï³Ë_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[25] = "»³¹â_Æï³Ë_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[26] = "Ì«ê»_Æï³Ë_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[27] = "Ìì»ª_Æï³Ë_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[28] = "ÁéÙí_Æï³Ë_·¨±¦_ÈÕÔÂ±¦¼ø";
            action[29] = "Ó¢ÕĞ_Æï³Ë_·¨±¦_ÈÕÔÂ±¦¼ø";
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
        virtual ~ Skill4514Stub ()
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
            return (float) (315 - 25 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel ());
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
                               315 - 25 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel (),
                               6.3 - 0.3 * skill->GetLevel (),
                               5 * skill->GetLevel (),
                               205 * skill->GetLevel (),
                               4 + 2 * skill->GetLevel (),
                               28 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel (),
                               9 - skill->GetLevel (),
                               10,
                               2 * skill->GetLevel (),
                               2 * skill->GetLevel (),
                               11 + 2 * skill->GetLevel (),
                               5 + 5 * skill->GetLevel (), 35 + 25 * skill->GetLevel () * skill->GetLevel (), 2 + 2 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (4000 + 2000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (28 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
            skill->GetVictim ()->SetSubattack (1);
            skill->GetVictim ()->SetProbability (1.0 * (33));
            skill->GetVictim ()->SetTime (10000 + 100);
            skill->GetVictim ()->SetRatio (0.02001 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDeccritrate (1);
            skill->GetVictim ()->SetProbability (1.0 * (norm (zrand (100) / 94) * 600));
            skill->GetVictim ()->SetTime (2000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetDizzy (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (5 + 5 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (12000 + 2000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetAmount (2 + 2 * skill->GetLevel ());
            skill->GetVictim ()->SetValue (35 + 25 * skill->GetLevel () * skill->GetLevel ());
            skill->GetVictim ()->SetHpregain (1);
            return true;
        }
#endif
    };
}
#endif
