#ifndef __CPPGEN_GNET_SKILL280
#define __CPPGEN_GNET_SKILL280
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill280:public Skill
    {
      public:
        enum
        { SKILL_ID = 280 };
          Skill280 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill280Stub:public SkillStub
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
                skill->SetRatio (1 + 0.05 * skill->GetT2 ());
                skill->SetPlus (15 * skill->GetLevel () + skill->GetPlayer ()->GetLevel () * 0.12 * skill->GetT0 () * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill280Stub ():SkillStub (280)
        {
            occupation = 11;
            name = L"¿àº£ÎŞ±ß";
            nativename = "¿àº£ÎŞ±ß";
            icon = "¿àº£ÎŞ±ß.dds";
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
            talent[0] = 675;
            talent[1] = 676;
            talent[2] = 681;
            talent_size = 3;
            action[0] = "ÌìÒôËÂ_¿àº£ÎŞ±ß";
            action[1] = "ÌìÒôËÂ_¿àº£ÎŞ±ß";
            action[2] = "ÌìÒôËÂ_¿àº£ÎŞ±ß";
            action[3] = "ÌìÒôËÂ_¿àº£ÎŞ±ß";
            action[4] = "ÌìÒôËÂ_¿àº£ÎŞ±ß";
            action[5] = "ÌìÒôËÂ_¿àº£ÎŞ±ß";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_¿àº£ÎŞ±ß";
            action[16] = "ÌìÒôËÂ_Æï³Ë_¿àº£ÎŞ±ß";
            action[17] = "ÌìÒôËÂ_Æï³Ë_¿àº£ÎŞ±ß";
            action[18] = "ÌìÒôËÂ_Æï³Ë_¿àº£ÎŞ±ß";
            action[19] = "ÌìÒôËÂ_Æï³Ë_¿àº£ÎŞ±ß";
            action[20] = "ÌìÒôËÂ_Æï³Ë_¿àº£ÎŞ±ß";
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
            rangetype = 2;
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
        virtual ~ Skill280Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 382 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 5 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (139 - 1.4 * skill->GetLevel () * skill->GetLevel () + 105 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return (14500 - 500 * skill->GetLevel ()) * (100 - 15 * skill->GetT0 ()) / 100;
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
        float GetRadius (Skill * skill) const
        {
            return (float) (8 + skill->GetLevel ());
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (8 + skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (4 + skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (139 - 1.4 * skill->GetLevel () * skill->GetLevel () + 105 * skill->GetLevel ()),
                               30,
                               7 + skill->GetLevel (),
                               15 * skill->GetLevel (),
                               INT (1088 + 880 * skill->GetLevel () - 2.4 * skill->GetLevel () * skill->GetLevel ()),
                               14.5 - 0.5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (12000);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount ((1088 + 880 * skill->GetLevel () - 2.4 * skill->GetLevel () * skill->GetLevel ()) *
                                            (1 + 0.33 * skill->GetT1 ()));
            skill->GetVictim ()->SetValue (0.03 * skill->GetT1 () * skill->GetPlayer ()->GetMaxmp ());
            skill->GetVictim ()->SetMpleak (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (30);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
