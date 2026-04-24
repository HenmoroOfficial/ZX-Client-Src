#ifndef __CPPGEN_GNET_SKILL2947
#define __CPPGEN_GNET_SKILL2947
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2947:public Skill
    {
      public:
        enum
        { SKILL_ID = 2947 };
          Skill2947 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2947Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1000;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 330;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (1);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (0.3 + 0.01 * skill->GetT0 () + 0.03 * skill->GetLevel () +
                                 0.003 * INT (skill->GetPlayer ()->GetLevel () / (8 - skill->GetLevel ())));
                skill->SetPlus (255 * skill->GetLevel () + 0.01 * skill->GetT1 () * skill->GetPlayer ()->GetDef ());
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
                return 330;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (2);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (0.4 + 0.01 * skill->GetT0 () + 0.03 * skill->GetLevel () +
                                 0.003 * INT (skill->GetPlayer ()->GetLevel () / (8 - skill->GetLevel ())));
                skill->SetPlus (255 * skill->GetLevel () + 0.01 * skill->GetT1 () * skill->GetPlayer ()->GetDef ());
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
                return 340;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (3);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (0.5 + 0.01 * skill->GetT0 () + 0.03 * skill->GetLevel () +
                                 0.003 * INT (skill->GetPlayer ()->GetLevel () / (8 - skill->GetLevel ())));
                skill->SetPlus (255 * skill->GetLevel () + 0.01 * skill->GetT1 () * skill->GetPlayer ()->GetDef ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill2947Stub ():SkillStub (2947)
        {
            occupation = 130;
            name = L"Ììçğ¾øÊÀ";
            nativename = "Ììçğ¾øÊÀ";
            icon = "Ììçğ¾øÊÀ.dds";
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
            action[0] = "¹íµÀ_·¨±¦_Ììçğ";
            action[1] = "ÇàÔÆÃÅ_·¨±¦_Ììçğ";
            action[2] = "¹íÍõ×Ú_·¨±¦_Ììçğ";
            action[3] = "ºÏ»¶ÅÉ_·¨±¦_Ììçğ";
            action[4] = "ÌìÒôËÂ_·¨±¦_Ììçğ";
            action[5] = "¹íµÀ_·¨±¦_Ììçğ";
            action[6] = "·ÙÏã¹È_·¨±¦_Ììçğ";
            action[7] = "ÊŞÉñ_·¨±¦_Ììçğ";
            action[8] = "ºüÑı_·¨±¦_Ììçğ";
            action[9] = "ĞùÔ¯_·¨±¦_Ììçğ";
            action[10] = "»³¹â_·¨±¦_Ììçğ";
            action[11] = "Ì«ê»_·¨±¦_Ììçğ";
            action[12] = "Ìì»ª_·¨±¦_Ììçğ";
            action[13] = "ÁéÙí_·¨±¦_Ììçğ";
            action[14] = "Ó¢ÕĞ_·¨±¦_Ììçğ";
            action[15] = "¹íµÀ_Æï³Ë_·¨±¦_Ììçğ";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_·¨±¦_Ììçğ";
            action[17] = "¹íÍõ×Ú_Æï³Ë_·¨±¦_Ììçğ";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_·¨±¦_Ììçğ";
            action[19] = "ÌìÒôËÂ_Æï³Ë_·¨±¦_Ììçğ";
            action[20] = "¹íµÀ_Æï³Ë_·¨±¦_Ììçğ";
            action[21] = "·ÙÏã¹È_Æï³Ë_·¨±¦_Ììçğ";
            action[22] = "ÊŞÉñ_Æï³Ë_·¨±¦_Ììçğ";
            action[23] = "ºüÑı_Æï³Ë_·¨±¦_Ììçğ";
            action[24] = "ĞùÔ¯_Æï³Ë_·¨±¦_Ììçğ";
            action[25] = "»³¹â_Æï³Ë_·¨±¦_Ììçğ";
            action[26] = "Ì«ê»_Æï³Ë_·¨±¦_Ììçğ";
            action[27] = "Ìì»ª_Æï³Ë_·¨±¦_Ììçğ";
            action[28] = "ÁéÙí_Æï³Ë_·¨±¦_Ììçğ";
            action[29] = "Ó¢ÕĞ_Æï³Ë_·¨±¦_Ììçğ";
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
        virtual ~ Skill2947Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.1 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05 + 10);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (705 - 55 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6300 - 300 * skill->GetLevel ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.1 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05 + 5);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.1 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               705 - 55 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel (),
                               30 + 3 * skill->GetLevel (),
                               255 * skill->GetLevel (),
                               40 + 3 * skill->GetLevel (),
                               255 * skill->GetLevel (),
                               50 + 3 * skill->GetLevel (),
                               255 * skill->GetLevel (),
                               2 * (skill->GetLevel () + 1) * (skill->GetLevel () + 1),
                               skill->GetLevel () * 20,
                               10 + 4 * skill->GetLevel (),
                               (skill->GetLevel () == 5 ? 8 : (skill->GetLevel () == 4 ? 5 : skill->GetLevel ())),
                               7 * (skill->GetLevel () + 1),
                               0.5 * skill->GetLevel () * skill->GetLevel (),
                               6.3 - 0.3 * skill->GetLevel (),
                               8 - skill->GetLevel (),
                               skill->GetLevel () * 10,
                               10 + 4 * skill->GetLevel (),
                               skill->GetLevel () * 20,
                               skill->GetLevel () * 20,
                               20 * skill->GetLevel (), 3 * skill->GetLevel () + 1, 4 * skill->GetLevel (), 4 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () == 2 ? skill->GetLevel () * 20 : 0));
            skill->GetVictim ()->SetTime (10000 + 4000 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio (skill->GetPlayer ()->GetType () ==
                                           0 ? (2 * (skill->GetLevel () + 1) * (skill->GetLevel () + 1) * 0.01 +
                                                zrand (2) * (1 -
                                                             2 * (skill->GetLevel () + 1) * (skill->GetLevel () +
                                                                                             1) * 0.01)) : (skill->GetPlayer ()->GetDef () <
                                                                                                            25000 ? 2 * (skill->GetLevel () +
                                                                                                                         1) * (skill->GetLevel () +
                                                                                                                               1) * 0.01 +
                                                                                                            zrand (2) * (1 -
                                                                                                                         2 * (skill->GetLevel () +
                                                                                                                              1) *
                                                                                                                         (skill->GetLevel () +
                                                                                                                          1) * 0.01) : 0));
            skill->GetVictim ()->SetBuffid (6);
            skill->GetVictim ()->SetDecdefence (1);
            skill->GetVictim ()->SetTime ((zrand (100) < skill->GetLevel () * 20 ? 1 : 0) * 1000 * (3 * skill->GetLevel () + 1));
            skill->GetVictim ()->SetAmount (4 * skill->GetLevel ());
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetVar2 () * 0.04 * skill->GetLevel ());
            skill->GetVictim ()->SetCursed (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (skill->GetPlayer ()->GetVar1 () ==
                                          3 ? (zrand (100) <=
                                               (34 + 5 * (skill->GetLevel () + 1) +
                                                (skill->GetLevel () + 1) * (skill->GetLevel () +
                                                                            1)) ? 1000 * (0.5 * skill->GetLevel () * skill->GetLevel ()) : 0) : 0);
            skill->GetVictim ()->SetRatio ((skill->GetLevel () == 5 ? 8 : (skill->GetLevel () == 4 ? 5 : skill->GetLevel ())) * 0.01001);
            skill->GetVictim ()->SetInccritrate (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetLevel () * 20));
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetAmount (2);
            skill->GetVictim ()->SetValue (2);
            skill->GetVictim ()->SetExorcism (1);
            return true;
        }
#endif
    };
}
#endif
