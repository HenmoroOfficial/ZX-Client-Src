#ifndef __CPPGEN_GNET_SKILL2712
#define __CPPGEN_GNET_SKILL2712
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2712:public Skill
    {
      public:
        enum
        { SKILL_ID = 2712 };
          Skill2712 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2712Stub:public SkillStub
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
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetHide ());
                skill->SetRatio (1 + 0.02 * skill->GetLevel () + (skill->GetPlayer ()->GetHide () > 0 ? 0.1 * skill->GetT0 () : 0));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2712Stub ():SkillStub (2712)
        {
            occupation = 150;
            name = L"ÁÔÏ®II";
            nativename = "ÁÔÏ®II";
            icon = "ÁÔÏ®II.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 0;
            credittype = 11;
            clearmask = 132;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 7;
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
            preskillnum = 1;
            talent[0] = 2528;
            talent[1] = 2453;
            talent_size = 2;
            action[0] = "»³¹â_»êÏ®";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "»³¹â_»êÏ®";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "»³¹â_Æï³Ë_»êÏ®";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "»³¹â_Æï³Ë_»êÏ®";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
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
            weapon_limit = 10;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill2712Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 2432 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 9 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1000);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 20000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 125, 125, 125, 125, 125, 127, 129, 131, 133, 135 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (7);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
        }
        int GetCreditCost (int level) const
        {
            static int array[10] = { 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 38000, 45000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 2 * skill->GetLevel (), 10 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetRatio (11);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetDispel (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () > 0 ? 120 : 10 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (60000);
            skill->GetVictim ()->
                SetRatio (((skill->GetPlayer ()->GetOccupation () == 1 || skill->GetPlayer ()->GetOccupation () == 2
                            || skill->GetPlayer ()->GetOccupation () == 3 || skill->GetPlayer ()->GetOccupation () == 13
                            || skill->GetPlayer ()->GetOccupation () == 14) ? 243 : 0) + ((skill->GetPlayer ()->GetOccupation () == 4
                                                                                           || skill->GetPlayer ()->GetOccupation () == 5
                                                                                           || skill->GetPlayer ()->GetOccupation () == 6
                                                                                           || skill->GetPlayer ()->GetOccupation () == 16
                                                                                           || skill->GetPlayer ()->GetOccupation () ==
                                                                                           17) ? 289 : 0) + ((skill->GetPlayer ()->GetOccupation () ==
                                                                                                              7
                                                                                                              || skill->GetPlayer ()->
                                                                                                              GetOccupation () == 8
                                                                                                              || skill->GetPlayer ()->
                                                                                                              GetOccupation () == 9
                                                                                                              || skill->GetPlayer ()->
                                                                                                              GetOccupation () == 19
                                                                                                              || skill->GetPlayer ()->
                                                                                                              GetOccupation () ==
                                                                                                              20) ? 233 : 0) +
                          ((skill->GetPlayer ()->GetOccupation () == 10 || skill->GetPlayer ()->GetOccupation () == 11
                            || skill->GetPlayer ()->GetOccupation () == 12 || skill->GetPlayer ()->GetOccupation () == 22
                            || skill->GetPlayer ()->GetOccupation () == 23) ? 276 : 0) + ((skill->GetPlayer ()->GetOccupation () == 25
                                                                                           || skill->GetPlayer ()->GetOccupation () == 26
                                                                                           || skill->GetPlayer ()->GetOccupation () == 27
                                                                                           || skill->GetPlayer ()->GetOccupation () == 28
                                                                                           || skill->GetPlayer ()->GetOccupation () ==
                                                                                           29) ? 1094 : 0));
            skill->GetVictim ()->
                SetAmount (((skill->GetPlayer ()->GetOccupation () == 1 || skill->GetPlayer ()->GetOccupation () == 2
                             || skill->GetPlayer ()->GetOccupation () == 3 || skill->GetPlayer ()->GetOccupation () == 13
                             || skill->GetPlayer ()->GetOccupation () == 14) ? 393 : 0) + ((skill->GetPlayer ()->GetOccupation () == 4
                                                                                            || skill->GetPlayer ()->GetOccupation () == 5
                                                                                            || skill->GetPlayer ()->GetOccupation () == 6
                                                                                            || skill->GetPlayer ()->GetOccupation () == 16
                                                                                            || skill->GetPlayer ()->GetOccupation () ==
                                                                                            17) ? 394 : 0) +
                           ((skill->GetPlayer ()->GetOccupation () == 7 || skill->GetPlayer ()->GetOccupation () == 8
                             || skill->GetPlayer ()->GetOccupation () == 9 || skill->GetPlayer ()->GetOccupation () == 19
                             || skill->GetPlayer ()->GetOccupation () == 20) ? 395 : 0) + ((skill->GetPlayer ()->GetOccupation () == 10
                                                                                            || skill->GetPlayer ()->GetOccupation () == 11
                                                                                            || skill->GetPlayer ()->GetOccupation () == 12
                                                                                            || skill->GetPlayer ()->GetOccupation () == 22
                                                                                            || skill->GetPlayer ()->GetOccupation () ==
                                                                                            23) ? 396 : 0) +
                           ((skill->GetPlayer ()->GetOccupation () == 25 || skill->GetPlayer ()->GetOccupation () == 26
                             || skill->GetPlayer ()->GetOccupation () == 27 || skill->GetPlayer ()->GetOccupation () == 28
                             || skill->GetPlayer ()->GetOccupation () == 29) ? 1104 : 0));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetSetcooldown (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () > 0 ? 120 : 10 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (60000);
            skill->GetVictim ()->
                SetRatio (((skill->GetPlayer ()->GetOccupation () == 33 || skill->GetPlayer ()->GetOccupation () == 34
                            || skill->GetPlayer ()->GetOccupation () == 35 || skill->GetPlayer ()->GetOccupation () == 36
                            || skill->GetPlayer ()->GetOccupation () == 37) ? 1844 : 0) + ((skill->GetPlayer ()->GetOccupation () == 39
                                                                                            || skill->GetPlayer ()->GetOccupation () == 40
                                                                                            || skill->GetPlayer ()->GetOccupation () == 41
                                                                                            || skill->GetPlayer ()->GetOccupation () == 42
                                                                                            || skill->GetPlayer ()->GetOccupation () ==
                                                                                            43) ? 1886 : 0) +
                          ((skill->GetPlayer ()->GetOccupation () == 45 || skill->GetPlayer ()->GetOccupation () == 46
                            || skill->GetPlayer ()->GetOccupation () == 47 || skill->GetPlayer ()->GetOccupation () == 48
                            || skill->GetPlayer ()->GetOccupation () == 49) ? 2416 : 0) + ((skill->GetPlayer ()->GetOccupation () == 51
                                                                                            || skill->GetPlayer ()->GetOccupation () == 52
                                                                                            || skill->GetPlayer ()->GetOccupation () == 53
                                                                                            || skill->GetPlayer ()->GetOccupation () == 54
                                                                                            || skill->GetPlayer ()->GetOccupation () ==
                                                                                            55) ? 2465 : 0));
            skill->GetVictim ()->
                SetAmount (((skill->GetPlayer ()->GetOccupation () == 33 || skill->GetPlayer ()->GetOccupation () == 34
                             || skill->GetPlayer ()->GetOccupation () == 35 || skill->GetPlayer ()->GetOccupation () == 36
                             || skill->GetPlayer ()->GetOccupation () == 37) ? 1853 : 0) + ((skill->GetPlayer ()->GetOccupation () == 39
                                                                                             || skill->GetPlayer ()->GetOccupation () == 40
                                                                                             || skill->GetPlayer ()->GetOccupation () == 41
                                                                                             || skill->GetPlayer ()->GetOccupation () == 42
                                                                                             || skill->GetPlayer ()->GetOccupation () ==
                                                                                             43) ? 1895 : 0) +
                           ((skill->GetPlayer ()->GetOccupation () == 45 || skill->GetPlayer ()->GetOccupation () == 46
                             || skill->GetPlayer ()->GetOccupation () == 47 || skill->GetPlayer ()->GetOccupation () == 48
                             || skill->GetPlayer ()->GetOccupation () == 49) ? 2426 : 0) + ((skill->GetPlayer ()->GetOccupation () == 51
                                                                                             || skill->GetPlayer ()->GetOccupation () == 52
                                                                                             || skill->GetPlayer ()->GetOccupation () == 53
                                                                                             || skill->GetPlayer ()->GetOccupation () == 54
                                                                                             || skill->GetPlayer ()->GetOccupation () ==
                                                                                             55) ? 2477 : 0));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetSetcooldown (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () > 0 ? 20 * skill->GetT1 () : 0));
            skill->GetVictim ()->SetTime (20000);
            skill->GetVictim ()->SetRatio (3);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount (1);
            skill->GetVictim ()->SetValue (34670);
            skill->GetVictim ()->SetSummonclone (1);
            return true;
        }
#endif
    };
}
#endif
