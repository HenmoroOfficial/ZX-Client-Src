#ifndef __CPPGEN_GNET_SKILL4473
#define __CPPGEN_GNET_SKILL4473
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4473:public Skill
    {
      public:
        enum
        { SKILL_ID = 4473 };
          Skill4473 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4473Stub:public SkillStub
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
                skill->SetRatio (1 + 0.02 * skill->GetLevel () + 0.01 * INT (skill->GetPlayer ()->GetLevel () / (12 - skill->GetLevel ())));
                skill->SetPlus (15 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4473Stub ():SkillStub (4473)
        {
            occupation = 130;
            name = L"³Ğ¡¤ÂÊ¶µÉñ»ğ";
            nativename = "³Ğ¡¤ÂÊ¶µÉñ»ğ";
            icon = "Òµ»ğ.dds";
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
            action[0] = "ĞùÔ¯_·¨±¦_ÂÊ¶µÆì";
            action[1] = "·ÙÏã¹È_·¨±¦_ÄÏÃ÷Ó¡";
            action[2] = "·ÙÏã¹È_·¨±¦_ÄÏÃ÷Ó¡";
            action[3] = "·ÙÏã¹È_·¨±¦_ÄÏÃ÷Ó¡";
            action[4] = "·ÙÏã¹È_·¨±¦_ÄÏÃ÷Ó¡";
            action[5] = "·ÙÏã¹È_·¨±¦_ÄÏÃ÷Ó¡";
            action[6] = "·ÙÏã¹È_·¨±¦_ÄÏÃ÷Ó¡";
            action[7] = "ÊŞÉñ_·¨±¦_Å­ÊŞá¦";
            action[8] = "ºüÑı_·¨±¦_µ¤Öì";
            action[9] = "ĞùÔ¯_·¨±¦_ÂÊ¶µÆì";
            action[10] = "»³¹â_·¨±¦_ÆßÉ±";
            action[11] = "Ì«ê»_·¨±¦_À×¹«Ä«";
            action[12] = "Ìì»ª_·¨±¦_¾ÅÒôÂİ";
            action[13] = "01";
            action[14] = "01";
            action[15] = "ĞùÔ¯_Æï³Ë__·¨±¦_ÂÊ¶µÆì";
            action[16] = "·ÙÏã¹È_Æï³Ë_·¨±¦_ÄÏÃ÷Ó¡";
            action[17] = "·ÙÏã¹È_Æï³Ë_·¨±¦_ÄÏÃ÷Ó¡";
            action[18] = "·ÙÏã¹È_Æï³Ë_·¨±¦_ÄÏÃ÷Ó¡";
            action[19] = "·ÙÏã¹È_Æï³Ë_·¨±¦_ÄÏÃ÷Ó¡";
            action[20] = "·ÙÏã¹È_Æï³Ë_·¨±¦_ÄÏÃ÷Ó¡";
            action[21] = "·ÙÏã¹È_Æï³Ë_·¨±¦_ÄÏÃ÷Ó¡";
            action[22] = "ÊŞÉñ_Æï³Ë_·¨±¦_Å­ÊŞá¦";
            action[23] = "ºüÑı_Æï³Ë_·¨±¦_µ¤Öì";
            action[24] = "ĞùÔ¯_Æï³Ë__·¨±¦_ÂÊ¶µÆì";
            action[25] = "»³¹â_Æï³Ë_·¨±¦_ÆßÉ±";
            action[26] = "Ì«ê»_Æï³Ë_·¨±¦_À×¹«Ä«";
            action[27] = "Ìì»ª_Æï³Ë_·¨±¦_¾ÅÒôÂİ";
            action[28] = "01";
            action[29] = "01";
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
        virtual ~ Skill4473Stub ()
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
            return (float) (63 - 3 * skill->GetLevel ());
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
                               63 - 3 * skill->GetLevel (),
                               6.3 - 0.3 * skill->GetLevel (),
                               2 * skill->GetLevel (),
                               15 * skill->GetLevel (), 12 - skill->GetLevel (), 5 + skill->GetLevel (), 32 + 12 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (5 + skill->GetLevel ()));
            skill->GetVictim ()->SetTime (6100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount ((32 + 12 * skill->GetLevel ()) * 3);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            return true;
        }
#endif
    };
}
#endif
