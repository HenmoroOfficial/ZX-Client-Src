#ifndef __CPPGEN_GNET_SKILL228
#define __CPPGEN_GNET_SKILL228
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill228:public Skill
    {
      public:
        enum
        { SKILL_ID = 228 };
          Skill228 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill228Stub:public SkillStub
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
                skill->SetCrit (0.01 * skill->GetT3 () * INT (skill->GetLevel () / 9) + 0.01 * skill->GetT3 ());
                skill->SetCrithurt (0.3 * skill->GetT3 () * INT (skill->GetLevel () / 9));
                skill->SetRatio (1 + 0.05 * skill->GetT3 () +
                                 ((skill->GetPlayer ()->GetSkilllevel (222) > 8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (237) >
                                                                                            8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (232) >
                                                                                                          8 ? 1 : 0) +
                                  (skill->GetPlayer ()->GetSkilllevel (228) > 8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (312) >
                                                                                            8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (240) >
                                                                                                          8 ? 1 : 0) +
                                  (skill->GetPlayer ()->GetSkilllevel (541) > 8 ? 1 : 0) + (skill->GetPlayer ()->GetSkilllevel (545) >
                                                                                            8 ? 1 : 0)) * 0.05 * skill->GetT3 () *
                                 norm (skill->GetT3 ()));
                skill->
                    SetPlus ((INT (1.42 - 0.05 * skill->GetLevel ()) * (325 + 27 * skill->GetLevel ()) +
                              INT (0.05 * skill->GetLevel () + 0.55) * (1.2 * skill->GetLevel () * skill->GetLevel () + 24 * skill->GetLevel () +
                                                                        299)) * (1 + 0.2 * skill->GetT0 ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill228Stub ():SkillStub (228)
        {
            occupation = 8;
            name = L"ÆÆÄ§½£Æø";
            nativename = "ÆÆÄ§½£Æø";
            icon = "ÆÆÄ§½£Æø.dds";
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
            talent[0] = 573;
            talent[1] = 574;
            talent[2] = 579;
            talent[3] = 583;
            talent_size = 4;
            action[0] = "ÇàÔÆÃÅ_ÆÆÄ§½£Æø";
            action[1] = "ÇàÔÆÃÅ_ÆÆÄ§½£Æø";
            action[2] = "ÇàÔÆÃÅ_ÆÆÄ§½£Æø";
            action[3] = "ÇàÔÆÃÅ_ÆÆÄ§½£Æø";
            action[4] = "ÇàÔÆÃÅ_ÆÆÄ§½£Æø";
            action[5] = "ÇàÔÆÃÅ_ÆÆÄ§½£Æø";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_ÆÆÄ§½£Æø";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_ÆÆÄ§½£Æø";
            action[17] = "ÇàÔÆÃÅ_Æï³Ë_ÆÆÄ§½£Æø";
            action[18] = "ÇàÔÆÃÅ_Æï³Ë_ÆÆÄ§½£Æø";
            action[19] = "ÇàÔÆÃÅ_Æï³Ë_ÆÆÄ§½£Æø";
            action[20] = "ÇàÔÆÃÅ_Æï³Ë_ÆÆÄ§½£Æø";
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
        virtual ~ Skill228Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 312 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 6 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (19);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0.46 * skill->GetLevel () * skill->GetLevel () + 6.7 * skill->GetLevel () + 159);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6000 - 1000 * skill->GetT2 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 70, 72, 74, 76, 78, 80, 82, 83, 85 };
            return array[level - 1];
        }
        float GetAttackdistance (Skill * skill) const
        {
            return (float) (10);
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
                               INT (0.46 * skill->GetLevel () * skill->GetLevel () + 6.7 * skill->GetLevel () + 159),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) * (325 + 27 * skill->GetLevel ()) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (1.2 * skill->GetLevel () * skill->GetLevel () +
                                                                              24 * skill->GetLevel () + 299)), 12 + 3 * skill->GetLevel (),
                               16 + 4 * skill->GetLevel ());

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
                                                 ((12 + 3 * skill->GetLevel () + 10 * skill->GetT1 ()) * (1 -
                                                                                                          0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetRatio ((0.16 + 0.04 * skill->GetLevel ()) * (1 + 0.5 * skill->GetT0 ()));
            skill->GetVictim ()->SetDrainmagic (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (20 * skill->GetT1 ()));
            skill->GetVictim ()->SetTime (2000);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue ((0.46 * skill->GetLevel () * skill->GetLevel () + 6.7 * skill->GetLevel () + 159) * 0.2 * skill->GetT1 ());
            skill->GetVictim ()->SetMpgen (1);
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
