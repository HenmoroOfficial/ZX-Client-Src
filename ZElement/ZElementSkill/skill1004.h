#ifndef __CPPGEN_GNET_SKILL1004
#define __CPPGEN_GNET_SKILL1004
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1004:public Skill
    {
      public:
        enum
        { SKILL_ID = 1004 };
          Skill1004 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1004Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1004Stub ():SkillStub (1004)
        {
            occupation = 130;
            name = L"ÐþÎäÖ®×£";
            nativename = "ÐþÎäÖ®×£";
            icon = "ÐþÎäÖ®×£.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 2;
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
            talent[0] = 687;
            talent[1] = 1582;
            talent_size = 2;
            action[0] = "¹íÍõ×Ú_ºáÐÐÎÞ¼É";
            action[1] = "ÇàÔÆÃÅ_±ùÐÄ¾÷";
            action[2] = "¹íÍõ×Ú_ºáÐÐÎÞ¼É";
            action[3] = "ºÏ»¶ÅÉ_Æí¸£";
            action[4] = "ÌìÒôËÂ_Ò»ºÈ";
            action[5] = "ÌìÒôËÂ_Ò»ºÈ";
            action[6] = "ÌìÒôËÂ_Ò»ºÈ";
            action[7] = "ÊÞÉñ_»¢Á¦";
            action[8] = "ºüÑý_¼²Èç·ç";
            action[9] = "ÐùÔ¯_µÍ½×°µÊ¹";
            action[10] = "»³¹â_ÍµÏ®";
            action[11] = "ÊÞÉñ_»¢Á¦";
            action[12] = "Ìì»ª_ËÉ·çÑÅÔÏ";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_ºáÐÐÎÞ¼É";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_±ùÐÄ¾÷";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ºáÐÐÎÞ¼É";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_Æí¸£";
            action[19] = "ÌìÒôËÂ_Æï³Ë_Ò»ºÈ";
            action[20] = "ÌìÒôËÂ_Æï³Ë_Ò»ºÈ";
            action[21] = "ÌìÒôËÂ_Æï³Ë_Ò»ºÈ";
            action[22] = "ÊÞÉñ_Æï³Ë_»¢Á¦";
            action[23] = "ºüÑý_Æï³Ë_¼²Èç·ç";
            action[24] = "ÐùÔ¯_Æï³Ë_µÍ½×°µÊ¹";
            action[25] = "»³¹â_Æï³Ë_ÍµÏ®";
            action[26] = "ÊÞÉñ_Æï³Ë_»¢Á¦";
            action[27] = "Ìì»ª_Æï³Ë_ËÉ·çÑÅÔÏ";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 5;
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
        virtual ~ Skill1004Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (365);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 240000 - 10000 * skill->GetT0 () - 10000 * skill->GetLevel ();
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 365);

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
            skill->GetVictim ()->SetRatio (220);
            skill->GetVictim ()->
                SetAmount (((skill->GetPlayer ()->GetOccupation () == 1 || skill->GetPlayer ()->GetOccupation () == 2
                             || skill->GetPlayer ()->GetOccupation () == 3 || skill->GetPlayer ()->GetOccupation () == 13
                             || skill->GetPlayer ()->GetOccupation () == 14) ? 258 : 0) + ((skill->GetPlayer ()->GetOccupation () == 4
                                                                                            || skill->GetPlayer ()->GetOccupation () == 5
                                                                                            || skill->GetPlayer ()->GetOccupation () == 6
                                                                                            || skill->GetPlayer ()->GetOccupation () == 16
                                                                                            || skill->GetPlayer ()->GetOccupation () ==
                                                                                            17) ? 291 : 0) +
                           ((skill->GetPlayer ()->GetOccupation () == 7 || skill->GetPlayer ()->GetOccupation () == 8
                             || skill->GetPlayer ()->GetOccupation () == 9 || skill->GetPlayer ()->GetOccupation () == 19
                             || skill->GetPlayer ()->GetOccupation () == 20) ? 235 : 0) + ((skill->GetPlayer ()->GetOccupation () == 10
                                                                                            || skill->GetPlayer ()->GetOccupation () == 11
                                                                                            || skill->GetPlayer ()->GetOccupation () == 12
                                                                                            || skill->GetPlayer ()->GetOccupation () == 22
                                                                                            || skill->GetPlayer ()->GetOccupation () ==
                                                                                            23) ? 319 : 0) +
                           ((skill->GetPlayer ()->GetOccupation () == 25 || skill->GetPlayer ()->GetOccupation () == 26
                             || skill->GetPlayer ()->GetOccupation () == 27 || skill->GetPlayer ()->GetOccupation () == 28
                             || skill->GetPlayer ()->GetOccupation () == 29) ? 1102 : 0) + ((skill->GetPlayer ()->GetOccupation () == 64
                                                                                             || skill->GetPlayer ()->GetOccupation () == 65
                                                                                             || skill->GetPlayer ()->GetOccupation () == 66
                                                                                             || skill->GetPlayer ()->GetOccupation () == 67
                                                                                             || skill->GetPlayer ()->GetOccupation () ==
                                                                                             68) ? 3435 : 0));
            skill->GetVictim ()->
                SetValue (((skill->GetPlayer ()->GetOccupation () == 1 || skill->GetPlayer ()->GetOccupation () == 2
                            || skill->GetPlayer ()->GetOccupation () == 3 || skill->GetPlayer ()->GetOccupation () == 13
                            || skill->GetPlayer ()->GetOccupation () == 14) ? 524 : 0) + ((skill->GetPlayer ()->GetOccupation () == 4
                                                                                           || skill->GetPlayer ()->GetOccupation () == 5
                                                                                           || skill->GetPlayer ()->GetOccupation () == 6
                                                                                           || skill->GetPlayer ()->GetOccupation () == 16
                                                                                           || skill->GetPlayer ()->GetOccupation () ==
                                                                                           17) ? 530 : 0) + ((skill->GetPlayer ()->GetOccupation () ==
                                                                                                              7
                                                                                                              || skill->GetPlayer ()->
                                                                                                              GetOccupation () == 8
                                                                                                              || skill->GetPlayer ()->
                                                                                                              GetOccupation () == 9
                                                                                                              || skill->GetPlayer ()->
                                                                                                              GetOccupation () == 19
                                                                                                              || skill->GetPlayer ()->
                                                                                                              GetOccupation () ==
                                                                                                              20) ? 540 : 0) +
                          ((skill->GetPlayer ()->GetOccupation () == 10 || skill->GetPlayer ()->GetOccupation () == 11
                            || skill->GetPlayer ()->GetOccupation () == 12 || skill->GetPlayer ()->GetOccupation () == 22
                            || skill->GetPlayer ()->GetOccupation () == 23) ? 550 : 0) + ((skill->GetPlayer ()->GetOccupation () == 25
                                                                                           || skill->GetPlayer ()->GetOccupation () == 26
                                                                                           || skill->GetPlayer ()->GetOccupation () == 27
                                                                                           || skill->GetPlayer ()->GetOccupation () == 28
                                                                                           || skill->GetPlayer ()->GetOccupation () ==
                                                                                           29) ? 1124 : 0) +
                          ((skill->GetPlayer ()->GetOccupation () == 64 || skill->GetPlayer ()->GetOccupation () == 65
                            || skill->GetPlayer ()->GetOccupation () == 66 || skill->GetPlayer ()->GetOccupation () == 67
                            || skill->GetPlayer ()->GetOccupation () == 68) ? 3444 : 0));
            skill->GetVictim ()->SetClearcooldown (1);
            skill->GetVictim ()->SetRatio (220);
            skill->GetVictim ()->
                SetAmount (((skill->GetPlayer ()->GetOccupation () == 33 || skill->GetPlayer ()->GetOccupation () == 34
                             || skill->GetPlayer ()->GetOccupation () == 35 || skill->GetPlayer ()->GetOccupation () == 36
                             || skill->GetPlayer ()->GetOccupation () == 37) ? 1854 : 0) + ((skill->GetPlayer ()->GetOccupation () == 39
                                                                                             || skill->GetPlayer ()->GetOccupation () == 40
                                                                                             || skill->GetPlayer ()->GetOccupation () == 41
                                                                                             || skill->GetPlayer ()->GetOccupation () == 42
                                                                                             || skill->GetPlayer ()->GetOccupation () ==
                                                                                             43) ? 1908 : 0) +
                           ((skill->GetPlayer ()->GetOccupation () == 45 || skill->GetPlayer ()->GetOccupation () == 46
                             || skill->GetPlayer ()->GetOccupation () == 47 || skill->GetPlayer ()->GetOccupation () == 48
                             || skill->GetPlayer ()->GetOccupation () == 49) ? 2431 : 0) + ((skill->GetPlayer ()->GetOccupation () == 51
                                                                                             || skill->GetPlayer ()->GetOccupation () == 52
                                                                                             || skill->GetPlayer ()->GetOccupation () == 53
                                                                                             || skill->GetPlayer ()->GetOccupation () == 54
                                                                                             || skill->GetPlayer ()->GetOccupation () ==
                                                                                             55) ? 2496 : 0) +
                           ((skill->GetPlayer ()->GetOccupation () == 96 || skill->GetPlayer ()->GetOccupation () == 97
                             || skill->GetPlayer ()->GetOccupation () == 98 || skill->GetPlayer ()->GetOccupation () == 99
                             || skill->GetPlayer ()->GetOccupation () == 100) ? 3496 : 0) + ((skill->GetPlayer ()->GetOccupation () == 56
                                                                                              || skill->GetPlayer ()->GetOccupation () == 57
                                                                                              || skill->GetPlayer ()->GetOccupation () == 58
                                                                                              || skill->GetPlayer ()->GetOccupation () == 59
                                                                                              || skill->GetPlayer ()->GetOccupation () ==
                                                                                              60) ? 4010 : 0));
            skill->GetVictim ()->
                SetValue (((skill->GetPlayer ()->GetOccupation () == 33 || skill->GetPlayer ()->GetOccupation () == 34
                            || skill->GetPlayer ()->GetOccupation () == 35 || skill->GetPlayer ()->GetOccupation () == 36
                            || skill->GetPlayer ()->GetOccupation () == 37) ? 1872 : 0) + ((skill->GetPlayer ()->GetOccupation () == 39
                                                                                            || skill->GetPlayer ()->GetOccupation () == 40
                                                                                            || skill->GetPlayer ()->GetOccupation () == 41
                                                                                            || skill->GetPlayer ()->GetOccupation () == 42
                                                                                            || skill->GetPlayer ()->GetOccupation () ==
                                                                                            43) ? 1917 : 0) +
                          ((skill->GetPlayer ()->GetOccupation () == 45 || skill->GetPlayer ()->GetOccupation () == 46
                            || skill->GetPlayer ()->GetOccupation () == 47 || skill->GetPlayer ()->GetOccupation () == 48
                            || skill->GetPlayer ()->GetOccupation () == 49) ? 2449 : 0) + ((skill->GetPlayer ()->GetOccupation () == 51
                                                                                            || skill->GetPlayer ()->GetOccupation () == 52
                                                                                            || skill->GetPlayer ()->GetOccupation () == 53
                                                                                            || skill->GetPlayer ()->GetOccupation () == 54
                                                                                            || skill->GetPlayer ()->GetOccupation () ==
                                                                                            55) ? 2483 : 0) +
                          ((skill->GetPlayer ()->GetOccupation () == 96 || skill->GetPlayer ()->GetOccupation () == 97
                            || skill->GetPlayer ()->GetOccupation () == 98 || skill->GetPlayer ()->GetOccupation () == 99
                            || skill->GetPlayer ()->GetOccupation () == 100) ? 3484 : 0) + ((skill->GetPlayer ()->GetOccupation () == 56
                                                                                             || skill->GetPlayer ()->GetOccupation () == 57
                                                                                             || skill->GetPlayer ()->GetOccupation () == 58
                                                                                             || skill->GetPlayer ()->GetOccupation () == 59
                                                                                             || skill->GetPlayer ()->GetOccupation () ==
                                                                                             60) ? 4019 : 0));
            skill->GetVictim ()->SetClearcooldown (1);
            return true;
        }
#endif
    };
}
#endif
