#ifndef __CPPGEN_GNET_SKILL4504
#define __CPPGEN_GNET_SKILL4504
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4504:public Skill
    {
      public:
        enum
        { SKILL_ID = 4504 };
          Skill4504 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4504Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1700;
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
                skill->GetPlayer ()->SetVar11 (skill->GetPlayer ()->GetOccupation ());
                skill->SetRatio (1 + 0.01 * INT (skill->GetPlayer ()->GetLevel () / (9 - skill->GetLevel ())));
                skill->SetPlus (145 * skill->GetLevel ());
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
                return 275;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar11 (skill->GetPlayer ()->GetOccupation ());
                skill->SetRatio (1 + 0.01 * INT (skill->GetPlayer ()->GetLevel () / (9 - skill->GetLevel ())));
                skill->SetPlus (145 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State4:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 325;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar11 (skill->GetPlayer ()->GetOccupation ());
                skill->GetPlayer ()->SetVar1 (1);
                skill->SetCrit (0.02 * skill->GetLevel () + 0.03 * INT (skill->GetLevel () / 4.9));
                skill->SetCrithurt (0.1 * skill->GetLevel () + 0.01 * skill->GetLevel () * skill->GetLevel ());
                skill->SetRatio (1 + 0.06 * skill->GetLevel () + 0.01 * INT (skill->GetPlayer ()->GetLevel () / (9 - skill->GetLevel ())));
                skill->SetPlus (145 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill4504Stub ():SkillStub (4504)
        {
            occupation = 130;
            name = L"³Ğ¡¤Íö»êÉ¥ÆÇ";
            nativename = "³Ğ¡¤Íö»êÉ¥ÆÇ";
            icon = "Íö»êÉ¥ÆÇ.dds";
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
            action[0] = "ÊŞÉñ_·¨±¦_É¥ÃÅ½£";
            action[1] = "ÇàÔÆÃÅ_·¨±¦_Ç¬À¤Ëø";
            action[2] = "¹íÍõ×Ú_·¨±¦_ÓÄÚ¤¹íÊÖ";
            action[3] = "ÌìÒôËÂ_·¨±¦_ÂÖ»ØÅÌ";
            action[4] = "ºÏ»¶ÅÉ_·¨±¦_³à½ğç±";
            action[5] = "¹íµÀ_·¨±¦_¹ííà";
            action[6] = "·ÙÏã¹È_·¨±¦_¾Åº®Äı±ù´Ì";
            action[7] = "ÊŞÉñ_·¨±¦_É¥ÃÅ½£";
            action[8] = "ºüÑı_·¨±¦_ÂåÊé";
            action[9] = "ĞùÔ¯_·¨±¦_³ĞÓ°";
            action[10] = "»³¹â_·¨±¦_¾øÓ°";
            action[11] = "Ì«ê»_·¨±¦_ôË»ÊÖÓ";
            action[12] = "Ìì»ª_·¨±¦_±ÌÂä·Éºè";
            action[13] = "1";
            action[14] = "1";
            action[15] = "ÊŞÉñ_Æï³Ë_·¨±¦_É¥ÃÅ½£";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_·¨±¦_Ç¬À¤Ëø";
            action[17] = "¹íÍõ×Ú_Æï³Ë_·¨±¦_ÓÄÚ¤¹íÊÖ";
            action[18] = "ÌìÒôËÂ_Æï³Ë_·¨±¦_ÂÖ»ØÅÌ";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_·¨±¦_³à½ğç±";
            action[20] = "¹íµÀ_Æï³Ë_·¨±¦_¹ííà";
            action[21] = "·ÙÏã¹È_Æï³Ë_·¨±¦_¾Åº®Äı±ù´Ì";
            action[22] = "ÊŞÉñ_Æï³Ë_·¨±¦_É¥ÃÅ½£";
            action[23] = "ºüÑı_Æï³Ë_·¨±¦_ÂåÊé";
            action[24] = "ĞùÔ¯_Æï³Ë_·¨±¦_³ĞÓ°";
            action[25] = "»³¹â_Æï³Ë_·¨±¦_¾øÓ°";
            action[26] = "Ì«ê»_Æï³Ë_·¨±¦_ôË»ÊÖÓ";
            action[27] = "Ìì»ª_Æï³Ë_·¨±¦_±ÌÂä·Éºè";
            action[28] = "1";
            action[29] = "1";
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
            statestub.push_back (new State4 ());
#endif
        }
        virtual ~ Skill4504Stub ()
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
            return (float) (745 - 35 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2500;
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
                               745 - 35 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel (),
                               6.3 - 0.3 * skill->GetLevel (),
                               6 * skill->GetLevel (),
                               2 * skill->GetLevel () + 3 * INT (skill->GetLevel () / 5),
                               10 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel (),
                               145 * skill->GetLevel (),
                               9 - skill->GetLevel (),
                               2 + 0.5 * skill->GetLevel (),
                               3 + skill->GetLevel (),
                               50,
                               4,
                               2 * skill->GetLevel () + 5 * INT (skill->GetLevel () / 5),
                               48 + 36 * skill->GetLevel () + 36 * INT (skill->GetLevel () / 5),
                               2 * skill->GetLevel () + INT (skill->GetLevel () / 4) + 2 * INT (skill->GetLevel () / 5));

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
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetVar1 () == 1 ? norm ((zrand (25) + skill->GetLevel ()) / 25) * 600 : 0));
            skill->GetVictim ()->SetTime (8000 + 100);
            skill->GetVictim ()->SetRatio (0.5);
            skill->GetVictim ()->SetBuffid (4);
            skill->GetVictim ()->SetSlow (1);
            skill->GetVictim ()->SetTime (2000 * skill->GetLevel () + 5000 * norm (skill->GetLevel () / 4.9) + 100);
            skill->GetVictim ()->SetAmount (2 * skill->GetLevel () + norm (skill->GetLevel () / 3.9) + 4 * norm (skill->GetLevel () / 4.9));
            skill->GetVictim ()->SetValue (48 + 36 * skill->GetLevel () + 36 * norm (skill->GetLevel () / 4.9));
            skill->GetVictim ()->SetCursed (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetRatio (1853);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetClearcooldown (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetVar1 () == 1 ? (skill->GetPlayer ()->GetVar11 () == 37 ? 25 : 20) : 0));
            skill->GetVictim ()->SetTime (3000 + 1000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetValue (2 + 0.5 * skill->GetLevel ());
            skill->GetVictim ()->SetAddspeed (1);
            return true;
        }
#endif
    };
}
#endif
