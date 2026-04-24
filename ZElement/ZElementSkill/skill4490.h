#ifndef __CPPGEN_GNET_SKILL4490
#define __CPPGEN_GNET_SKILL4490
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4490:public Skill
    {
      public:
        enum
        { SKILL_ID = 4490 };
          Skill4490 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4490Stub:public SkillStub
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
                skill->SetRatio (1 + 0.1 * skill->GetT0 () + 0.05 * skill->GetLevel () +
                                 0.01 * INT (skill->GetPlayer ()->GetLevel () / (10 - skill->GetLevel ())));
                skill->SetPlus (135 * skill->GetLevel () +
                                0.03 * (skill->GetPlayer ()->GetHp () + skill->GetPlayer ()->GetMp ()) * (1 + 0.1 * skill->GetT0 ()) +
                                0.03 * skill->GetT1 () * skill->GetPlayer ()->GetDef ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4490Stub ():SkillStub (4490)
        {
            occupation = 130;
            name = L"³Ğ¡¤ËÀÍö²¨ÎÆ";
            nativename = "³Ğ¡¤ËÀÍö²¨ÎÆ";
            icon = "¹íòâÚ¤Ú¤.dds";
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
            talent[0] = 1174;
            talent[1] = 1582;
            talent_size = 2;
            action[0] = "¹íµÀ_·¨±¦_Ç§¶¾¹Æ";
            action[1] = "¹íµÀ_·¨±¦_Ç§¶¾¹Æ";
            action[2] = "¹íµÀ_·¨±¦_Ç§¶¾¹Æ";
            action[3] = "¹íµÀ_·¨±¦_Ç§¶¾¹Æ";
            action[4] = "¹íµÀ_·¨±¦_Ç§¶¾¹Æ";
            action[5] = "¹íµÀ_·¨±¦_Ç§¶¾¹Æ";
            action[6] = "¹íµÀ_·¨±¦_Ç§¶¾¹Æ";
            action[7] = "ÊŞÉñ_·¨±¦_î¸á°";
            action[8] = "ºüÑı_·¨±¦_¸¡Ó°¹Å¾µ";
            action[9] = "ĞùÔ¯_·¨±¦_ÂäµØ½ğÇ®";
            action[10] = "»³¹â_·¨±¦_Ì°ÀÇ";
            action[11] = "Ì«ê»_·¨±¦_×ÓÄ¸ÒõÑô¿Û";
            action[12] = "Ìì»ª_·¨±¦_ÁğÁ§Õµ";
            action[13] = "01";
            action[14] = "01";
            action[15] = "¹íµÀ_Æï³Ë_·¨±¦_Ç§¶¾¹Æ";
            action[16] = "¹íµÀ_Æï³Ë_·¨±¦_Ç§¶¾¹Æ";
            action[17] = "¹íµÀ_Æï³Ë_·¨±¦_Ç§¶¾¹Æ";
            action[18] = "¹íµÀ_Æï³Ë_·¨±¦_Ç§¶¾¹Æ";
            action[19] = "¹íµÀ_Æï³Ë_·¨±¦_Ç§¶¾¹Æ";
            action[20] = "¹íµÀ_Æï³Ë_·¨±¦_Ç§¶¾¹Æ";
            action[21] = "¹íµÀ_Æï³Ë_·¨±¦_Ç§¶¾¹Æ";
            action[22] = "ÊŞÉñ_Æï³Ë_·¨±¦_î¸á°";
            action[23] = "ºüÑı_Æï³Ë_·¨±¦_¸¡Ó°¹Å¾µ";
            action[24] = "ĞùÔ¯_Æï³Ë_·¨±¦_ÂäµØ½ğÇ®";
            action[25] = "»³¹â_Æï³Ë_·¨±¦_Ì°ÀÇ";
            action[26] = "Ì«ê»_Æï³Ë_·¨±¦_×ÓÄ¸ÒõÑô¿Û";
            action[27] = "Ìì»ª_Æï³Ë_·¨±¦_ÁğÁ§Õµ";
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
        virtual ~ Skill4490Stub ()
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
            return (float) (305 - 25 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel ());
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
                               305 - 25 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel (),
                               6.3 - 0.3 * skill->GetLevel (),
                               5 * skill->GetLevel (),
                               135 * skill->GetLevel (),
                               6, 3 * skill->GetLevel (), 10 - skill->GetLevel (), 30 * skill->GetLevel (), 3 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (6000 + 100);
            skill->GetVictim ()->SetRatio (0.03 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecskilldamage (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (3000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetBuffid (5);
            skill->GetVictim ()->SetValue (0.3 * skill->GetLevel () * skill->GetPlayer ()->GetLevel () + 1);
            skill->GetVictim ()->SetSubdefence (1);
            return true;
        }
#endif
    };
}
#endif
