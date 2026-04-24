#ifndef __CPPGEN_GNET_SKILL1134
#define __CPPGEN_GNET_SKILL1134
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1134:public Skill
    {
      public:
        enum
        { SKILL_ID = 1134 };
          Skill1134 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1134Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar3 ((skill->GetPlayer ()->GetCultivation () == 1 ? 1 : 0));
                skill->GetPlayer ()->SetVar4 ((skill->GetPlayer ()->GetCultivation () == 4 ? 1 : 0));
                skill->GetPlayer ()->SetVar5 ((skill->GetPlayer ()->GetCultivation () == 2 ? 1 : 0));
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetSaint ());
                skill->SetRatio (0.48 + 0.02 * skill->GetLevel () +
                                 0.02 * (skill->GetT4 () >=
                                         skill->GetT5 ()? (skill->GetT4 () >=
                                                           skill->GetT6 ()? skill->GetT4 () : skill->GetT6 ()) : (skill->GetT5 () >=
                                                                                                                  skill->GetT6 ()? skill->
                                                                                                                  GetT5 () : skill->GetT6 ())));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1134Stub ():SkillStub (1134)
        {
            occupation = 29;
            name = L"天选道怨";
            nativename = "天选道怨";
            icon = "天选道怨.dds";
            maxlevel = 20;
            maxlearn = 2;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 4;
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
            skill_class = 4;
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
            talent[0] = 1185;
            talent[1] = 1187;
            talent[2] = 1189;
            talent[3] = 2062;
            talent[4] = 2947;
            talent[5] = 4112;
            talent[6] = 4530;
            talent_size = 7;
            action[0] = "天选道怨";
            action[1] = "天选道怨";
            action[2] = "天选道怨";
            action[3] = "天选道怨";
            action[4] = "天选道怨";
            action[5] = "天选道怨";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "天选道怨";
            action[16] = "天选道怨";
            action[17] = "天选道怨";
            action[18] = "天选道怨";
            action[19] = "天选道怨";
            action[20] = "天选道怨";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "火焰之环";
            rangetype = 3;
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
        virtual ~ Skill1134Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1131 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (24);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (625 + 625 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 12000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[2] = { 139, 141 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (20);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (22);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (20);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               625 + 625 * skill->GetLevel (),
                               25,
                               48 + 2 * skill->GetLevel (),
                               20,
                               60,
                               2 * skill->GetLevel (),
                               5 * skill->GetLevel (),
                               2 * skill->GetLevel (),
                               2000 * skill->GetLevel (),
                               5 * skill->GetLevel (),
                               5 * skill->GetLevel (),
                               3 * skill->GetLevel (),
                               skill->GetT4 () > 0 || skill->GetPlayer ()->GetSkilllevel (4530) > 0
                               || skill->GetPlayer ()->GetSkilllevel (4112) > 0 ? table.Find (11343) : table.Find (11344));

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
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetSglevel () ==
                                                  0 ? 100 * (1 - 0.002 * skill->GetPlayer ()->GetLevel () + 0.03 * skill->GetT3 ()) * (1 +
                                                                                                                                       0.1 *
                                                                                                                                       skill->
                                                                                                                                       GetT2 ())
                                                  : ((30 + skill->GetT2 ()) * skill->GetPlayer ()->GetVar3 () +
                                                     (30 + skill->GetT2 ()) * skill->GetPlayer ()->GetVar4 () + (40 + skill->GetT3 () +
                                                                                                                 skill->GetT2 ()) *
                                                     skill->GetPlayer ()->GetVar5 () +
                                                     ((skill->GetPlayer ()->GetVar1 () - skill->GetPlayer ()->GetMaxatk ()) / 200 >
                                                      20 ? 20 : ((skill->GetPlayer ()->GetVar1 () - skill->GetPlayer ()->GetMaxatk ()) / 200 <
                                                                 -20 ? -20 : (skill->GetPlayer ()->GetVar1 () -
                                                                              skill->GetPlayer ()->GetMaxatk ()) / 200)) +
                                                     (skill->GetPlayer ()->GetVar2 () - skill->GetPlayer ()->GetSaint ()) / 10 <
                                                     25 ? 25 : ((30 + skill->GetT2 ()) * skill->GetPlayer ()->GetVar3 () +
                                                                (30 + skill->GetT2 ()) * skill->GetPlayer ()->GetVar4 () + (40 + skill->GetT3 () +
                                                                                                                            skill->GetT2 ()) *
                                                                skill->GetPlayer ()->GetVar5 () +
                                                                ((skill->GetPlayer ()->GetVar1 () - skill->GetPlayer ()->GetMaxatk ()) / 200 >
                                                                 20 ? 20 : ((skill->GetPlayer ()->GetVar1 () - skill->GetPlayer ()->GetMaxatk ()) /
                                                                            200 <
                                                                            -20 ? -20 : (skill->GetPlayer ()->GetVar1 () -
                                                                                         skill->GetPlayer ()->GetMaxatk ()) / 200)) +
                                                                (skill->GetPlayer ()->GetVar2 () - skill->GetPlayer ()->GetSaint ()) / 10 >
                                                                65 * skill->GetPlayer ()->GetVar3 () + 65 * skill->GetPlayer ()->GetVar4 () +
                                                                85 * skill->GetPlayer ()->GetVar5 ()? 65 * skill->GetPlayer ()->GetVar3 () +
                                                                65 * skill->GetPlayer ()->GetVar4 () +
                                                                85 * skill->GetPlayer ()->GetVar5 () : ((30 + skill->GetT2 ()) *
                                                                                                        skill->GetPlayer ()->GetVar3 () + (30 +
                                                                                                                                           skill->
                                                                                                                                           GetT2 ()) *
                                                                                                        skill->GetPlayer ()->GetVar4 () + (40 +
                                                                                                                                           skill->
                                                                                                                                           GetT3 () +
                                                                                                                                           skill->
                                                                                                                                           GetT2 ()) *
                                                                                                        skill->GetPlayer ()->GetVar5 () +
                                                                                                        ((skill->GetPlayer ()->GetVar1 () -
                                                                                                          skill->GetPlayer ()->GetMaxatk ()) / 200 >
                                                                                                         20 ? 20
                                                                                                         : ((skill->GetPlayer ()->GetVar1 () -
                                                                                                             skill->GetPlayer ()->GetMaxatk ()) /
                                                                                                            200 <
                                                                                                            -20 ? -20 : (skill->GetPlayer ()->
                                                                                                                         GetVar1 () -
                                                                                                                         skill->GetPlayer ()->
                                                                                                                         GetMaxatk ()) / 200)) +
                                                                                                        (skill->GetPlayer ()->GetVar2 () -
                                                                                                         skill->GetPlayer ()->GetSaint ()) / 10)))));
            skill->GetVictim ()->SetTime (60000 - 2000 * skill->GetT2 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount ((2 * skill->GetLevel () * skill->GetPlayer ()->GetMaxhp ()) * (1 + 0.1 * skill->GetT0 ()) +
                                            0.005 * skill->GetT0 () * skill->GetLevel () * skill->GetPlayer ()->GetHp ());
            skill->GetVictim ()->SetValue (2000 * skill->GetLevel ());
            skill->GetVictim ()->SetHpleak3 (1);
            skill->GetVictim ()->SetProbability (1.0 * (5 * skill->GetLevel () * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetDrainmagic (1);
            skill->GetVictim ()->SetProbability (1.0 * (5 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (3000 * skill->GetLevel () + 100 + 2000 * skill->GetT1 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (10000);
            skill->GetVictim ()->SetSubdefence (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (5 * skill->GetLevel () +
                                                  2 * (skill->GetT4 () >=
                                                       skill->GetT5 ()? (skill->GetT4 () >=
                                                                         skill->GetPlayer ()->GetSkilllevel (4530) ? skill->GetT4 () : skill->
                                                                         GetPlayer ()->GetSkilllevel (4530)) : (skill->GetT5 () >=
                                                                                                                skill->GetPlayer ()->
                                                                                                                GetSkilllevel (4530) ? skill->
                                                                                                                GetT5 () : skill->GetPlayer ()->
                                                                                                                GetSkilllevel (4530)))));
            skill->GetVictim ()->SetTime (2000 * skill->GetLevel () + 100 + 2000 * skill->GetT1 ());
            skill->GetVictim ()->SetRatio (0.2);
            skill->GetVictim ()->SetValue (0.2);
            skill->GetVictim ()->SetRandcurse (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (8100);
            skill->GetVictim ()->SetBuffid (5);
            skill->GetVictim ()->SetValue (0.004 * skill->GetPlayer ()->GetMp () + 1);
            skill->GetVictim ()->SetAdddefence (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (25);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
