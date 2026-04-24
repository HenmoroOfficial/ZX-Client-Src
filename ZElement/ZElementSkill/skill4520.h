#ifndef __CPPGEN_GNET_SKILL4520
#define __CPPGEN_GNET_SKILL4520
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4520:public Skill
    {
      public:
        enum
        { SKILL_ID = 4520 };
          Skill4520 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4520Stub:public SkillStub
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
                skill->SetPlus (210 * skill->GetLevel () + 0.03 * skill->GetT1 () * skill->GetPlayer ()->GetDef ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4520Stub ():SkillStub (4520)
        {
            occupation = 130;
            name = L"³Ð¡¤Ê¥Ëþ½µÄ§";
            nativename = "³Ð¡¤Ê¥Ëþ½µÄ§";
            icon = "Ê¥Ëþ½µÄ§.dds";
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
            action[0] = "¹íµÀ_·¨±¦_ËÄÏñ·ðËþ";
            action[1] = "ÇàÔÆÃÅ_·¨±¦_ËÄÏñ·ðËþ";
            action[2] = "¹íÍõ×Ú_·¨±¦_ËÄÏñ·ðËþ";
            action[3] = "ºÏ»¶ÅÉ_·¨±¦_ËÄÏñ·ðËþ";
            action[4] = "ÌìÒôËÂ_·¨±¦_ËÄÏñ·ðËþ";
            action[5] = "¹íµÀ_·¨±¦_ËÄÏñ·ðËþ";
            action[6] = "·ÙÏã¹È_·¨±¦_ËÄÏñ·ðËþ";
            action[7] = "ÊÞÉñ_·¨±¦_ËÄÏó·ðËþ";
            action[8] = "ºüÑý_·¨±¦_ËÄÏó·ðËþ";
            action[9] = "ÐùÔ¯_·¨±¦_ËÄÏó·ðËþ";
            action[10] = "»³¹â_·¨±¦_ËÄÏó·ðËþ";
            action[11] = "Ì«ê»_·¨±¦_ËÄÏó·ðËþ";
            action[12] = "Ìì»ª_·¨±¦_ËÄÏó·ðËþ";
            action[13] = "ÁéÙí_·¨±¦_ËÄÏó·ðËþ";
            action[14] = "Ó¢ÕÐ_·¨±¦_ËÄÏó·ðËþ";
            action[15] = "¹íµÀ_Æï³Ë_·¨±¦_ËÄÏñ·ðËþ";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_·¨±¦_ËÄÏñ·ðËþ";
            action[17] = "¹íÍõ×Ú_Æï³Ë_·¨±¦_ËÄÏñ·ðËþ";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_·¨±¦_ËÄÏñ·ðËþ";
            action[19] = "ÌìÒôËÂ_Æï³Ë_·¨±¦_ËÄÏñ·ðËþ";
            action[20] = "¹íµÀ_Æï³Ë_·¨±¦_ËÄÏñ·ðËþ";
            action[21] = "·ÙÏã¹È_Æï³Ë_·¨±¦_ËÄÏñ·ðËþ";
            action[22] = "ÊÞÉñ_Æï³Ë_·¨±¦_ËÄÏó·ðËþ";
            action[23] = "ºüÑý_Æï³Ë_·¨±¦_ËÄÏó·ðËþ";
            action[24] = "ÐùÔ¯_Æï³Ë_·¨±¦_ËÄÏó·ðËþ";
            action[25] = "»³¹â_Æï³Ë_·¨±¦_ËÄÏó·ðËþ";
            action[26] = "Ì«ê»_Æï³Ë_·¨±¦_ËÄÏó·ðËþ";
            action[27] = "Ìì»ª_Æï³Ë_·¨±¦_ËÄÏó·ðËþ";
            action[28] = "ÁéÙí_Æï³Ë_·¨±¦_ËÄÏñ·ðËþ";
            action[29] = "Ó¢ÕÐ_Æï³Ë_·¨±¦_ËÄÏñ·ðËþ";
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
        virtual ~ Skill4520Stub ()
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
            return (float) (380 - 30 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel ());
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
                               380 - 30 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel (),
                               6.3 - 0.3 * skill->GetLevel (),
                               5 * skill->GetLevel (),
                               210 * skill->GetLevel (),
                               10 + 10 * skill->GetLevel (), 9 - skill->GetLevel (), 10 + 10 * skill->GetLevel (), 10 + 10 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (norm (zrand (100) / (90 - 10 * skill->GetLevel ())) * 600));
            skill->GetVictim ()->SetTime (2200);
            skill->GetVictim ()->SetDizzy (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetAmount (1);
            skill->GetVictim ()->SetClearbuff (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (10 + 10 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (380 - 30 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetMpgen (1);
            skill->GetVictim ()->SetProbability (1.0 * (10 + 10 * skill->GetLevel ()));
            skill->GetVictim ()->SetAmount (1);
            skill->GetVictim ()->SetCleardebuff (1);
            return true;
        }
#endif
    };
}
#endif
