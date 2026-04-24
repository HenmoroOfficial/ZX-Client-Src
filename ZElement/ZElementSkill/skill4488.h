#ifndef __CPPGEN_GNET_SKILL4488
#define __CPPGEN_GNET_SKILL4488
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4488:public Skill
    {
      public:
        enum
        { SKILL_ID = 4488 };
          Skill4488 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4488Stub:public SkillStub
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
                skill->SetPlus (120 * skill->GetLevel () + 0.03 * skill->GetT1 () * skill->GetPlayer ()->GetDef ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4488Stub ():SkillStub (4488)
        {
            occupation = 130;
            name = L"³Ğ¡¤Ç¬À¤ÈçÒâ";
            nativename = "³Ğ¡¤Ç¬À¤ÈçÒâ";
            icon = "Ç¬À¤ÈçÒâ.dds";
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
            talent[0] = 670;
            talent[1] = 1582;
            talent_size = 2;
            action[0] = "¹íÍõ×Ú_·¨±¦_ÈçÒâÇ¬À¤´ü";
            action[1] = "ÇàÔÆÃÅ_·¨±¦_ÈçÒâÇ¬À¤´ü";
            action[2] = "¹íÍõ×Ú_·¨±¦_ÈçÒâÇ¬À¤´ü";
            action[3] = "ºÏ»¶ÅÉ_·¨±¦_ÈçÒâÇ¬À¤´ü";
            action[4] = "ÌìÒôËÂ_·¨±¦_ÈçÒâÇ¬À¤´ü";
            action[5] = "¹íµÀ_·¨±¦_ÈçÒâÇ¬À¤´ü";
            action[6] = "¹íµÀ_·¨±¦_ÈçÒâÇ¬À¤´ü";
            action[7] = "ÊŞÉñ_·¨±¦_î¸á°";
            action[8] = "ºüÑı_·¨±¦_¸¡Ó°¹Å¾µ";
            action[9] = "ĞùÔ¯_·¨±¦_ÂäµØ½ğÇ®";
            action[10] = "»³¹â_·¨±¦_Ì°ÀÇ";
            action[11] = "Ì«ê»_·¨±¦_×ÓÄ¸ÒõÑô¿Û";
            action[12] = "Ìì»ª_·¨±¦_ÁğÁ§Õµ";
            action[13] = "01";
            action[14] = "01";
            action[15] = "¹íÍõ×Ú_Æï³Ë_·¨±¦_ÈçÒâÇ¬À¤´ü";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_·¨±¦_ÈçÒâÇ¬À¤´ü";
            action[17] = "¹íÍõ×Ú_Æï³Ë_·¨±¦_ÈçÒâÇ¬À¤´ü";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_·¨±¦_ÈçÒâÇ¬À¤´ü";
            action[19] = "ÌìÒôËÂ_Æï³Ë_·¨±¦_ÈçÒâÇ¬À¤´ü";
            action[20] = "¹íµÀ_Æï³Ë_·¨±¦_ÈçÒâÇ¬À¤´ü";
            action[21] = "¹íµÀ_Æï³Ë_·¨±¦_ÈçÒâÇ¬À¤´ü";
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
        virtual ~ Skill4488Stub ()
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
        float GetRadius (Skill * skill) const
        {
            return (float) (15);
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
                               25 * skill->GetLevel (),
                               3 * skill->GetLevel (),
                               10 - skill->GetLevel (), 24 * skill->GetLevel (), 4 + skill->GetLevel (), 3 + 2 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (3000 * skill->GetLevel () + 100 + 1000 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (25 * skill->GetLevel () + 15 * skill->GetT0 ());
            skill->GetVictim ()->SetSubattack (1);
            skill->GetVictim ()->SetProbability (1.0 * (norm (zrand (100) / (100 - 4 - skill->GetLevel ())) * 600));
            skill->GetVictim ()->SetTime (3000 + 2000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetDizzy (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetValue (24 * skill->GetLevel () * (1 + skill->GetT0 ()));
            skill->GetVictim ()->SetJuqi (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (1 + INT (zrand (110) / 100) * 12);
        }
#endif
    };
}
#endif
