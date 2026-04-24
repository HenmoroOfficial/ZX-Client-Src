#ifndef __CPPGEN_GNET_SKILL3287
#define __CPPGEN_GNET_SKILL3287
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3287:public Skill
    {
      public:
        enum
        { SKILL_ID = 3287 };
          Skill3287 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3287Stub:public SkillStub
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
      Skill3287Stub ():SkillStub (3287)
        {
            occupation = 128;
            name = L"新主线神族传送";
            nativename = "新主线神族传送";
            icon = "";
            maxlevel = 10;
            maxlearn = 10;
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
        virtual ~ Skill3287Stub ()
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
            return (float) (2.6);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (2.6);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (2.5);
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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetLevel () == 1 ? 100 : 0));
            skill->GetVictim ()->SetTime (16000);
            skill->GetVictim ()->
                SetBuffid ((skill->GetPlayer ()->GetOccupation () == 33 || skill->GetPlayer ()->GetOccupation () == 34
                            || skill->GetPlayer ()->GetOccupation () == 35 || skill->GetPlayer ()->GetOccupation () == 36
                            || skill->GetPlayer ()->GetOccupation () == 37) ? 40 : ((skill->GetPlayer ()->GetOccupation () == 45
                                                                                     || skill->GetPlayer ()->GetOccupation () == 46
                                                                                     || skill->GetPlayer ()->GetOccupation () == 47
                                                                                     || skill->GetPlayer ()->GetOccupation () == 48
                                                                                     || skill->GetPlayer ()->GetOccupation () == 49) ? 40 : 345));
            skill->GetVictim ()->
                SetAmount ((skill->GetPlayer ()->GetOccupation () == 33 || skill->GetPlayer ()->GetOccupation () == 34
                            || skill->GetPlayer ()->GetOccupation () == 35 || skill->GetPlayer ()->GetOccupation () == 36
                            || skill->GetPlayer ()->GetOccupation () == 37) ? 462 : ((skill->GetPlayer ()->GetOccupation () == 45
                                                                                      || skill->GetPlayer ()->GetOccupation () == 46
                                                                                      || skill->GetPlayer ()->GetOccupation () == 47
                                                                                      || skill->GetPlayer ()->GetOccupation () == 48
                                                                                      || skill->GetPlayer ()->GetOccupation () == 49) ? -143 : 103));
            skill->GetVictim ()->
                SetValue ((skill->GetPlayer ()->GetOccupation () == 33 || skill->GetPlayer ()->GetOccupation () == 34
                           || skill->GetPlayer ()->GetOccupation () == 35 || skill->GetPlayer ()->GetOccupation () == 36
                           || skill->GetPlayer ()->GetOccupation () == 37) ? 69 : ((skill->GetPlayer ()->GetOccupation () == 45
                                                                                    || skill->GetPlayer ()->GetOccupation () == 46
                                                                                    || skill->GetPlayer ()->GetOccupation () == 47
                                                                                    || skill->GetPlayer ()->GetOccupation () == 48
                                                                                    || skill->GetPlayer ()->GetOccupation () == 49) ? 370 : 481));
            skill->GetVictim ()->SetMagicdoor (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetLevel () == 2 ? 100 : 0));
            skill->GetVictim ()->SetTime (16000);
            skill->GetVictim ()->
                SetBuffid ((skill->GetPlayer ()->GetOccupation () == 39 || skill->GetPlayer ()->GetOccupation () == 40
                            || skill->GetPlayer ()->GetOccupation () == 41 || skill->GetPlayer ()->GetOccupation () == 42
                            || skill->GetPlayer ()->GetOccupation () == 43) ? 40 : 392);
            skill->GetVictim ()->
                SetAmount ((skill->GetPlayer ()->GetOccupation () == 39 || skill->GetPlayer ()->GetOccupation () == 40
                            || skill->GetPlayer ()->GetOccupation () == 41 || skill->GetPlayer ()->GetOccupation () == 42
                            || skill->GetPlayer ()->GetOccupation () == 43) ? 462 : -366);
            skill->GetVictim ()->
                SetValue ((skill->GetPlayer ()->GetOccupation () == 39 || skill->GetPlayer ()->GetOccupation () == 40
                           || skill->GetPlayer ()->GetOccupation () == 41 || skill->GetPlayer ()->GetOccupation () == 42
                           || skill->GetPlayer ()->GetOccupation () == 43) ? 135 : 456);
            skill->GetVictim ()->SetMagicdoor (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetLevel () == 3 ? 100 : 0));
            skill->GetVictim ()->SetTime (16000);
            skill->GetVictim ()->
                SetBuffid ((skill->GetPlayer ()->GetOccupation () == 45 || skill->GetPlayer ()->GetOccupation () == 46
                            || skill->GetPlayer ()->GetOccupation () == 47 || skill->GetPlayer ()->GetOccupation () == 48
                            || skill->GetPlayer ()->GetOccupation () == 49) ? 40 : 345);
            skill->GetVictim ()->
                SetAmount ((skill->GetPlayer ()->GetOccupation () == 45 || skill->GetPlayer ()->GetOccupation () == 46
                            || skill->GetPlayer ()->GetOccupation () == 47 || skill->GetPlayer ()->GetOccupation () == 48
                            || skill->GetPlayer ()->GetOccupation () == 49) ? -143 : 103);
            skill->GetVictim ()->
                SetValue ((skill->GetPlayer ()->GetOccupation () == 45 || skill->GetPlayer ()->GetOccupation () == 46
                           || skill->GetPlayer ()->GetOccupation () == 47 || skill->GetPlayer ()->GetOccupation () == 48
                           || skill->GetPlayer ()->GetOccupation () == 49) ? 370 : 481);
            skill->GetVictim ()->SetMagicdoor (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetLevel () == 4 ? 100 : 0));
            skill->GetVictim ()->SetTime (16000);
            skill->GetVictim ()->
                SetBuffid ((skill->GetPlayer ()->GetOccupation () == 51 || skill->GetPlayer ()->GetOccupation () == 52
                            || skill->GetPlayer ()->GetOccupation () == 53 || skill->GetPlayer ()->GetOccupation () == 54
                            || skill->GetPlayer ()->GetOccupation () == 55) ? 40 : 389);
            skill->GetVictim ()->
                SetAmount ((skill->GetPlayer ()->GetOccupation () == 51 || skill->GetPlayer ()->GetOccupation () == 52
                            || skill->GetPlayer ()->GetOccupation () == 53 || skill->GetPlayer ()->GetOccupation () == 54
                            || skill->GetPlayer ()->GetOccupation () == 55) ? -142 : -324);
            skill->GetVictim ()->
                SetValue ((skill->GetPlayer ()->GetOccupation () == 51 || skill->GetPlayer ()->GetOccupation () == 52
                           || skill->GetPlayer ()->GetOccupation () == 53 || skill->GetPlayer ()->GetOccupation () == 54
                           || skill->GetPlayer ()->GetOccupation () == 55) ? 295 : 429);
            skill->GetVictim ()->SetMagicdoor (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (12);
        }
#endif
    };
}
#endif
