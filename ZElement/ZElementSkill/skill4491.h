#ifndef __CPPGEN_GNET_SKILL4491
#define __CPPGEN_GNET_SKILL4491
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4491:public Skill
    {
      public:
        enum
        { SKILL_ID = 4491 };
          Skill4491 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4491Stub:public SkillStub
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.05 * skill->GetLevel () + 0.01 * INT (skill->GetPlayer ()->GetLevel () / (10 - skill->GetLevel ())));
                skill->SetPlus (120 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4491Stub ():SkillStub (4491)
        {
            occupation = 130;
            name = L"³Ð¡¤Áè»ðÃ÷Ñô";
            nativename = "³Ð¡¤Áè»ðÃ÷Ñô";
            icon = "Áè»ðÃ÷Ñô.dds";
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
            action[0] = "·ÙÏã¹È_·¨±¦_»ðÁáçç";
            action[1] = "·ÙÏã¹È_·¨±¦_»ðÁáçç";
            action[2] = "·ÙÏã¹È_·¨±¦_»ðÁáçç";
            action[3] = "·ÙÏã¹È_·¨±¦_»ðÁáçç";
            action[4] = "·ÙÏã¹È_·¨±¦_»ðÁáçç";
            action[5] = "·ÙÏã¹È_·¨±¦_»ðÁáçç";
            action[6] = "·ÙÏã¹È_·¨±¦_»ðÁáçç";
            action[7] = "ÊÞÉñ_·¨±¦_î¸á°";
            action[8] = "ºüÑý_·¨±¦_¸¡Ó°¹Å¾µ";
            action[9] = "ÐùÔ¯_·¨±¦_ÂäµØ½ðÇ®";
            action[10] = "»³¹â_·¨±¦_Ì°ÀÇ";
            action[11] = "Ì«ê»_·¨±¦_×ÓÄ¸ÒõÑô¿Û";
            action[12] = "Ìì»ª_·¨±¦_ÁðÁ§Õµ";
            action[13] = "01";
            action[14] = "01";
            action[15] = "·ÙÏã¹È_Æï³Ë_·¨±¦_»ðÁáçç";
            action[16] = "·ÙÏã¹È_Æï³Ë_·¨±¦_»ðÁáçç";
            action[17] = "·ÙÏã¹È_Æï³Ë_·¨±¦_»ðÁáçç";
            action[18] = "·ÙÏã¹È_Æï³Ë_·¨±¦_»ðÁáçç";
            action[19] = "·ÙÏã¹È_Æï³Ë_·¨±¦_»ðÁáçç";
            action[20] = "·ÙÏã¹È_Æï³Ë_·¨±¦_»ðÁáçç";
            action[21] = "·ÙÏã¹È_Æï³Ë_·¨±¦_»ðÁáçç";
            action[22] = "ÊÞÉñ_Æï³Ë_·¨±¦_î¸á°";
            action[23] = "ºüÑý_Æï³Ë_·¨±¦_¸¡Ó°¹Å¾µ";
            action[24] = "ÐùÔ¯_Æï³Ë_·¨±¦_ÂäµØ½ðÇ®";
            action[25] = "»³¹â_Æï³Ë_·¨±¦_Ì°ÀÇ";
            action[26] = "Ì«ê»_Æï³Ë_·¨±¦_×ÓÄ¸ÒõÑô¿Û";
            action[27] = "Ìì»ª_Æï³Ë_·¨±¦_ÁðÁ§Õµ";
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
        virtual ~ Skill4491Stub ()
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
            return (float) (295 - 25 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel ());
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
                               295 - 25 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel (),
                               6.3 - 0.3 * skill->GetLevel (),
                               5 * skill->GetLevel (),
                               120 * skill->GetLevel (),
                               10 - skill->GetLevel (),
                               25 + 2 * skill->GetLevel (),
                               8 + skill->GetLevel (),
                               315 + 150 * skill->GetLevel (),
                               10 + 5 * skill->GetLevel (),
                               3 + skill->GetLevel (),
                               40 + 25 * skill->GetLevel (), 15 + 5 * skill->GetLevel (), 2 + skill->GetLevel (), 15 + 5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (25 + 2 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime ((8 + skill->GetLevel ()) * 1000);
            skill->GetVictim ()->SetRatio (6);
            skill->GetVictim ()->SetBuffid (4);
            skill->GetVictim ()->SetAmount (6);
            skill->GetVictim ()->SetValue (315 + 150 * skill->GetLevel ());
            skill->GetVictim ()->SetFiring (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetHasmultbuff (113) == 1 ? (10 + 5 * skill->GetLevel ()) : 0));
            skill->GetVictim ()->SetTime ((3 + skill->GetLevel ()) * 1000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (40 + 25 * skill->GetLevel ());
            skill->GetVictim ()->SetSubdefence (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetHasmultbuff (113) == 1
                                                  && zrand (100) < (15 + 5 * skill->GetLevel ())? 150 : 0));
            skill->GetVictim ()->SetTime ((2 + skill->GetLevel ()) * 1000);
            skill->GetVictim ()->SetRatio ((15 + 5 * skill->GetLevel ()) * 0.01);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetSlow (1);
            return true;
        }
#endif
    };
}
#endif
