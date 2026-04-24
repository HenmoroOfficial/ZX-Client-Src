#ifndef __CPPGEN_GNET_SKILL4014
#define __CPPGEN_GNET_SKILL4014
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4014:public Skill
    {
      public:
        enum
        { SKILL_ID = 4014 };
          Skill4014 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4014Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 375;
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
                return 625;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar3 (skill->GetPlayer ()->GetSkilllevel (4577));
                skill->GetPlayer ()->SetVar2 ((skill->GetPlayer ()->GetCultivation () == 2)
                                              && (skill->GetPlayer ()->GetSkilllevel (4577) > 0) ? 120 : 0);
                skill->SetPlus (skill->GetPlayer ()->GetLevel () * 3 *
                                (1 +
                                 0.1 *
                                 ((skill->GetPlayer ()->GetDarklightform () == 3 || skill->GetPlayer ()->GetDarklightform () == 5
                                   || skill->GetPlayer ()->GetDarklightform () == 6) ? skill->GetT2 () : 0)));
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
                return 750;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar3 (skill->GetPlayer ()->GetSkilllevel (4577));
                skill->GetPlayer ()->SetVar2 ((skill->GetPlayer ()->GetCultivation () == 2)
                                              && (skill->GetPlayer ()->GetSkilllevel (4577) > 0) ? 120 : 0);
                skill->SetPlus ((1 + 0.05 * skill->GetT4 ()) * skill->GetPlayer ()->GetLevel () * 3 *
                                (1 +
                                 0.1 *
                                 ((skill->GetPlayer ()->GetDarklightform () == 3 || skill->GetPlayer ()->GetDarklightform () == 5
                                   || skill->GetPlayer ()->GetDarklightform () == 6) ? skill->GetT2 () : 0)));
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
                return 625;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar3 (skill->GetPlayer ()->GetSkilllevel (4577));
                skill->GetPlayer ()->SetVar2 ((skill->GetPlayer ()->GetCultivation () == 2)
                                              && (skill->GetPlayer ()->GetSkilllevel (4577) > 0) ? 120 : 0);
                skill->SetPlus ((1 + 0.10 * skill->GetT4 ()) * skill->GetPlayer ()->GetLevel () * 3 *
                                (1 +
                                 0.1 *
                                 ((skill->GetPlayer ()->GetDarklightform () == 3 || skill->GetPlayer ()->GetDarklightform () == 5
                                   || skill->GetPlayer ()->GetDarklightform () == 6) ? skill->GetT2 () : 0)));
                skill->GetPlayer ()->SetPerform (0);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State5:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 625;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar3 (skill->GetPlayer ()->GetSkilllevel (4577));
                skill->GetPlayer ()->SetVar2 ((skill->GetPlayer ()->GetCultivation () == 2)
                                              && (skill->GetPlayer ()->GetSkilllevel (4577) > 0) ? 120 : 0);
                skill->SetPlus ((1 + 0.15 * skill->GetT4 ()) * skill->GetPlayer ()->GetLevel () * 3 *
                                (1 +
                                 0.1 *
                                 ((skill->GetPlayer ()->GetDarklightform () == 3 || skill->GetPlayer ()->GetDarklightform () == 5
                                   || skill->GetPlayer ()->GetDarklightform () == 6) ? skill->GetT2 () : 0)));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill4014Stub ():SkillStub (4014)
        {
            occupation = 57;
            name = L"¾»Äù¡¤½Ù¡¤Ìý³ð";
            nativename = "¾»Äù¡¤½Ù¡¤Ìý³ð";
            icon = "¾»Äù¡¤½Ù¡¤Ìý³ð.dds";
            maxlevel = 10;
            maxlearn = 9;
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
            talent[3] = 4122;
            talent[4] = 4150;
            talent_size = 5;
            action[0] = "ÐùÔ¯_°µÉäÏß";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "ÐùÔ¯_°µÉäÏß";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ÐùÔ¯_Æï³Ë_°µÉäÏß";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "ÐùÔ¯_Æï³Ë_°µÉäÏß";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
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
            weapon_limit = 9;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
            statestub.push_back (new State3 ());
            statestub.push_back (new State4 ());
            statestub.push_back (new State5 ());
#endif
        }
        virtual ~ Skill4014Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 4012 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetLevel () * 3);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 55, 57, 59, 61, 63, 65, 67, 69, 71 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (17);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (15);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetPlayer ()->GetLevel () * 3, skill->GetLevel (), 10 * skill->GetLevel ());

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
            skill->GetVictim ()->SetRatio (0.1 * skill->GetLevel ());
            skill->GetVictim ()->SetExtracthp (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetT3 () > 0 ? 600 : 0));
            skill->GetVictim ()->SetTime (1500);
            skill->GetVictim ()->SetRatio (0.25 * skill->GetT3 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetSlow (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar2 ()));
            skill->GetVictim ()->SetTime (1500);
            skill->GetVictim ()->SetRatio (0.02 * skill->GetPlayer ()->GetVar3 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetIncfatalratio (1);
            return true;
        }
#endif
    };
}
#endif
