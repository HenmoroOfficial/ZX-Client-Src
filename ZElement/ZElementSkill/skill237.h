#ifndef __CPPGEN_GNET_SKILL237
#define __CPPGEN_GNET_SKILL237
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill237:public Skill
    {
      public:
        enum
        { SKILL_ID = 237 };
          Skill237 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill237Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1600;
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrit (0.01 * skill->GetT2 () * INT (skill->GetLevel () / 9) + 0.01 * skill->GetT2 ());
                skill->SetCrithurt (0.3 * skill->GetT2 () * INT (skill->GetLevel () / 9));
                skill->SetRatio (1 + 0.05 * skill->GetT2 () +
                                 0.12 * INT (skill->GetPlayer ()->GetMp () / (0.95 - 0.1 * skill->GetT3 ()) / skill->GetPlayer ()->GetMaxmp ()) +
                                 ((skill->GetPlayer ()->GetSkilllevel (222) > 8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (232) >
                                                                                            8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (228) >
                                                                                                          8 ? 1 : 0) +
                                  (skill->GetPlayer ()->GetSkilllevel (237) > 8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (312) >
                                                                                            8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (240) >
                                                                                                          8 ? 1 : 0) +
                                  (skill->GetPlayer ()->GetSkilllevel (541) > 8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (545) >
                                                                                            8 ? 1 : 0)) * 0.05 * skill->GetT2 () *
                                 norm (skill->GetT2 ()));
                skill->SetPlus (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                (901 + 114 * skill->GetLevel () - 6.4 * skill->GetLevel () * skill->GetLevel ()) + INT (0.05 * skill->GetLevel () +
                                                                                                                        0.55) * (1.8 *
                                                                                                                                 skill->GetLevel () *
                                                                                                                                 skill->GetLevel () +
                                                                                                                                 77 *
                                                                                                                                 skill->GetLevel () +
                                                                                                                                 636) +
                                25 * skill->GetLevel () * skill->GetT0 ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill237Stub ():SkillStub (237)
        {
            occupation = 9;
            name = L"Å­½£¿ñ»¨";
            nativename = "Å­½£¿ñ»¨";
            icon = "Å­½£¿ñ»¨.dds";
            maxlevel = 20;
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
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
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
            talent[0] = 576;
            talent[1] = 579;
            talent[2] = 583;
            talent[3] = 612;
            talent_size = 4;
            action[0] = "ÇàÔÆÃÅ_Å­½£¿ñ»¨";
            action[1] = "ÇàÔÆÃÅ_Å­½£¿ñ»¨";
            action[2] = "ÇàÔÆÃÅ_Å­½£¿ñ»¨";
            action[3] = "ÇàÔÆÃÅ_Å­½£¿ñ»¨";
            action[4] = "ÇàÔÆÃÅ_Å­½£¿ñ»¨";
            action[5] = "ÇàÔÆÃÅ_Å­½£¿ñ»¨";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_Å­½£¿ñ»¨";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_Å­½£¿ñ»¨";
            action[17] = "ÇàÔÆÃÅ_Æï³Ë_Å­½£¿ñ»¨";
            action[18] = "ÇàÔÆÃÅ_Æï³Ë_Å­½£¿ñ»¨";
            action[19] = "ÇàÔÆÃÅ_Æï³Ë_Å­½£¿ñ»¨";
            action[20] = "ÇàÔÆÃÅ_Æï³Ë_Å­½£¿ñ»¨";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "¼¼ÄÜ¹¥»÷1";
            rangetype = 0;
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
        virtual ~ Skill237Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 461 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (21);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1.4 * skill->GetLevel () * skill->GetLevel () + 0.72 * skill->GetLevel () + 364);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 8000 - 1000 * skill->GetT1 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 85, 87, 89, 91, 93, 95, 97, 99, 101 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (18);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (1.4 * skill->GetLevel () * skill->GetLevel () + 0.72 * skill->GetLevel () + 364),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (901 + 114 * skill->GetLevel () - 6.4 * skill->GetLevel () * skill->GetLevel ()) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (1.8 * skill->GetLevel () * skill->GetLevel () +
                                                                              77 * skill->GetLevel () + 636)), 4 * (85 * skill->GetLevel () + 739));

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
            skill->GetVictim ()->SetProbability (1.0 * (100 * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetTime (8000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (4 * (85 * skill->GetLevel () + 739));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetBuffid (INT (skill->GetPlayer ()->GetMp () / (0.95 - 0.1 * skill->GetT3 ()) / skill->GetPlayer ()->GetMaxmp ()));
            skill->GetVictim ()->SetUniqprompt (1);
            return true;
        }
#endif
    };
}
#endif
