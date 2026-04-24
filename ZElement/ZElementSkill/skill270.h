#ifndef __CPPGEN_GNET_SKILL270
#define __CPPGEN_GNET_SKILL270
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill270:public Skill
    {
      public:
        enum
        { SKILL_ID = 270 };
          Skill270 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill270Stub:public SkillStub
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
                skill->SetRatio (1 + 0.05 * skill->GetT1 ());
                skill->SetPlus (INT (1.42 - 0.05 * skill->GetLevel ()) * (53 + 5 * skill->GetLevel ()) +
                                INT (0.05 * skill->GetLevel () + 0.55) * (0.43 * skill->GetLevel () * skill->GetLevel () + 1.8 * skill->GetLevel () +
                                                                          58));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill270Stub ():SkillStub (270)
        {
            occupation = 10;
            name = L"Ê¨×Óºð";
            nativename = "Ê¨×Óºð";
            icon = "Ê¨×Óºð.dds";
            maxlevel = 20;
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
            talent[0] = 672;
            talent[1] = 681;
            talent_size = 2;
            action[0] = "ÌìÒôËÂ_Ê¨×Óºð";
            action[1] = "ÌìÒôËÂ_Ê¨×Óºð";
            action[2] = "ÌìÒôËÂ_Ê¨×Óºð";
            action[3] = "ÌìÒôËÂ_Ê¨×Óºð";
            action[4] = "ÌìÒôËÂ_Ê¨×Óºð";
            action[5] = "ÌìÒôËÂ_Ê¨×Óºð";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_Ê¨×Óºð";
            action[16] = "ÌìÒôËÂ_Æï³Ë_Ê¨×Óºð";
            action[17] = "ÌìÒôËÂ_Æï³Ë_Ê¨×Óºð";
            action[18] = "ÌìÒôËÂ_Æï³Ë_Ê¨×Óºð";
            action[19] = "ÌìÒôËÂ_Æï³Ë_Ê¨×Óºð";
            action[20] = "ÌìÒôËÂ_Æï³Ë_Ê¨×Óºð";
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
            rangetype = 4;
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
        virtual ~ Skill270Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 269 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 6 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0.48 * skill->GetLevel () * skill->GetLevel () + 5.2 * skill->GetLevel () + 98);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return (10500 - 500 * skill->GetLevel ()) * (100 - 15 * skill->GetT0 ()) / 100;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 40, 42, 44, 46, 47, 48, 51, 53, 55 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (5.8 + 0.2 * skill->GetLevel ());
        }
        float GetAngle (Skill * skill) const
        {
            return (float) (1 - 0.0111111 * (120));
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (5.8 + 0.2 * skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (2.3 + 0.2 * skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (0.48 * skill->GetLevel () * skill->GetLevel () + 5.2 * skill->GetLevel () + 98),
                               20 + skill->GetLevel (),
                               4.8 + 0.2 * skill->GetLevel (),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) * (53 + 5 * skill->GetLevel ()) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (0.43 * skill->GetLevel () * skill->GetLevel () +
                                                                              1.8 * skill->GetLevel () + 58)), 50, 10.5 - 0.5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (INT (zrand (100) / (100 - 6 * skill->GetT0 ())) * 600));
            skill->GetVictim ()->SetTime (1050 * skill->GetT0 ());
            skill->GetVictim ()->SetDizzy (1);
            skill->GetVictim ()->SetProbability (1.0 * (50 + 5 * skill->GetT0 ()));
            skill->GetVictim ()->SetValue (9);
            skill->GetVictim ()->SetRepel (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (20 + skill->GetLevel ());
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.4);
        }
#endif
    };
}
#endif
