#ifndef __CPPGEN_GNET_SKILL4032
#define __CPPGEN_GNET_SKILL4032
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4032:public Skill
    {
      public:
        enum
        { SKILL_ID = 4032 };
          Skill4032 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4032Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 500;
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
                return 500;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar5 (skill->GetPlayer ()->GetSkilllevel (4579));
                skill->GetPlayer ()->SetVar4 ((skill->GetPlayer ()->GetCultivation () == 2)
                                              && (skill->GetPlayer ()->GetSkilllevel (4579) > 0) ? 120 : 0);
                skill->GetPlayer ()->SetVar1 (1);
                skill->SetPlus (skill->GetPlayer ()->GetLevel () * 5 *
                                (1 +
                                 0.1 *
                                 ((skill->GetPlayer ()->GetDarklightform () == 3 || skill->GetPlayer ()->GetDarklightform () == 5
                                   || skill->GetPlayer ()->GetDarklightform () == 6) ? skill->GetT2 () : 0)));
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetMaxatk ());
                skill->GetPlayer ()->SetVar3 (skill->GetPlayer ()->GetRes2 ());
                skill->GetPlayer ()->SetDarklight (skill->GetPlayer ()->GetDarklight () -
                                                   (skill->GetLevel () + skill->GetT0 ()) * (skill->GetPlayer ()->GetDarklightform () == 6 ? -1 : 1));
                skill->GetPlayer ()->SetPerform (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State3:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 500;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar5 (skill->GetPlayer ()->GetSkilllevel (4579));
                skill->GetPlayer ()->SetVar4 ((skill->GetPlayer ()->GetCultivation () == 2)
                                              && (skill->GetPlayer ()->GetSkilllevel (4579) > 0) ? 120 : 0);
                skill->GetPlayer ()->SetVar1 (2);
                skill->SetPlus (skill->GetPlayer ()->GetLevel () * 5 *
                                (1 +
                                 0.1 *
                                 ((skill->GetPlayer ()->GetDarklightform () == 3 || skill->GetPlayer ()->GetDarklightform () == 5
                                   || skill->GetPlayer ()->GetDarklightform () == 6) ? skill->GetT2 () : 0)));
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetMaxatk ());
                skill->GetPlayer ()->SetVar3 (skill->GetPlayer ()->GetRes2 ());
                skill->GetPlayer ()->SetPerform (0);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State4:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 725;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar5 (skill->GetPlayer ()->GetSkilllevel (4579));
                skill->GetPlayer ()->SetVar4 ((skill->GetPlayer ()->GetCultivation () == 2)
                                              && (skill->GetPlayer ()->GetSkilllevel (4579) > 0) ? 120 : 0);
                skill->GetPlayer ()->SetVar1 (3);
                skill->SetPlus (skill->GetPlayer ()->GetLevel () * 5 *
                                (1 +
                                 0.1 *
                                 ((skill->GetPlayer ()->GetDarklightform () == 3 || skill->GetPlayer ()->GetDarklightform () == 5
                                   || skill->GetPlayer ()->GetDarklightform () == 6) ? skill->GetT2 () : 0)));
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetMaxatk ());
                skill->GetPlayer ()->SetVar3 (skill->GetPlayer ()->GetRes2 ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill4032Stub ():SkillStub (4032)
        {
            occupation = 59;
            name = L"ËéÄù¡¤½Ù¡¤ÓÀÒ¹";
            nativename = "ËéÄù¡¤½Ù¡¤ÓÀÒ¹";
            icon = "ËéÄù¡¤½Ù¡¤ÓÀÒ¹.dds";
            maxlevel = 10;
            maxlearn = 9;
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
            skill_class = 11;
            guide = true;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 1;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 1;
            talent[0] = 4115;
            talent[1] = 4116;
            talent[2] = 4117;
            talent[3] = 4124;
            talent[4] = 4152;
            talent_size = 5;
            action[0] = "ÐùÔ¯_°µ·ç±©";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "ÐùÔ¯_°µ·ç±©";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ÐùÔ¯_Æï³Ë_°µ·ç±©";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "ÐùÔ¯_Æï³Ë_°µ·ç±©";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 3;
            doenchant = true;
            dobless = true;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = 9;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
            statestub.push_back (new State3 ());
            statestub.push_back (new State4 ());
#endif
        }
        virtual ~ Skill4032Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 4030 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetLevel () * 5);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2225;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 15000 - 1000 * skill->GetT4 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 115, 117, 119, 121, 123, 125, 125, 125, 125 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (15);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (15);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetPlayer ()->GetLevel () * 5, skill->GetLevel (), 20, 80 + 20 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetRes2 () <= skill->GetPlayer ()->GetVar3 ()? 120 : 0));
            skill->GetVictim ()->SetTime (10);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->
                SetAmount (((skill->GetPlayer ()->GetMaxhp () - skill->GetPlayer ()->GetHp ()) <
                            skill->GetPlayer ()->GetVar2 () * (0.8 + 0.2 * skill->GetLevel ())? (skill->GetPlayer ()->GetMaxhp () -
                                                                                                 skill->GetPlayer ()->GetHp ()) : skill->
                            GetPlayer ()->GetVar2 () * (0.8 + 0.2 * skill->GetLevel ())) * (1 + ((skill->GetPlayer ()->GetType () == 0)
                                                                                                 && (skill->GetPlayer ()->GetVar4 () >
                                                                                                     0) ? 0.1 *
                                                                                                 skill->GetPlayer ()->GetVar5 () : 0)));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetT3 () > 0 ? 120 : 0));
            skill->GetVictim ()->SetTime (3000);
            skill->GetVictim ()->SetRatio (0.01 * 5 * skill->GetT3 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecskilldodge (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () == 3 ? 120 : 0));
            skill->GetVictim ()->SetTime (12000);
            skill->GetVictim ()->SetRatio (15);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (54003);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetCreateobject (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (20);
        }
#endif
    };
}
#endif
