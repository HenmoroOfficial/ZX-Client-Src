#ifndef __CPPGEN_GNET_SKILL781
#define __CPPGEN_GNET_SKILL781
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill781:public Skill
    {
      public:
        enum
        { SKILL_ID = 781 };
          Skill781 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill781Stub:public SkillStub
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
                skill->SetCrit (0.02 * skill->GetT0 ());
                skill->SetRatio (0.75 + 0.1 * skill->GetT1 ());
                skill->SetPlus (60 * skill->GetLevel () + 5 * skill->GetLevel () * skill->GetLevel () + 55);
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
                skill->SetCrit (0.02 * skill->GetT0 ());
                skill->SetRatio (0.75 + 0.1 * skill->GetT1 ());
                skill->SetPlus (60 * skill->GetLevel () + 5 * skill->GetLevel () * skill->GetLevel () + 55);
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill781Stub ():SkillStub (781)
        {
            occupation = 17;
            name = L"Á¯Ó°";
            nativename = "Á¯Ó°";
            icon = "Á¯Ó°.dds";
            maxlevel = 5;
            maxlearn = 4;
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
            talent[0] = 731;
            talent[1] = 735;
            talent[2] = 744;
            talent[3] = 748;
            talent_size = 4;
            action[0] = "ºÏ»¶ÅÉ_Á¯Ó°";
            action[1] = "ºÏ»¶ÅÉ_Á¯Ó°";
            action[2] = "ºÏ»¶ÅÉ_Á¯Ó°";
            action[3] = "ºÏ»¶ÅÉ_Á¯Ó°";
            action[4] = "ºÏ»¶ÅÉ_Á¯Ó°";
            action[5] = "ºÏ»¶ÅÉ_Á¯Ó°";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_Á¯Ó°";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_Á¯Ó°";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_Á¯Ó°";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_Á¯Ó°";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_Á¯Ó°";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_Á¯Ó°";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 2;
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
            statestub.push_back (new State3 ());
#endif
        }
        virtual ~ Skill781Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 780 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1.2 * skill->GetLevel () * skill->GetLevel () + 12 * skill->GetLevel () + 780);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[4] = { 135, 135, 136, 138 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (12);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (10);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (8);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (1.2 * skill->GetLevel () * skill->GetLevel () + 12 * skill->GetLevel () + 780),
                               25,
                               75,
                               12, INT (60 * skill->GetLevel () + 5 * skill->GetLevel () * skill->GetLevel () + 55), 33, 4, 120 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (33 * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetTime (4000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (120 * skill->GetLevel ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (8000 * (1 + 0.3 * skill->GetT2 ()) + 100);
            skill->GetVictim ()->SetRatio (0.10 + 0.03 * skill->GetT3 ());
            skill->GetVictim ()->SetInchurt2 (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetRatio (293);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (2000);
            skill->GetVictim ()->SetReduceskillcd (1);
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
