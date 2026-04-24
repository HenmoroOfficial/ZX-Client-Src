#ifndef __CPPGEN_GNET_SKILL298
#define __CPPGEN_GNET_SKILL298
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill298:public Skill
    {
      public:
        enum
        { SKILL_ID = 298 };
          Skill298 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill298Stub:public SkillStub
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
                skill->SetPlus (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                (0.056 * skill->GetLevel () * skill->GetLevel () + 12 * skill->GetLevel () + 476) + INT (0.05 * skill->GetLevel () +
                                                                                                                         0.55) * (76 +
                                                                                                                                  70 *
                                                                                                                                  skill->GetLevel () -
                                                                                                                                  0.21 *
                                                                                                                                  skill->GetLevel () *
                                                                                                                                  skill->
                                                                                                                                  GetLevel ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill298Stub ():SkillStub (298)
        {
            occupation = 5;
            name = L"Ïú»ê";
            nativename = "Ïú»ê";
            icon = "Ïú»ê.dds";
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
            skill_class = 1;
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
            talent[0] = 727;
            talent[1] = 736;
            talent[2] = 744;
            talent[3] = 748;
            talent_size = 4;
            action[0] = "ºÏ»¶ÅÉ_Ïú»ê";
            action[1] = "ºÏ»¶ÅÉ_Ïú»ê";
            action[2] = "ºÏ»¶ÅÉ_Ïú»ê";
            action[3] = "ºÏ»¶ÅÉ_Ïú»ê";
            action[4] = "ºÏ»¶ÅÉ_Ïú»ê";
            action[5] = "ºÏ»¶ÅÉ_Ïú»ê";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_Ïú»ê";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_Ïú»ê";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_Ïú»ê";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_Ïú»ê";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_Ïú»ê";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_Ïú»ê";
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
            dobless = false;
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
        virtual ~ Skill298Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 288 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1.1 * skill->GetLevel () * skill->GetLevel () + 2.8 * skill->GetLevel () + 121);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 5000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 60, 62, 64, 66, 68, 70, 72, 74, 76 };
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
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (1.1 * skill->GetLevel () * skill->GetLevel () + 2.8 * skill->GetLevel () + 121),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (0.056 * skill->GetLevel () * skill->GetLevel () + 12 * skill->GetLevel () + 476) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (76 + 70 * skill->GetLevel () -
                                                                              0.21 * skill->GetLevel () * skill->GetLevel ())),
                               INT (63 + 5.8 * skill->GetLevel ()), 2.2 + 1.3 * skill->GetLevel ());

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
                                                 (63 + 5.8 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetT0 () + 15 * skill->GetT1 ()));
            skill->GetVictim ()->SetTime ((2000 + 1300 * skill->GetLevel ()) * (1 + 0.2 * skill->GetT1 ()) *
                                          (INT (zrand (100) / (100 - 8 * skill->GetT1 ())) + 1) + 100);
            skill->GetVictim ()->SetSilent (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (8000 * (1 + 0.3 * skill->GetT2 ()) + 100);
            skill->GetVictim ()->SetRatio ((0.10 + 0.03 * skill->GetT3 ()) * (INT (zrand (100) / (100 - 4 * skill->GetT2 ())) + 1));
            skill->GetVictim ()->SetInchurt2 (1);
            return true;
        }
#endif
    };
}
#endif
