#ifndef __CPPGEN_GNET_SKILL4033
#define __CPPGEN_GNET_SKILL4033
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4033:public Skill
    {
      public:
        enum
        { SKILL_ID = 4033 };
          Skill4033 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4033Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar4 (skill->GetPlayer ()->GetSkilllevel (4574));
                skill->GetPlayer ()->SetVar3 ((skill->GetPlayer ()->GetCultivation () == 1)
                                              && (skill->GetPlayer ()->GetSkilllevel (4574) > 0) ? 120 : 0);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetLevel ());
                skill->GetPlayer ()->SetVar1 (1);
                skill->SetPlus ((skill->GetPlayer ()->GetLevel () * 5 + skill->GetPlayer ()->GetMaxmp () * skill->GetLevel () / 100) *
                                (1 +
                                 0.1 *
                                 ((skill->GetPlayer ()->GetDarklightform () == 4 || skill->GetPlayer ()->GetDarklightform () == 5
                                   || skill->GetPlayer ()->GetDarklightform () == 6) ? skill->GetT2 () : 0)));
                skill->GetPlayer ()->SetDarklight (skill->GetPlayer ()->GetDarklight () +
                                                   (skill->GetLevel () + skill->GetT0 ()) * (skill->GetPlayer ()->GetDarklightform () == 5 ? -1 : 1));
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
                skill->GetPlayer ()->SetVar4 (skill->GetPlayer ()->GetSkilllevel (4574));
                skill->GetPlayer ()->SetVar3 ((skill->GetPlayer ()->GetCultivation () == 1)
                                              && (skill->GetPlayer ()->GetSkilllevel (4574) > 0) ? 120 : 0);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetLevel ());
                skill->GetPlayer ()->SetVar1 (2);
                skill->SetPlus ((skill->GetPlayer ()->GetLevel () * 5 + skill->GetPlayer ()->GetMaxmp () * skill->GetLevel () / 100) *
                                (1 +
                                 0.1 *
                                 ((skill->GetPlayer ()->GetDarklightform () == 4 || skill->GetPlayer ()->GetDarklightform () == 5
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
                return 1150;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar4 (skill->GetPlayer ()->GetSkilllevel (4574));
                skill->GetPlayer ()->SetVar3 ((skill->GetPlayer ()->GetCultivation () == 1)
                                              && (skill->GetPlayer ()->GetSkilllevel (4574) > 0) ? 120 : 0);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetLevel ());
                skill->GetPlayer ()->SetVar1 (3);
                skill->SetPlus ((skill->GetPlayer ()->GetLevel () * 5 + skill->GetPlayer ()->GetMaxmp () * skill->GetLevel () / 100) *
                                (1 +
                                 0.1 *
                                 ((skill->GetPlayer ()->GetDarklightform () == 4 || skill->GetPlayer ()->GetDarklightform () == 5
                                   || skill->GetPlayer ()->GetDarklightform () == 6) ? skill->GetT2 () : 0)));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill4033Stub ():SkillStub (4033)
        {
            occupation = 59;
            name = L"ËéÄù¡¤î¸¡¤Ê¢·ç";
            nativename = "ËéÄù¡¤î¸¡¤Ê¢·ç";
            icon = "ËéÄù¡¤î¸¡¤Ê¢·ç.dds";
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
            darkLightType = 2;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 1;
            talent[0] = 4132;
            talent[1] = 4133;
            talent[2] = 4134;
            talent[3] = 4141;
            talent[4] = 4152;
            talent_size = 5;
            action[0] = "ĞùÔ¯_¹â·ç±©";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "ĞùÔ¯_¹â·ç±©";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ĞùÔ¯_Æï³Ë_¹â·ç±©";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "ĞùÔ¯_Æï³Ë_¹â·ç±©";
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
        virtual ~ Skill4033Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 4031 };
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
            return 2650;
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
            return _snwprintf (buffer, length, format, skill->GetPlayer ()->GetLevel () * 5, skill->GetLevel (), 20, skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetT3 () > 0 ? (skill->GetPlayer ()->GetVar2 () * 3 + 20 * skill->GetT3 ()) : 0));
            skill->GetVictim ()->SetTime (3000);
            skill->GetVictim ()->SetSilent (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetType () == 0 ? skill->GetPlayer ()->GetVar3 () : 0));
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetMaxmp () * 0.01 * skill->GetPlayer ()->GetVar4 ());
            skill->GetVictim ()->SetDirecthurt (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () == 3 ? 120 : 0));
            skill->GetVictim ()->SetTime (12000);
            skill->GetVictim ()->SetRatio (15);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (54000);
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
