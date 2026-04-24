#ifndef __CPPGEN_GNET_SKILL4873
#define __CPPGEN_GNET_SKILL4873
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4873:public Skill
    {
      public:
        enum
        { SKILL_ID = 4873 };
          Skill4873 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4873Stub:public SkillStub
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
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                float cul = (skill->GetPlayer ()->GetCultivation () == 2 ? skill->GetPlayer ()->GetSkilllevel (5256) * 0.1 : 0);
                int dam =
                    (skill->GetPlayer ()->GetHasbuff (4274) ? skill->GetPlayer ()->GetRage () * 0.01 * skill->GetPlayer ()->GetSkilllevel (4852) : 0);
                  skill->SetPlus (640 + 80 * skill->GetLevel () + dam * cul);
                  skill->SetPlus2 (dam * (1 - cul));
                  skill->SetRatio (1 + (skill->GetPlayer ()->GetCultivation () == 2 ? skill->GetPlayer ()->GetSkilllevel (5260) * 0.05 : 0));
                  skill->
                    SetCrit ((skill->GetPlayer ()->
                              GetHasbuff (4274) ? (1.0 * skill->GetPlayer ()->GetRage () / skill->GetPlayer ()->GetMaxhp () /
                                                   (30 - 5 * skill->GetPlayer ()->GetSkilllevel (4852))) : 0) +
                             1.0 * (skill->GetPlayer ()->GetMaxhp () - skill->GetPlayer ()->GetHp ()) / skill->GetPlayer ()->GetMaxhp ());
                  skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State3:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 100;
            }
            void Calculate (Skill * skill) const
            {
                float cul = (skill->GetPlayer ()->GetCultivation () == 2 ? skill->GetPlayer ()->GetSkilllevel (5256) * 0.1 : 0);
                int dam =
                    (skill->GetPlayer ()->GetHasbuff (4274) ? skill->GetPlayer ()->GetRage () * 0.01 * skill->GetPlayer ()->GetSkilllevel (4852) : 0);
                  skill->SetPlus (skill->GetPlayer ()->GetIscrit ()? (640 + 80 * skill->GetLevel () + dam * cul) : 0);
                  skill->SetPlus2 (skill->GetPlayer ()->GetIscrit ()? dam * (1 - cul) : 0);
                  skill->SetRatio (skill->GetPlayer ()->
                                   GetIscrit ()? (1 +
                                                  (skill->GetPlayer ()->GetCultivation () ==
                                                   2 ? skill->GetPlayer ()->GetSkilllevel (5260) * 0.05 : 0)) : 0);
                  skill->SetCrit (skill->GetPlayer ()->
                                  GetIscrit ()? (((skill->GetPlayer ()->
                                                   GetHasbuff (4274) ? (1.0 * skill->GetPlayer ()->GetRage () / skill->GetPlayer ()->GetMaxhp () /
                                                                        (30 - 5 * skill->GetPlayer ()->GetSkilllevel (4852))) : 0)) +
                                                 1.0 * (skill->GetPlayer ()->GetMaxhp () -
                                                        skill->GetPlayer ()->GetHp ()) / skill->GetPlayer ()->GetMaxhp () *
                                                 (skill->GetPlayer ()->GetCultivation () ==
                                                  2 ? skill->GetPlayer ()->GetSkilllevel (5260) * 0.05 : 0)) : -10);
                  skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State4:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 400;
            }
            bool Quit (Skill * skill) const
            {
                return skill->GetPlayer ()->GetIscrit () == 0;
            }
            void Calculate (Skill * skill) const
            {
                float cul = (skill->GetPlayer ()->GetCultivation () == 2 ? skill->GetPlayer ()->GetSkilllevel (5256) * 0.1 : 0);
                int dam =
                    (skill->GetPlayer ()->GetHasbuff (4274) ? skill->GetPlayer ()->GetRage () * 0.01 * skill->GetPlayer ()->GetSkilllevel (4852) : 0);
                  skill->SetPlus (640 + 80 * skill->GetLevel () + dam * cul);
                  skill->SetPlus2 (dam * (1 - cul));
                  skill->SetRatio (1 + (skill->GetPlayer ()->GetCultivation () == 2 ? skill->GetPlayer ()->GetSkilllevel (5260) * 0.05 : 0));
                  skill->
                    SetCrit ((skill->GetPlayer ()->
                              GetHasbuff (4274) ? (1.0 * skill->GetPlayer ()->GetRage () / skill->GetPlayer ()->GetMaxhp () /
                                                   (30 - 5 * skill->GetPlayer ()->GetSkilllevel (4852))) : 0) +
                             1.0 * (skill->GetPlayer ()->GetMaxhp () -
                                    skill->GetPlayer ()->GetHp ()) / skill->GetPlayer ()->GetMaxhp () * (skill->GetPlayer ()->GetCultivation () ==
                                                                                                         2 ? skill->GetPlayer ()->
                                                                                                         GetSkilllevel (5260) * 0.05 : 0));
                  skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State5:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                float cul = (skill->GetPlayer ()->GetCultivation () == 2 ? skill->GetPlayer ()->GetSkilllevel (5256) * 0.1 : 0);
                int dam =
                    (skill->GetPlayer ()->GetHasbuff (4274) ? skill->GetPlayer ()->GetRage () * 0.01 * skill->GetPlayer ()->GetSkilllevel (4852) : 0);
                  skill->SetPlus (640 + 80 * skill->GetLevel () + dam * cul);
                  skill->SetPlus2 (dam * (1 - cul));
                  skill->SetRatio (1 + (skill->GetPlayer ()->GetCultivation () == 2 ? skill->GetPlayer ()->GetSkilllevel (5260) * 0.05 : 0));
                  skill->
                    SetCrit ((skill->GetPlayer ()->
                              GetHasbuff (4274) ? (1.0 * skill->GetPlayer ()->GetRage () / skill->GetPlayer ()->GetMaxhp () /
                                                   (30 - 5 * skill->GetPlayer ()->GetSkilllevel (4852))) : 0) +
                             1.0 * (skill->GetPlayer ()->GetMaxhp () -
                                    skill->GetPlayer ()->GetHp ()) / skill->GetPlayer ()->GetMaxhp () * (skill->GetPlayer ()->GetCultivation () ==
                                                                                                         2 ? skill->GetPlayer ()->
                                                                                                         GetSkilllevel (5260) * 0.05 : 0));
                  skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill4873Stub ():SkillStub (4873)
        {
            occupation = 111;
            name = L"±³Ë®";
            nativename = "±³Ë®";
            icon = "±³Ë®.dds";
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
            skill_class = 13;
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
            talent[0] = 5219;
            talent_size = 1;
            action[0] = "Ó¢ÕĞ_¿ñÕ½";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "0";
            action[14] = "Ó¢ÕĞ_¿ñÕ½";
            action[15] = "0";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "0";
            action[29] = "Ó¢ÕĞ_¿ñÕ½";
            action[30] = "0";
            rangetype = 0;
            doenchant = false;
            dobless = true;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = 14;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
            statestub.push_back (new State3 ());
            statestub.push_back (new State4 ());
            statestub.push_back (new State5 ());
#endif
        }
        virtual ~ Skill4873Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 4870 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        int GetHpcost (Skill * skill) const
        {
            return (int) (400);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 10);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 115, 117, 119, 121, 123, 125, 125, 125, 125 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 5);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 640 + 80 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetSkilllevel (4846) > 0 ? 120 : 0));
            skill->GetVictim ()->SetTime (10000 + skill->GetPlayer ()->GetSkilllevel (5215) * 1000);
            skill->GetVictim ()->SetRatio (0.02 * skill->GetPlayer ()->GetSkilllevel (4846));
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetIncattack (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetRatio (4275);
            skill->GetVictim ()->SetAmount (4276);
            skill->GetVictim ()->SetValue (4277);
            skill->GetVictim ()->SetDispel (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetIscrit () > 0 && skill->GetT0 () > 0 ? 120 : 0));
            skill->GetVictim ()->SetTime (10000 + skill->GetPlayer ()->GetSkilllevel (5215) * 1000);
            skill->GetVictim ()->SetRatio (0.1 * skill->GetT0 ());
            skill->GetVictim ()->SetValue (0.1 * skill->GetT0 ());
            skill->GetVictim ()->SetSwift (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetSkilllevel (5213) > 0 ? 120 : 0));
            skill->GetVictim ()->SetTime (10000 + skill->GetPlayer ()->GetSkilllevel (5215) * 1000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (0.5 * skill->GetPlayer ()->GetSkilllevel (5213));
            skill->GetVictim ()->SetAddspeed (1);
            return true;
        }
#endif
    };
}
#endif
