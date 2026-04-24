#ifndef __CPPGEN_GNET_SKILL3329
#define __CPPGEN_GNET_SKILL3329
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3329:public Skill
    {
      public:
        enum
        { SKILL_ID = 3329 };
          Skill3329 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3329Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 50;
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
                return 50;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3329Stub ():SkillStub (3329)
        {
            occupation = 129;
            name = L"家族副本初始任务变身";
            nativename = "家族副本初始任务变身";
            icon = "";
            maxlevel = 1;
            maxlearn = 1;
            type = 3;
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
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
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
            action[0] = "";
            action[1] = "";
            action[2] = "";
            action[3] = "";
            action[4] = "";
            action[5] = "";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "";
            action[16] = "";
            action[17] = "";
            action[18] = "";
            action[19] = "";
            action[20] = "";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "";
            rangetype = 2;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 7;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill3329Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 100;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 0;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (40);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (39);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (38);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (300000);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetAmount (1);
            skill->GetVictim ()->
                SetValue ((skill->GetPlayer ()->GetOccupation () == 7 || skill->GetPlayer ()->GetOccupation () == 8
                           || skill->GetPlayer ()->GetOccupation () == 9 || skill->GetPlayer ()->GetOccupation () == 19
                           || skill->GetPlayer ()->GetOccupation () == 20 ? 1 : 0) * 410 + (skill->GetPlayer ()->GetOccupation () == 1
                                                                                            || skill->GetPlayer ()->GetOccupation () == 2
                                                                                            || skill->GetPlayer ()->GetOccupation () == 3
                                                                                            || skill->GetPlayer ()->GetOccupation () == 13
                                                                                            || skill->GetPlayer ()->GetOccupation () ==
                                                                                            14 ? 1 : 0) * 412 +
                          (skill->GetPlayer ()->GetOccupation () == 10 || skill->GetPlayer ()->GetOccupation () == 11
                           || skill->GetPlayer ()->GetOccupation () == 12 || skill->GetPlayer ()->GetOccupation () == 22
                           || skill->GetPlayer ()->GetOccupation () == 23 ? 1 : 0) * 413 + (skill->GetPlayer ()->GetOccupation () == 4
                                                                                            || skill->GetPlayer ()->GetOccupation () == 5
                                                                                            || skill->GetPlayer ()->GetOccupation () == 6
                                                                                            || skill->GetPlayer ()->GetOccupation () == 16
                                                                                            || skill->GetPlayer ()->GetOccupation () ==
                                                                                            17 ? 1 : 0) * 411 +
                          (skill->GetPlayer ()->GetOccupation () == 25 || skill->GetPlayer ()->GetOccupation () == 26
                           || skill->GetPlayer ()->GetOccupation () == 27 || skill->GetPlayer ()->GetOccupation () == 28
                           || skill->GetPlayer ()->GetOccupation () == 29 ? 1 : 0) * 414 + (skill->GetPlayer ()->GetOccupation () == 33
                                                                                            || skill->GetPlayer ()->GetOccupation () == 34
                                                                                            || skill->GetPlayer ()->GetOccupation () == 35
                                                                                            || skill->GetPlayer ()->GetOccupation () == 36
                                                                                            || skill->GetPlayer ()->GetOccupation () ==
                                                                                            37 ? 1 : 0) * 416 +
                          (skill->GetPlayer ()->GetOccupation () == 39 || skill->GetPlayer ()->GetOccupation () == 40
                           || skill->GetPlayer ()->GetOccupation () == 41 || skill->GetPlayer ()->GetOccupation () == 42
                           || skill->GetPlayer ()->GetOccupation () == 43 ? 1 : 0) * 415 + (skill->GetPlayer ()->GetOccupation () == 45
                                                                                            || skill->GetPlayer ()->GetOccupation () == 46
                                                                                            || skill->GetPlayer ()->GetOccupation () == 47
                                                                                            || skill->GetPlayer ()->GetOccupation () == 48
                                                                                            || skill->GetPlayer ()->GetOccupation () ==
                                                                                            49 ? 1 : 0) * 417 +
                          (skill->GetPlayer ()->GetOccupation () == 51 || skill->GetPlayer ()->GetOccupation () == 52
                           || skill->GetPlayer ()->GetOccupation () == 53 || skill->GetPlayer ()->GetOccupation () == 54
                           || skill->GetPlayer ()->GetOccupation () == 55 ? 1 : 0) * 418 + (skill->GetPlayer ()->GetOccupation () ==
                                                                                            0 ? 1 : 0) * (410 + zrand (5)) +
                          (skill->GetPlayer ()->GetOccupation () == 64 || skill->GetPlayer ()->GetOccupation () == 65
                           || skill->GetPlayer ()->GetOccupation () == 66 || skill->GetPlayer ()->GetOccupation () == 67
                           || skill->GetPlayer ()->GetOccupation () == 68 ? 1 : 0) * 486 + (skill->GetPlayer ()->GetOccupation () == 96
                                                                                            || skill->GetPlayer ()->GetOccupation () == 97
                                                                                            || skill->GetPlayer ()->GetOccupation () == 98
                                                                                            || skill->GetPlayer ()->GetOccupation () == 99
                                                                                            || skill->GetPlayer ()->GetOccupation () ==
                                                                                            100 ? 1 : 0) * 487 +
                          (skill->GetPlayer ()->GetOccupation () == 56 || skill->GetPlayer ()->GetOccupation () == 57
                           || skill->GetPlayer ()->GetOccupation () == 58 || skill->GetPlayer ()->GetOccupation () == 59
                           || skill->GetPlayer ()->GetOccupation () == 60 ? 1 : 0) * 708 + (skill->GetPlayer ()->GetOccupation () == 102
                                                                                            || skill->GetPlayer ()->GetOccupation () == 103
                                                                                            || skill->GetPlayer ()->GetOccupation () == 104
                                                                                            || skill->GetPlayer ()->GetOccupation () == 105
                                                                                            || skill->GetPlayer ()->GetOccupation () ==
                                                                                            106 ? 1 : 0) * 841 +
                          (skill->GetPlayer ()->GetOccupation () == 108 || skill->GetPlayer ()->GetOccupation () == 109
                           || skill->GetPlayer ()->GetOccupation () == 110 || skill->GetPlayer ()->GetOccupation () == 111
                           || skill->GetPlayer ()->GetOccupation () == 112 ? 1 : 0) * 842);
            skill->GetVictim ()->SetTransform (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (99);
        }
#endif
    };
}
#endif
