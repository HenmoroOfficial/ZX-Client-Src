#ifndef __CPPGEN_GNET_SKILL222
#define __CPPGEN_GNET_SKILL222
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill222:public Skill
    {
      public:
        enum
        { SKILL_ID = 222 };
          Skill222 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill222Stub:public SkillStub
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
                skill->SetCrit (0.01 * skill->GetT2 () * int (skill->GetLevel () / 9) + 0.01 * skill->GetT2 ());
                  skill->SetCrithurt (0.3 * skill->GetT2 () * INT (skill->GetLevel () / 9));
                  skill->SetRatio (1 +
                                   ((skill->GetPlayer ()->GetSkilllevel (237) > 8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (222) >
                                                                                              8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (232) >
                                                                                                            8 ? 1 : 0) +
                                    (skill->GetPlayer ()->GetSkilllevel (228) > 8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (312) >
                                                                                              8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (240) >
                                                                                                            8 ? 1 : 0) +
                                    (skill->GetPlayer ()->GetSkilllevel (541) > 8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (545) >
                                                                                              8 ? 1 : 0)) * 0.05 * skill->GetT2 () *
                                   norm (skill->GetT2 ()) + 0.05 * skill->GetT2 () +
                                   0.04 * INT (skill->GetPlayer ()->GetMp () / (0.95 - 0.1 * skill->GetT3 ()) / skill->GetPlayer ()->GetMaxmp ()));
                  skill->
                    SetPlus ((INT (1.42 - 0.05 * skill->GetLevel ()) * (60 + 8 * skill->GetLevel ()) +
                              INT (0.05 * skill->GetLevel () + 0.55) * (0.72 * skill->GetLevel () * skill->GetLevel () + 8.9 * skill->GetLevel () +
                                                                        18)) * (1 + 0.15 * skill->GetT0 ()));
                  skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill222Stub ():SkillStub (222)
        {
            occupation = 7;
            name = L"Óù½£¾÷";
            nativename = "Óù½£¾÷";
            icon = "Óù½£¾÷.dds";
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
            talent[0] = 569;
            talent[1] = 579;
            talent[2] = 583;
            talent[3] = 612;
            talent_size = 4;
            action[0] = "ÇàÔÆÃÅ_Óù½£¾ö";
            action[1] = "ÇàÔÆÃÅ_Óù½£¾ö";
            action[2] = "ÇàÔÆÃÅ_Óù½£¾ö";
            action[3] = "ÇàÔÆÃÅ_Óù½£¾ö";
            action[4] = "ÇàÔÆÃÅ_Óù½£¾ö";
            action[5] = "ÇàÔÆÃÅ_Óù½£¾ö";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_Óù½£¾ö";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_Óù½£¾ö";
            action[17] = "ÇàÔÆÃÅ_Æï³Ë_Óù½£¾ö";
            action[18] = "ÇàÔÆÃÅ_Æï³Ë_Óù½£¾ö";
            action[19] = "ÇàÔÆÃÅ_Æï³Ë_Óù½£¾ö";
            action[20] = "ÇàÔÆÃÅ_Æï³Ë_Óù½£¾ö";
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
#endif
        }
        virtual ~ Skill222Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 460 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (19);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (39 + 2 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6000 - 1000 * skill->GetT1 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 25, 27, 29, 31, 33, 35, 37, 39, 41 };
            return array[level - 1];
        }
        float GetAttackdistance (Skill * skill) const
        {
            return (float) (6.5 + 0.5 * skill->GetLevel ());
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (16);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               39 + 2 * skill->GetLevel (),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) * (60 + 8 * skill->GetLevel ()) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (0.72 * skill->GetLevel () * skill->GetLevel () +
                                                                              8.9 * skill->GetLevel () + 18)));

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
            skill->GetVictim ()->SetBuffid (INT (skill->GetPlayer ()->GetMp () / (0.95 - 0.1 * skill->GetT3 ()) / skill->GetPlayer ()->GetMaxmp ()));
            skill->GetVictim ()->SetUniqprompt (1);
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
