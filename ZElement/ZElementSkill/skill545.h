#ifndef __CPPGEN_GNET_SKILL545
#define __CPPGEN_GNET_SKILL545
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill545:public Skill
    {
      public:
        enum
        { SKILL_ID = 545 };
          Skill545 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill545Stub:public SkillStub
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
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrithurt (0.3 * skill->GetT2 () * INT (skill->GetLevel () / 9));
                skill->SetCrit (0.01 * skill->GetT2 () + 0.01 * skill->GetT2 () * int (skill->GetLevel () / 9));
                  skill->SetRatio (1 + 0.01 * skill->GetLevel () + 0.05 * skill->GetT2 () +
                                   ((skill->GetPlayer ()->GetSkilllevel (222) > 8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (232) >
                                                                                              8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (228) >
                                                                                                            8 ? 1 : 0) +
                                    (skill->GetPlayer ()->GetSkilllevel (312) > 8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (240) >
                                                                                              8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (541) >
                                                                                                            8 ? 1 : 0) +
                                    (skill->GetPlayer ()->GetSkilllevel (545) > 8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (237) >
                                                                                              8 ? 1 : 0)) * 0.05 * skill->GetT2 () *
                                   norm (skill->GetT2 ()));
                  skill->SetPlus (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                  (1230 + 105 * skill->GetLevel () - 5.3 * skill->GetLevel () * skill->GetLevel ()) + INT (0.05 * skill->GetLevel () +
                                                                                                                           0.55) * (1126 +
                                                                                                                                    101 *
                                                                                                                                    skill->
                                                                                                                                    GetLevel ()));
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
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrithurt (0.3 * skill->GetT2 () * INT (skill->GetLevel () / 9));
                skill->SetCrit (0.01 * skill->GetT2 () * INT (skill->GetLevel () / 9) + 0.01 * skill->GetT2 ());
                skill->SetRatio (1 + 0.01 * skill->GetLevel () + 0.05 * skill->GetT2 () +
                                 ((skill->GetPlayer ()->GetSkilllevel (222) > 8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (232) >
                                                                                            8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (228) >
                                                                                                          8 ? 1 : 0) +
                                  (skill->GetPlayer ()->GetSkilllevel (312) > 8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (240) >
                                                                                            8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (541) >
                                                                                                          8 ? 1 : 0) +
                                  (skill->GetPlayer ()->GetSkilllevel (545) > 8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (237) >
                                                                                            8 ? 1 : 0)) * 0.05 * skill->GetT2 () *
                                 norm (skill->GetT2 ()));
                skill->SetPlus (60 * skill->GetT1 () * skill->GetLevel () +
                                INT (1.42 - 0.05 * skill->GetLevel ()) * (1230 + 105 * skill->GetLevel () -
                                                                          5.3 * skill->GetLevel () * skill->GetLevel ()) +
                                INT (0.05 * skill->GetLevel () + 0.55) * (1126 + 101 * skill->GetLevel ()));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill545Stub ():SkillStub (545)
        {
            occupation = 19;
            name = L"Æß½ÙÕ¶Áú¾÷";
            nativename = "Æß½ÙÕ¶Áú¾÷";
            icon = "Æß½ÙÕ¶Áú¾÷.dds";
            maxlevel = 18;
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
            skill_class = 2;
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
            talent[0] = 579;
            talent[1] = 581;
            talent[2] = 583;
            talent_size = 3;
            action[0] = "ÇàÔÆÃÅ_Æß½ÙÕ¶Áú¾÷";
            action[1] = "ÇàÔÆÃÅ_Æß½ÙÕ¶Áú¾÷";
            action[2] = "ÇàÔÆÃÅ_Æß½ÙÕ¶Áú¾÷";
            action[3] = "ÇàÔÆÃÅ_Æß½ÙÕ¶Áú¾÷";
            action[4] = "ÇàÔÆÃÅ_Æß½ÙÕ¶Áú¾÷";
            action[5] = "ÇàÔÆÃÅ_Æß½ÙÕ¶Áú¾÷";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_Æß½ÙÕ¶Áú¾÷";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_Æß½ÙÕ¶Áú¾÷";
            action[17] = "ÇàÔÆÃÅ_Æï³Ë_Æß½ÙÕ¶Áú¾÷";
            action[18] = "ÇàÔÆÃÅ_Æï³Ë_Æß½ÙÕ¶Áú¾÷";
            action[19] = "ÇàÔÆÃÅ_Æï³Ë_Æß½ÙÕ¶Áú¾÷";
            action[20] = "ÇàÔÆÃÅ_Æï³Ë_Æß½ÙÕ¶Áú¾÷";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
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
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
            statestub.push_back (new State3 ());
#endif
        }
        virtual ~ Skill545Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 541 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 6 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (19 + 0.5 * skill->GetLevel ());
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1.78 * skill->GetLevel () * skill->GetLevel () + 13 * skill->GetLevel () + 580);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6000 - 1000 * skill->GetT0 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 125, 125, 125, 125, 125, 125, 125, 125, 125 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (16 + 0.5 * skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10.7 + 0.5 * skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (1.78 * skill->GetLevel () * skill->GetLevel () + 13 * skill->GetLevel () + 580),
                               skill->GetLevel (),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (1230 + 105 * skill->GetLevel () - 5.3 * skill->GetLevel () * skill->GetLevel ()) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (1126 + 101 * skill->GetLevel ())), 33, 12, skill->GetLevel (),
                               0.5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (33));
            skill->GetVictim ()->SetTime (12000 + 100);
            skill->GetVictim ()->SetRatio (0.01 * skill->GetLevel ());
            skill->GetVictim ()->SetAttack2hp (1);
            return true;
        }
#endif
    };
}
#endif
