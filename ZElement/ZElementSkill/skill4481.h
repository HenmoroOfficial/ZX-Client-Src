#ifndef __CPPGEN_GNET_SKILL4481
#define __CPPGEN_GNET_SKILL4481
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4481:public Skill
    {
      public:
        enum
        { SKILL_ID = 4481 };
          Skill4481 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4481Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1625;
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
                return 375;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.03 * skill->GetLevel () + 0.01 * INT (skill->GetPlayer ()->GetLevel () / (11 - skill->GetLevel ())));
                skill->SetPlus (60 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4481Stub ():SkillStub (4481)
        {
            occupation = 130;
            name = L"³Ğ¡¤¶áÆÇ·µ»ê";
            nativename = "³Ğ¡¤¶áÆÇ·µ»ê";
            icon = "¶áÆÇ·µ»ê.dds";
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
            action[0] = "ºüÑı_·¨±¦_·µ»êÏã";
            action[1] = "¹íÍõ×Ú_·¨±¦_ËÄÁéá¦";
            action[2] = "¹íÍõ×Ú_·¨±¦_ËÄÁéá¦";
            action[3] = "¹íÍõ×Ú_·¨±¦_ËÄÁéá¦";
            action[4] = "¹íÍõ×Ú_·¨±¦_ËÄÁéá¦";
            action[5] = "¹íÍõ×Ú_·¨±¦_ËÄÁéá¦";
            action[6] = "¹íÍõ×Ú_·¨±¦_ËÄÁéá¦";
            action[7] = "ÊŞÉñ_·¨±¦_Ñª·ãÇ¹";
            action[8] = "ºüÑı_·¨±¦_·µ»êÏã";
            action[9] = "ĞùÔ¯_·¨±¦_ÄõÁú¹Ä";
            action[10] = "»³¹â_·¨±¦_ÆÆ¾ü";
            action[11] = "Ì«ê»_·¨±¦_´ºÇïËø";
            action[12] = "Ìì»ª_·¨±¦_ÑÇ·ç";
            action[13] = "01";
            action[14] = "01";
            action[15] = "ºüÑı_Æï³Ë_·¨±¦_·µ»êÏã";
            action[16] = "¹íÍõ×Ú_Æï³Ë_·¨±¦_ËÄÁéá¦";
            action[17] = "¹íÍõ×Ú_Æï³Ë_·¨±¦_ËÄÁéá¦";
            action[18] = "¹íÍõ×Ú_Æï³Ë_·¨±¦_ËÄÁéá¦";
            action[19] = "¹íÍõ×Ú_Æï³Ë_·¨±¦_ËÄÁéá¦";
            action[20] = "¹íÍõ×Ú_Æï³Ë_·¨±¦_ËÄÁéá¦";
            action[21] = "¹íÍõ×Ú_Æï³Ë_·¨±¦_ËÄÁéá¦";
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
        virtual ~ Skill4481Stub ()
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
            return (float) (195 - 15 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel ());
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
                               195 - 15 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel (),
                               6.3 - 0.3 * skill->GetLevel (),
                               3 * skill->GetLevel (),
                               60 * skill->GetLevel (),
                               11 - skill->GetLevel (),
                               13 + 3 * skill->GetLevel () + (skill->GetLevel () == 5 ? 5 : 0),
                               1 + (skill->GetLevel () == 5 ? 1 : 0),
                               6, 20 * skill->GetLevel (), 26 + 8 * skill->GetLevel (), 6 * 5 - 2 * skill->GetLevel (), 256);

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
            skill->GetVictim ()->SetProbability (1.0 * (26 + 8 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (6000 * 5 - 2000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetBuffid (4);
            skill->GetVictim ()->SetAmount (256);
            skill->GetVictim ()->SetCycsubdefence (1);
            skill->GetVictim ()->SetProbability (1.0 * (13 + 3 * skill->GetLevel () + (skill->GetLevel () == 5 ? 5 : 0)));
            skill->GetVictim ()->SetAmount (1 + (skill->GetLevel () == 5 ? 1 : 0));
            skill->GetVictim ()->SetClearbuff (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (6000 + 100);
            skill->GetVictim ()->SetBuffid (4);
            skill->GetVictim ()->SetAmount (skill->GetPlayer ()->GetMaxmp () * 0.2 * skill->GetLevel ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetMpleak (1);
            return true;
        }
#endif
    };
}
#endif
