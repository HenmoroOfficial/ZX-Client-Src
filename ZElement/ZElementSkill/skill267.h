#ifndef __CPPGEN_GNET_SKILL267
#define __CPPGEN_GNET_SKILL267
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill267:public Skill
    {
      public:
        enum
        { SKILL_ID = 267 };
          Skill267 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill267Stub:public SkillStub
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
                skill->SetRatio (1 + 0.07 * skill->GetT2 ());
                skill->SetPlus (20 * skill->GetLevel () * skill->GetT0 () +
                                INT (1.42 - 0.05 * skill->GetLevel ()) * (0.015 * skill->GetLevel () * skill->GetLevel () + 5 * skill->GetLevel () +
                                                                          39) + INT (0.05 * skill->GetLevel () +
                                                                                     0.55) * (0.4 * skill->GetLevel () * skill->GetLevel () +
                                                                                              5.2 * skill->GetLevel () + 14));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill267Stub ():SkillStub (267)
        {
            occupation = 10;
            name = L"½µÄ§ÕÈ";
            nativename = "½µÄ§ÕÈ";
            icon = "½µÄ§ÕÈ.dds";
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
            skill_class = 3;
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
            talent[0] = 686;
            talent[1] = 698;
            talent[2] = 702;
            talent_size = 3;
            action[0] = "ÌìÒôËÂ_½µÄ§ÕÈ";
            action[1] = "ÌìÒôËÂ_½µÄ§ÕÈ";
            action[2] = "ÌìÒôËÂ_½µÄ§ÕÈ";
            action[3] = "ÌìÒôËÂ_½µÄ§ÕÈ";
            action[4] = "ÌìÒôËÂ_½µÄ§ÕÈ";
            action[5] = "ÌìÒôËÂ_½µÄ§ÕÈ";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_½µÄ§ÕÈ";
            action[16] = "ÌìÒôËÂ_Æï³Ë_½µÄ§ÕÈ";
            action[17] = "ÌìÒôËÂ_Æï³Ë_½µÄ§ÕÈ";
            action[18] = "ÌìÒôËÂ_Æï³Ë_½µÄ§ÕÈ";
            action[19] = "ÌìÒôËÂ_Æï³Ë_½µÄ§ÕÈ";
            action[20] = "ÌìÒôËÂ_Æï³Ë_½µÄ§ÕÈ";
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
        virtual ~ Skill267Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 0 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 8 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (19);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (10);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 2000 - 1000 * skill->GetT0 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 15, 17, 19, 21, 23, 25, 27, 29, 31 };
            return array[level - 1];
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
                               10,
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (0.015 * skill->GetLevel () * skill->GetLevel () + 5 * skill->GetLevel () + 39) + INT (0.05 * skill->GetLevel () +
                                                                                                                           0.55) * (0.4 *
                                                                                                                                    skill->
                                                                                                                                    GetLevel () *
                                                                                                                                    skill->
                                                                                                                                    GetLevel () +
                                                                                                                                    5.2 *
                                                                                                                                    skill->
                                                                                                                                    GetLevel () +
                                                                                                                                    14)));

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
            skill->GetVictim ()->SetProbability (1.0 * (6 * skill->GetT1 ()));
            skill->GetVictim ()->SetTime (4050 * skill->GetT1 ());
            skill->GetVictim ()->SetRatio (0.03 * skill->GetT1 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecdefence (1);
            return true;
        }
#endif
    };
}
#endif
